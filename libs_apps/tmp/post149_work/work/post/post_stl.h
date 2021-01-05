//-< POST_STL.H >----------------------------------------------------*--------*
// POST++                     Version 1.0        (c) 1998  GARRET    *     ?  *
// (Persistent Object Storage)                                       *   /\|  *
//                                                                   *  /  \  *
//                          Created:     31-May-99    K.A. Knizhnik  * / [] \ *
//                          Last update: 31-May-99    K.A. Knizhnik  * GARRET *
//-------------------------------------------------------------------*--------*
// Interface with STL library
//-------------------------------------------------------------------*--------*

#ifndef __POST_STL_H__
#define __POST_STL_H__

#include "object.h"

#ifdef _WIN32
// trucation of long debug names
#pragma warning(disable:4786)
#endif

#include <new>

#ifdef REDEFINE_DEFAULT_ALLOCATOR

#ifdef USE_MICROSOFT_STL 

#define allocator default_allocator
#include <xmemory>
#undef allocator
#define post_alloc allocator

#else // ifdef USE_MICROSOFT_STL

#ifdef USE_OSPACE_LIBRARY

#define OS_STL_ALLOCATE_H
#define post_alloc allocator

#else // ifdef USE_OSPACE_LIBRARY

#ifdef USE_STLPORT
#ifndef USE_STD_ALLOCATORS
#define _STLP_USE_RAW_SGI_ALLOCATORS 1
#endif
#include "stl/_config.h"
#undef _STLP_DEFAULT_ALLOCATOR
#undef _STLP_DEFAULT_ALLOCATOR_SELECT
#undef _STLP_DEFAULT_PAIR_ALLOCATOR
#undef _STLP_DEFAULT_PAIR_ALLOCATOR_SELECT

#ifdef USE_STD_ALLOCATORS
#define _STLP_DEFAULT_ALLOCATOR(_Tp) post_alloc<_Tp>
#define _STLP_DEFAULT_ALLOCATOR_SELECT( _Tp ) __DFL_TMPL_PARAM(_Alloc, post_alloc< _Tp >)
#define _STLP_DEFAULT_PAIR_ALLOCATOR(_Key, _Tp) post_alloc< pair < _Key, _Tp > >
#define _STLP_DEFAULT_PAIR_ALLOCATOR_SELECT(_Key, _Tp ) \
             class _Alloc = post_alloc< pair < _Key, _Tp > >

#else // ifdef USE_STD_ALLOCATORS
#define _STLP_DEFAULT_ALLOCATOR( _Tp )post_alloc
#define _STLP_DEFAULT_ALLOCATOR_SELECT( _Tp ) __DFL_TYPE_PARAM(_Alloc,post_alloc)
#define _STLP_DEFAULT_PAIR_ALLOCATOR( _Key, _Tp ) post_alloc
#define _STLP_DEFAULT_PAIR_ALLOCATOR_SELECT(_Key, _Tp ) __DFL_TYPE_PARAM(_Alloc,post_alloc)
#endif // USE_STD_ALLOCATORS

#else // ifdef USE_STLPORT

#if defined(USE_STD_ALLOCATORS) 
#define __STL_DEFAULT_ALLOCATOR(T) post_alloc<T>
#else
#define __STL_DEFAULT_ALLOCATOR(T) post_alloc
#endif



#endif // USE_STLPORT
#endif // USE_OSPACE_LIBRARY
#endif // USE_MICROSOFT_STL
#endif // ifdef REDEFINE_DEFAULT_ALLOCATOR


#if defined(USE_STD_ALLOCATORS) || defined(USE_MICROSOFT_STL)

