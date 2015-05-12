
#version 120


#ifdef VERTEX_SHADER

varying vec2 uv;

void main()
{
        gl_Position = ftransform();

        vec2 inPos = sign(gl_Vertex.xy);

        uv = (vec2(inPos.x, -inPos.y) + 1.0)/2.0;
}



#else

varying vec2 uv;
uniform sampler2D sceneTex;

void main()
{
	//gl_FragColor = texture2D(sceneTex, uv);

	gl_FragColor = vec4(1,0,0,1);
}


#endif
