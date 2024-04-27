#ifndef TCL_PLUS_BASE_H
#define TCL_PLUS_BASE_H

#include "TCL_ObjType_PLUS.h" // LEX1

namespace TCL {
 struct TCL_PLUS_BASE : public TCL_ObjType_PLUS
 {
 
 	virtual ~TCL_PLUS_BASE()
	{
		// we are VTBL
	}

 	TCL_PLUS_BASE( const char * ABB ) // "DICT" LEX1" "CXX" "C" 
	: TCL_ObjType_PLUS( ABB )
	{
	}

  static
	bool is_PLUS_type( const Tcl_Obj * obj )
        {
                if(!obj) return false;
                if(!obj->typePtr) return false;
                return  obj->typePtr -> dupIntRepProc == GET_KEPT_PLUS_PTR_GLOBAL();
        }

  static
	bool is_PLUS_type( const Tcl_ObjType * typePtr )
        {
                if(!typePtr) return false;
                return  typePtr -> dupIntRepProc == GET_KEPT_PLUS_PTR_GLOBAL();
        }

  static
        bool get_PLUS_type( const Tcl_Obj * obj, TCL_PLUS_BASE *& PLUS )
        {
         //     return TCL_PLUS_BASE:: get_PLUS_type( obj, PLUS );
              if( !is_PLUS_type( obj )) return false;
              // see _PLUS.h
              PLUS = (TCL_PLUS_BASE *) get_PLUS_from_typePtr( obj->typePtr );
              return true;
	}

 };
};

#endif

