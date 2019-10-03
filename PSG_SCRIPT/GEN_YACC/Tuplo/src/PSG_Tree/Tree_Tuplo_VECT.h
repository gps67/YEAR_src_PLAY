#ifndef Tree_Tuplo_VECT_H
#define Tree_Tuplo_VECT_H

 class Tuplo_CSR : public obj_ref { public:
 	virtual ~Tuplo_CSR() {} // VTBL available
	 u32 * CSR; // see Tuplo as OBJV[ i ] // objv .P0P2 .P0 = P
 	Tuplo_CSR() {
		CSR = P0P2_P0 = P;
	} 
	operator=( Tuplo_CSR * RHS ) {
		CSR = RHS->CSR;
	}
	operator=( EA_EXPR EA ) {
		CSR = PTR_from_EA_EXPR( EA );
	}
 };


#endif

