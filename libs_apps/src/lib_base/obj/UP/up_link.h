

 virtual
 obj_ref_0 * as_weak_link()
 {
	FAIL("This class does not support weak_link");
	return NULL;
 }

 virtual
 void forget_own_weak_link()
 {
	// called within destructor, for every single object :-(
 }

 template <class ROOT>
 struct up_step<ROOT> ;
 template <class ROOT>
 up_step<ROOT> * obj_own_weak_link;
 template <class ROOT>
 virtual
 up_step<ROOT> *
 as_weak_link()
 {
	if(!obj_own_weak_link) {
		obj_own_weak_link = new up_step<ROOT>( this );
	}
	return obj_own_weak_link;
 }

 virtual
 void forget_own_weak_link()
 {
	// called within destructor
	if(obj_own_weak_link) {
		obj_own_weak_link = new up_step<ROOT>( this );
	}
 }

namespace weaklink {

 // essentially an up_link is an obj_hold<ROOT> upgraded to an obj_ref_0
 // ie, we count pointers to THIS step, which holds one ref_count to ROOT
 // could probably have multiple inheritance obj_ref_0, obj_hold<T>
 // this limits actual usage to fewer parts?

 template <class ROOT>
 struct up_step : public obj_ref_0 // not _js ?
 {
	obj_hold<ROOT> weak_link2;

	up_step(ROOT * link = NULL) weak_link2( link ) {}

	bool is_null() { return (bool) weak_link2; }

	void set_link(ROOT * link) { weak_link2 = link; }

	ROOT * get_link() { return weak_link2; }
	operator ROOT * () {
		return (ROOT *) weak_link2;
	}

	~up_step() {
		if( weak_link2 ) {
			weak_link2 -> forget_own_weak_link();
		}
	}
 };


 template <class ROOT>
 struct up_link : public GRP_lib_base // ie NOTHING
 {
  private:
	obj_hold<ROOT> weak_link2;

	up_step(ROOT * link) weak_link2( link ) {}

	bool is_null() { return (bool) weak_link2; }

	void set_link(ROOT * link) { weak_link2 = link; }

	ROOT * get_link() { return weak_link2; }
	operator ROOT * () {
		return (ROOT *) weak_link2;
	}
 };

 template <class ROOT>
 struct up_link : public obj_hold<up_step<ROOT>> // with mild adjustment
 {
	bool is_null() { return ((bool) ptr) && ((bool) (ptr->weak_link2));}
	operator ROOT * () {
		if(!ptr) return NULL;
		return (ROOT *) (*ptr);
	}
 };

};
