#include "dgb.h"
#include "TJ_instance.h"

using namespace TJ;


	TJ_instance_t:: TJ_instance_t()  {
		tjInstance = NULL;
	}

	bool TJ_instance_t:: expect_instance_NULL() {
		if( tjInstance ) {
			return WARN("expected NULL tjInstance");
		}
		return true;
	}

	bool TJ_instance_t:: set_instance_NULL() {
		return set_instance( NULL );
	}

	bool TJ_instance_t:: set_instance_check( tjhandle _tjInstance ) {
		if( !_tjInstance ) {
			return FAIL("NULL instance turbo jpeg error %s",
				get_error_str() );
		}
		return set_instance( _tjInstance );
	}

	bool TJ_instance_t:: set_instance( tjhandle _tjInstance ) {
		if( tjInstance ) {
			tjDestroy(tjInstance);
		}
		tjInstance = _tjInstance;
		return true;
	}

	bool TJ_instance_t:: set_instance_InitTransform() {
		return set_instance_check( tjInitTransform() );
	}

	bool TJ_instance_t:: set_instance_InitCompress() {
		return set_instance_check( tjInitCompress() );
	}

	bool TJ_instance_t:: set_instance_InitDecompress() {
		expect_instance_NULL(); // WARN // stay to overwrite //
		return set_instance_check( tjInitDecompress() );
	}

	STR0 TJ_instance_t:: get_error_str() {
		const char * err = tjGetErrorStr2(tjInstance);
		WARN("err %s", err);
		return err;
	}
