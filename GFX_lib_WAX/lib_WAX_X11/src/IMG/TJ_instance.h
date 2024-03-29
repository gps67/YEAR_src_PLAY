#ifndef TJ_instance_H
#define TJ_instance_H

#include "TJ_STUBS.h" // TJ::
#include <turbojpeg.h> // tjhandle
#include "str0.h"

namespace TJ {

 struct TJ_instance_t  {

	tjhandle tjInstance;

	TJ_instance_t();

	bool expect_instance_NULL();
	bool set_instance_check( tjhandle _tjInstance );
	bool set_instance_NULL();
	bool set_instance( tjhandle _tjInstance );
	bool set_instance_InitTransform();
	bool set_instance_InitCompress();
	bool set_instance_InitDecompress();

	STR0 get_error_str();

 }; // class
}; // namespace


#endif
