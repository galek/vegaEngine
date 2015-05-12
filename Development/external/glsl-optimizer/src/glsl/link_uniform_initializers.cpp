/*
 * Copyright © 2012 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include "main/core.h"
#include "ir.h"
#include "linker.h"
#include "ir_uniform.h"
#include "glsl_symbol_table.h"
#include "program/hash_table.h"

/* These functions are put in a "private" namespace instead of being marked
 * static so that the unit tests can access them.  See
 * http://code.google.com/p/googletest/wiki/AdvancedGuide#Testing_Private_Code
 */
namespace linker {

gl_uniform_storage *
get_storage(gl_uniform_storage *storage, unsigned num_storage,
	    const char *name)
{
   for (unsigned int i = 0; i < num_storage; i++) {
      if (strcmp(name, storage[i].name) == 0)
	 return &storage[i];
   }

   return NULL;
}

void
copy_constant_to_storage(union gl_constant_value *storage,
			 const ir_constant *val,
			 const enum glsl_base_type base_type,
			 const unsigned int elements)
{
   for (unsigned int i = 0; i < elements; i++) {
      switch (base_type) {
      case GLSL_TYPE_UINT:
	 storage[i].u = val->value.u[i];
	 break;
      case GLSL_TYPE_INT:
      case GLSL_TYPE_SAMPLER:
	 storage[i].i = val->value.i[i];
	 break;
      case GLSL_TYPE_FLOAT:
	 storage[i].f = val->value.f[i];
	 break;
      case GLSL_TYPE_BOOL:
	 storage[i].b = int(val->value.b[i]);
	 break;
      case GLSL_TYPE_ARRAY:
      case GLSL_TYPE_STRUCT:
      case GLSL_TYPE_ATOMIC_UINT:
      case GLSL_TYPE_INTERFACE:
      case GLSL_TYPE_VOID:
      case GLSL_TYPE_ERROR:
	 /* All other types should have already been filtered by other
	  * paths in the caller.
	  */
	 assert(!"Should not get here.");
	 break;
      }
   }
}

void
set_uniform_binding(void *mem_ctx, gl_shader_program *prog,
                    const char *name, const glsl_type *type, int binding)
{
   struct gl_uniform_storage *const storage =
      get_storage(prog->UniformStorage, prog->NumUserUniformStorage, name);

   if (storage == NULL) {
      assert(storage != NULL);
      return;
   }

   if (storage->type->is_sampler()) {
      unsigned elements = MAX2(storage->array_elements, 1);

      /* From section 4.4.4 of the GLSL 4.20 specification:
       * "If the binding identifier is used with an array, the first element
       *  of the array takes the specified unit and each subsequent element
       *  takes the next consecutive unit."
       */
      for (unsigned int i = 0; i < elements; i++) {
         storage->storage[i].i = binding + i;
      }

      for (int sh = 0; sh < MESA_SHADER_STAGES; sh++) {
         gl_shader *shader = prog->_LinkedShaders[sh];

         if (shader && storage->sampler[sh].active) {
            for (unsigned i = 0; i < elements; i++) {
               unsigned index = storage->sampler[sh].index + i;

               shader->SamplerUnits[index] = storage->storage[i].i;
            }
         }
      }
   } else if (storage->block_index != -1) {
      /* This is a field of a UBO.  val is the binding index. */
      for (int i = 0; i < MESA_SHADER_STAGES; i++) {
         int stage_index = prog->UniformBlockStageIndex[i][storage->block_index];

         if (stage_index != -1) {
            struct gl_shader *sh = prog->_LinkedShaders[i];
            sh->UniformBlocks[stage_index].Binding = binding;
         }
      }
   }

   storage->initialized = true;
}

void
set_uniform_initializer(void *mem_ctx, gl_shader_program *prog,
			const char *name, const glsl_type *type,
			ir_constant *val)
{
   if (type->is_record()) {
      ir_constant *field_constant;

      field_constant = (ir_constant *)val->components.get_head();

      for (unsigned int i = 0; i < type->length; i++) {
	 const glsl_type *field_type = type->fields.structure[i].type;
	 const char *field_name = ralloc_asprintf(mem_ctx, "%s.%s", name,
					    type->fields.structure[i].name);
	 set_uniform_initializer(mem_ctx, prog, field_name,
				 field_type, field_constant);
	 field_constant = (ir_constant *)field_constant->next;
      }
      return;
   } else if (type->is_array() && type->fields.array->is_record()) {
      const glsl_type *const element_type = type->fields.array;

      for (unsigned int i = 0; i < type->length; i++) {
	 const char *element_name = ralloc_asprintf(mem_ctx, "%s[%d]", name, i);

	 set_uniform_initializer(mem_ctx, prog, element_name,
				 element_type, val->array_elements[i]);
      }
      return;
   }

   struct gl_uniform_storage *const storage =
      get_storage(prog->UniformStorage,
		  prog->NumUserUniformStorage,
		  name);
   if (storage == NULL) {
      assert(storage != NULL);
      return;
   }

   if (val->type->is_array()) {
      const enum glsl_base_type base_type =
	 val->array_elements[0]->type->base_type;
      const unsigned int elements = val->array_elements[0]->type->components();
      unsigned int idx = 0;

      assert(val->type->length >= storage->array_elements);
      for (unsigned int i = 0; i < storage->array_elements; i++) {
	 copy_constant_to_storage(& storage->storage[idx],
				  val->array_elements[i],
				  base_type,
				  elements);

	 idx += elements;
      }
   } else {
      copy_constant_to_storage(storage->storage,
			       val,
			       val->type->base_type,
			       val->type->components());

      if (storage->type->is_sampler()) {
         for (int sh = 0; sh < MESA_SHADER_STAGES; sh++) {
            gl_shader *shader = prog->_LinkedShaders[sh];

            if (shader && storage->sampler[sh].active) {
               unsigned index = storage->sampler[sh].index;

               shader->SamplerUnits[index] = storage->storage[0].i;
            }
         }
      }
   }

   storage->initialized = true;
}
}

void
link_set_uniform_initializers(struct gl_shader_program *prog)
{
   void *mem_ctx = NULL;

   for (unsigned int i = 0; i < MESA_SHADER_STAGES; i++) {
      struct gl_shader *shader = prog->_LinkedShaders[i];

      if (shader == NULL)
	 continue;

      foreach_list(node, shader->ir) {
	 ir_variable *const var = ((ir_instruction *) node)->as_variable();

	 if (!var || var->data.mode != ir_var_uniform)
	    continue;

	 if (!mem_ctx)
	    mem_ctx = ralloc_context(NULL);

         if (var->data.explicit_binding) {
            linker::set_uniform_binding(mem_ctx, prog, var->name,
                                        var->type, var->data.binding);
         } else if (var->constant_value) {
            linker::set_uniform_initializer(mem_ctx, prog, var->name,
                                            var->type, var->constant_value);
         }
      }
   }

   ralloc_free(mem_ctx);
}
