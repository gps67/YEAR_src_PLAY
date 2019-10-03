#if 0
	STO/
	 SEQ/
	  VECT/
	   Tuplo_Obj/
	    inline_and_channel_pointer


	    So thats a VFS Filter as a script

	    PICK
	     channel_pointer 
	      Option *
	      Option **
	      PICK = _pick_one_ _tokenise_PICK_string_ lookup__from_("anystr")

	    LAYER
	     channel merger
	      Layer #
	      Layer # #
	      multiplex the lines as they happen in the stream
	      visual effects start with the editor
	      lines happen in order listed in { DOCUMENT } // GIT_VERSION //

	    SCRIPT
	     channel Layer + JOIN_SCRIPTS // some pre-compiled for STO
	      LIBR #
	      LIBR # #

	    So INLINE -vs- POINTER // BOTH COMMON

	     EG
	      Tuplo is u32_VECT_Tuplo

	      	tuplo = tuplo_root

		CONTAINER lib_Tuplo_STO

			MMAP_FILE

				One entire region, not shuffled, shared region

			HEAP_FILE

				u32_OFFS_EXPR
				u32_OFFS = _EXPR_
				u32_hilo *
				u32_hilo []
				u32_hilo [u16] // 64K * 4 bytes = 256 KB
				// 256 KB used as u32_WORD_Buffer
				// u32_hilo = STO_WORD[ u16 ]
				// hilo = OBJV[ u16 ]
				//  OBJV = ARGV_of_WORD_or_TEXT
				//  OBJV = Tree_in_4_segments
				// Tree_in_4_segments
				//  segment * SEG [ 4 ]
				//  tupulo * tuplo_root;
				// NB T * Obj; // * is EA_EXPR over VFS DATA
			
			STREAM_FILE

				// PKT_IN is our outer multiplexer
				// VFS_ROM is our inner multiplexer

			STREAM from OUTER
				// transfer STO = KB256 + META
				// root node switches to next version
				// by moving EA_tuplo_root = LOCN_in_FILE_FOUR

				// FILE_FOUR //[1]
				//  FILE_one
				//  FILE_two
				//  FILE_three
				//  FILE_four

			STREAM from INNER
				STO KB_256 
				FILE[ u16 * 4 ]


	[1] // FILE_FOUR // 
		when used as STRUCT count from ONE
		when used as VECT   count from ZERO
		when used as BUFFER count from P0P2

#endf

