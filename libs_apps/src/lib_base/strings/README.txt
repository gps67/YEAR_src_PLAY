see also lib_base/buffer/buffer1.h 
see also lib_base/buffer/str_builder.h

	str0 works in C++ because it casts to and from char * str;
	BUT when passed into ... VAR_ARGS it is not cast

	SO INFO( "%s", (stro) str ); // FAILS HERE
	SO INFO( "%s", (STR0) str ); // REQUIRED