#ifndef NO_NAMESPACES
namespace std { 
#endif
template<class _Tp> inline
void _DestroyPtr(_Tp *_P) { _P->~_Tp(); }

class basic_post_alloc {
public:
};

template<class _Tp>
class POST_DLL_ENTRY post_alloc : public basic_post_alloc {
public:
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef _Tp *pointer;
    typedef const _Tp *const_pointer;
    typedef _Tp& reference;
    typedef const _Tp& const_reference;
    typedef _Tp value_type;
    pointer address(reference _X) const {
	return (&_X); 
    }
#if 0 // this code is not compilable if _Tp is already const type
    const_pointer address(const_reference _X) const {
	return (&_X); 
    }
#endif
 
    post_alloc(basic_post_alloc const&) {}
    post_alloc() {}
   
    template <class _Tp1> struct rebind {
	typedef post_alloc<_Tp1> other;
    };

    void construct(pointer _P, const _Tp& _V) {
	new (_P) _Tp(_V);
    }
    void destroy(pointer _P) { 
	_DestroyPtr(_P);
    }
	
    size_t max_size() const { 
	size_t _N = (size_t)(-1) / sizeof (_Tp);
	return (0 < _N ? _N : 1); 
    }

    _Tp* allocate(size_t __n, void const*) { 
	return allocate(__n);
    }

    _Tp* raw_allocate(size_type each_object_size = 1,
        size_type n_objects = 1,
        const void* hint = 0)
    { 
	return allocate(each_object_size*n_objects, hint);
    }

    char* _Charalloc(size_t __n) { 
	return (char*)allocate(__n);
    }

    _Tp* allocate(size_t __n = 1) { 
	if (__n == 0) { 
	    return 0;
	}
	POST_NAMESPACE storage* store = POST_NAMESPACE storage::find_storage((POST_NAMESPACE object*)this);
	if (store == NULL) { 
#ifdef REDEFINE_DEFAULT_ALLOCATOR
	    return (_Tp*)malloc(__n*sizeof(_Tp));
#else
	    store = POST_NAMESPACE storage::get_current_storage();
#endif
	} 
	POST_NAMESPACE post_raw_object* a = (POST_NAMESPACE post_raw_object*)store->get_static_data();
	if (a == NULL) { 
	    a = POST_NAMESPACE post_raw_object::create(*store, __n*sizeof(_Tp));
	    store->set_static_data(a);
	}
	return (_Tp*)a;
    }

    void deallocate(void* __p, size_t __n = 1) { 
	POST_NAMESPACE storage* store = POST_NAMESPACE storage::find_storage((POST_NAMESPACE object*)__p);
	if (store != NULL) { 
#if defined(USE_MICROSOFT_STL)
	    if (!store->is_static_data(__p))
#endif
	    store->free((POST_NAMESPACE object*)__p);
	} else { 
	    free(__p);
	}
    }

    void raw_deallocate( void* p ) { 
	deallocate(p);
    }
};

template<class _Tp, class _Tq> inline
bool operator==(const post_alloc<_Tp>&, const post_alloc<_Tq>&) {
    return (false); 
}

template<class _Tp, class _Tq> inline
bool operator!=(const post_alloc<_Tp>&, const post_alloc<_Tq>&) {
    return (true); 
}

#ifdef USE_STLPORT
template <class _Tp1, class _Tp2> 
inline post_alloc<_Tp2>&  _STLP_CALL
__stl_alloc_rebind(post_alloc<_Tp1>& __a, const _Tp2*) { return (post_alloc<_Tp2>&)(__a); }

template <class _Tp1, class _Tp2>
inline post_alloc<_Tp2> _STLP_CALL
__stl_alloc_create(const post_alloc<_Tp1>&, const _Tp2*) { return post_alloc<_Tp2>(); }
#endif


#define __STL_USE_STD_ALLOCATORS

#ifdef REDEFINE_STRING

class POST_DLL_ENTRY static_post_alloc {
  public: 
    template <class _Tp1> struct rebind {
	typedef post_alloc<_Tp1> other;
    };
    static void* allocate(size_t __n)  {
	POST_NAMESPACE storage* store = POST_NAMESPACE storage::get_current_storage();
	assert(store != NULL);
	return POST_NAMESPACE post_raw_object::create(*store, __n);
    }

