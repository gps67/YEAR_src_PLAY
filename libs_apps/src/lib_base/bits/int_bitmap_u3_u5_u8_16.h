#ifndef int_bitmap_u3_u5_u8_u16_H
#define int_bitmap_u3_u5_u8_u16_H

	/*
  		#define u3_u5_u8_u16 bitmap // MACRO
  		#define u3_u5_u8_u16 bitmap // obtain from CORE.REGISTER.ea_that();
	*/

  struct int_bitmap_u3_u5_u8_u16 {
  
  	int_bitmap	bitmap;
  	// alias u3_u5_u8_u16 // bitmap
  	// alias u3_u5_u8_u16 bitmap
  	// alias bitmap u3_u5_u8_u16

  	
  };

  /*
  	VAR DECODER_in_SCRIPT {
		VAR_is_LINK // LINK_to_CORE_REGISTER( EXPR( u3 ) ) {...}
		VAR_is_u64_hilo // in a register, or maybe a plan-B
		VAR_is_u3_u5_u8_u16_EXPR // SCRIPT = DECODER CTXT u64_hilo
	int u3_get() => bitmap >> CT_u3_bit_pos.as_shift_pos
  */


#endif

