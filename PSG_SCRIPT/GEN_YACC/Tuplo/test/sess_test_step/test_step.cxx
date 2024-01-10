/*
	test_step xFF u8
	test_step xFFF u12
	test_step xFFFF u16
	test_step xFFFF_FF u24
	test_step xFFFF_FFFF u32
	test_step xFFFF_FFFF u32 { u24_u8 } // BYTE is lo in register WORD
	test_step xFFF u48 // OPTION u48_idx_expr_u8_u8_BYTE_B_BYTE_A // LOHI
	test_step xFFF u64_lohi_ _in_mem _in_cpu as_MEM_u32_u32 or CPU_PAIRyyp
	test_step xFFF u64_WORD _in_mem _in_cpu as_MEM_u32_u32 or CPU_PAIRyyp
	as_MEM_u32_u32 // on CACHED WORD MKVAR AVAR // FUTURE_u64_on_u32

		We track HALF LOHI LO HI OTHER_HALF u32_LOHI 
		 LOADED(%s) u32_LOHI as u32_IDX_EXP
		 DECODED(u32) u24_u8 // LOADED_from_MEM_LOHI


		u64_WORD_u32_LO
		u64_WORD_u32_HI

		DIALECT rely on GCC to OPTIMISE u32_in_MEM _in_CPU
		CACHE includes use of CPU_REGISTERS in CPU _in_MEM

		DESIGN we chose aligned WORD PAIR as WORD_array _as_PAIRS
		_PAIR_HALF_LO u32_LO
		_PAIR_HALF_HI u32_HI
		// += NEXT_LINE // STREAM to FILTER LINES // MULTI_LINE_LINE
		// LINE_BUNDLE_ARRIVED _on_CALL RETOKENISE_CONTAINED_EVAL
		// EVAL TEXT_INDENTED_BLOCK // recognise VAGUE_DIALECT

	TESTS are allocated POINTS of IDX [0[N vary_u8_u48

		POINT = TEST mount point, attachment point, CODE_ DATA_

		array[ IDX ] of_EIGHT

		u32_u32_PAIR EIGHT ITEM_in_ARRAY

		MACRO AMD_64

	MACRO

		u32 WORD _from_ARRAY [idx] of N N_of_idx CT_RT
		ACCESS_MACROS that make sense when in x86_32 _
		ACCESS_MACROS that make sense when in x86_16 _
		ACCESS_MACROS that make sense when in x86_u8 _
		ACCESS_MACROS that make sense when in ARM_32 _
		ACCESS_MACROS that make sense when in ESP_32 _
		ACCESS_MACROS that make sense when in AMD_64 _
		ACCESS_MACROS that make sense when in ARM_64 _
		ACCESS_MACROS that make sense when in GCC_32 _
		ACCESS_MACROS that make sense when in GCC_64 _
	
	BREAD BOARD wires around ASM_STEPS

		u32_WORD = ARRAY[ idx ] // idx = idx_PAIR *2 + { @LO @HI }
		@ tells PARSER to convert TYPE BUILD EXPRS TRY CACHED
		u24_idx = u32_WORD >> 8 // MACRO attaches new name "u24_idx"
		bytes bytes_of_OPCODE_and_CT_DATA

		ASM_STEP u32_LO = ARY[IDX].LO
		ASM_STEP u32_HI = ARY[IDX].HI

		ASM_STEP u32_LO = DECODES_AS u24_IDX u8_OPCODE
		ASM_STEP u32_HI = DECODES_AS u24_IDX u8_OPCODE // SAME_CODE

		CT_ {
		 PICK_LO == BIT0 == 0
		 LICK_HI == BIT0 == 1 
		 IDX_PAIR = IDX_EACH >> 1 ;

		 DESIGN EVEN PAIR as ITEM_PAIR
		 DESIGN MEMORY ACCESS GET SET CMP EVAL SCRIPT
		 u32[ u24_idx ] // u32_also_ _both reduce to u32 in CPU_REG
		 DECODE( u32_WORD * PTR )  
		 DECODE( u24_u8 * 32_WORD )  // BIND to CODE written
		 REG2 = WORD>>8 
		 REG3 = WORD>>16
		 REG4 = BASE
		 REG5 = "OFFS" idx * 4 + OFFS_of_BASE + adj_within_word
		 REG5 = "OFFS" idx * 8 + OFFS_of_BASE + adj_within_word
		 WORD = "*( BASE + OFFS + IDX * 8 )"
		}

		ASM_STEP u32_HI = ARY[IDX].HI u8_OPCODE
	CX	ASM_STEP u32_HI = ARY[IDX].HI u8_OPCODE
		


*/