    static void deallocate(void* __p, size_t /* __n */) {
	POST_NAMESPACE storage* store = POST_NAMESPACE storage::find_storage((POST_NAMESPACE object*)__p);
	assert(store != NULL);
	store->free((POST_NAMESPACE object*)__p);
    }
};
#if  defined(__GNUC__) && __GNUC__ >= 3
#include <bits/straits.h>
#include <bits/bastring.h>
#else
#include <std/straits.h>
#include <std/bastring.h>
#endif
typedef basic_string<char, string_char_traits<char>, static_post_alloc > string;
#define __STRING__
#endif // REDEFINE_STRING

#ifndef NO_NAMESPACES
}
#endif

#else //  defined(USE_STD_ALLOCATORS) || defined(USE_MICROSOFT_STL) 

class POST_DLL_ENTRY post_alloc {
  public:
    static void* allocate(size_t __n)  {
	POST_NAMESPACE storage* store = POST_NAMESPACE storage::get_current_storage();
	assert(store != NULL);
	return POST_NAMESPACE post_raw_object::create(*store, __n);
    }

    static void deallocate(void* __p, size_t /* __n */) {
	POST_NAMESPACE storage* store = POST_NAMESPACE storage::find_storage((POST_NAMESPACE object*)__p);
	assert(store != NULL);
	store->free((POST_NAMESPACE object*)__p);
    }
};

#if defined(REPL_RW_ALLOCATORS)

template <class T>
class POST_DLL_ENTRY static_post_alloc {
  public:
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef T *pointer;
    typedef const T *const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T value_type;

    static_post_alloc(){};
    ~static_post_alloc(){};

    void* allocate(size_t __n, void* = 0)  {
        POST_NAMESPACE storage* store = POST_NAMESPACE storage::get_current_storage();
        assert(store != NULL);
        return POST_NAMESPACE post_raw_object::create(*store, __n);
    }

    void deallocate(void* __p, size_t /* __n */) {
        if(__p)
        {
            POST_NAMESPACE storage* store = POST_NAMESPACE storage::find_storage((POST_NAMESPACE object*)__p);
            assert(store != NULL);
            store->free((POST_NAMESPACE object*)__p);
        }
    }
 
    size_t max_size(size_type size) const {
        size_t _N = (size_t)(-1) / size;
        return (0 < _N ? _N : 1);
    }
};
 
#ifdef REDEFINE_STRING
#include <string>
#ifndef NO_NAMESPACES
using namespace std;
#endif
typedef basic_string<char, char_traits<char>, static_post_alloc<char> > Pstring;   
#endif
#endif


#endif

#if defined(__NO_NEW_DEFINED__) 
inline void* operator new(size_t, void* addr) 
#ifndef USE_MICROSOFT_STL
throw (std::bad_alloc) 
#endif
{ 
    return addr;
}
#endif

inline void* operator new(size_t size, POST_NAMESPACE storage& store) { 
    POST_NAMESPACE post_raw_object* a = (POST_NAMESPACE  post_raw_object*)store.get_static_data();
    if (a == NULL) { 
	a = POST_NAMESPACE post_raw_object::create(store, size);
	store.set_static_data(a);
    }
    return a;
}

inline void* operator new(size_t size) 
#if !defined(_STLPORT_VERSION) && !defined(USE_MICROSOFT_STL)
throw (std::bad_alloc) 
#endif
{ 
    return malloc(size);
}

inline void operator delete(void* ptr, POST_NAMESPACE storage&) throw() 
{
    POST_NAMESPACE storage* store = POST_NAMESPACE storage::find_storage((POST_NAMESPACE object*)ptr);
    if (store != NULL) { 
	store->free((POST_NAMESPACE object*)ptr);
    } else { 
	free(ptr);
    }
}

#endif
