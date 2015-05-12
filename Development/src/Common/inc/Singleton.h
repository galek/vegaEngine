/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#ifndef __SINGLETON_H__
#define __SINGLETON_H__


#ifndef __PLATFORM_H_
#include "..\platform\platform.h"
#endif

namespace vega
{
	/*
	*/
	template <typename T> class Singleton
	{
	public:
		Singleton( void )
		{
			assert( !object );
#if defined( _MSC_VER ) && _MSC_VER < 1200	 
			int offset = (int)(T*)1 - (int)(Singleton <T>*)(T*)1;
			object = (T*)((int)this + offset);
#else
			object = static_cast< T* >( this );
#endif
		}
		~Singleton( void )
		{  assert( object );  object = 0;  }
		static T& SingletonIn( void )
		{	assert( object );  return ( *object ); }
		static T* SingletonPtr( void )
		{ return object; }

	private:
		/** \brief Explicit private copy constructor. This is a forbidden operation.*/
		Singleton(const Singleton<T> &);

		/** \brief Private operator= . This is a forbidden operation. */
		Singleton& operator=(const Singleton<T> &);

	protected:

		static T* object;

	};


	/*
	*/
	template <class Type> Type *Singleton<Type>::object = 0;
}

#endif /* __SINGLETON_H__ */
