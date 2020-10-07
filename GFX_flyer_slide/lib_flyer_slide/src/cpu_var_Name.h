#ifndef FT2__H
#define FT2__H

#include "obj_ref.h"

namespace WAX {

 class cpu_var : public obj_ref {
	// we need this to be a singleton
	// or do we ? one shared vs a few threaded
	// so move to a static
	static bool init_done; // init syntax // bool cpu_var:: init_done = false;
  public:
	// PATCH // SEGMENT.ON_INIT.set_ROM_init_done_bool_false // or SIMPLER
	// OPTION // set_rom_varname_type_initval ARGV //
	// ARGV // create_on_first_mention

  	cpu_var();
  	~cpu_var();
	bool OK_or_ERR( int err ); // 0 means OK // convert to bool // report
	bool init();
	bool done();
	bool test1();

 };

};

namespace WORDS {
	/*
		BITS	16-32-54
		SIGNED	unsigned signed
		CPU	ARM32 ARM64 ia32 ia64 C32 C64

		Cross product of those
		
		LOHI = new default !!
		HILO ~ BYTE_A BYTE_B u16_BA_lohi u16_AB_hilo
		BYTE_B

	*/

	struct WORD_16 {
	};
	struct WORD_32 {
		u32 get_u32() { return word; }
	};
	struct WORD_64 {
	};

	struct WORD_i16 {
	};
	struct WORD_i32 {
	};
	struct WORD_i64 {
	};
};

#endif

