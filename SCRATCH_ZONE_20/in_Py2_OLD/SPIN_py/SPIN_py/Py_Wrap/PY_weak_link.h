#ifndef PY_weak_link_H
#define PY_weak_link_H

#include "PY_Object_base.h"
struct PY_weak_base;

extern PyTypeObject PY_weak_link_Type;

/*


     +------------------------+
     V                        |
     |                        ^
  +-(base)--+                 |
  | .weak  --------+          |
  +---------+      |          |
  | .more   |      |          |
  | .data   |      |          |
  +---------+      |          |
                   |          |
                   |          |
                   V          |
                   |          |
  child ->+        |          |
          |        |          |
          +-->  +-(link)--+   |
  child ----->  | .link  -----+
          +-->  +---------+
          |
  child ->+


*/

/*!
	A weak link can only point to an ENABLED base type

	A weak link points to the real item OR NULL

	It has many refcounts, but does NOT hold its own refcount to item.

	The real item has a link to this, and controls this

	The real item can be deleted by its own refcount, leaving the
	(many uplink) weak links set to null.

	Cousins that refer to this link MUST check item == NULL

	That happens when the weak_item (which is the strong side)
	gets deleted, but something still points to this/it.
*/
// struct PY_weak_link : public PY_Object_base
struct PY_weak_link : public PY_Object_plain // dont need var_pool
{
	PY_weak_base * item;

	PY_weak_link(
		PY_weak_base * _item
	)
	: PY_Object_plain( & PY_weak_link_Type )
	, item( 0 )
	{
		item = _item; // no refcount
	}

	PY_weak_base * get_item_or_NULL() 
	{
		return item;
	}

	void declare_base_item_has_gone()
	{
		item = 0; // 
	}

	bool link_broken()
	{
		return item == NULL;
	}

	bool link_intact()
	{
		return item != NULL;
	}

	~PY_weak_link()
	{
		item = 0; // forget
	}
};


template <class BASE>
struct P_weak_link : public P_ref<PY_weak_link>
{
	P_weak_link( BASE * item = NULL )
	{
		if( item ) claim_weak( item );
	}

	bool link_intact()
	{
		return ref && self()->link_intact();
	}

	bool link_broken()
	{
		return !link_intact();
	}

	void claim_weak( BASE * item )
	{
		if(!item)
			release();
		else
			claim_this( item -> get_weak_link() );
	}

	BASE * link_target()
	{
		if(!ref) return NULL; // shouldnt happen - no init to weak yet
		return (BASE*) self()->get_item_or_NULL();
	}

	bool operator==( const PY_weak_link & rhs )
	{
		if( !rhs.item )
		{
			if( !ref ) return true;
			return false;
		}
		return ref == (PY_Object_plain *) rhs.item;
	}

};



#endif
