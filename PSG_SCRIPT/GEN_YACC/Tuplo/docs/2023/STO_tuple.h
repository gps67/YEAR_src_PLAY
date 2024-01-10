
namespace STO {

	class Tuplo : obj_ref0 { // FIXED VECTOR // with personality TYPE
	 virtual ~Tuplo() {}; // we need to fix DTORS when source IS available
	};

/*
	class PAIR : Tuplo {
		LHS
		RHS
	};

 ALSO DIDNT LHS = VECTRO[0]

	ObjRef & LOCAL = EA_EXPR_FROM_SOURCE( VECTOR[i] ) // vectro

	 CMD = VECTRO[0]
	 LHS = VECTRO[1]
	 RHS = VECTRO[2]

	 CMD = ARGV[0] ; shift ;
	 CMD = Class_VTBL * TuploType

 But am expecting to be able to say in Subclass UDEF

 	Object * Name2 // is an alias for // LHS


	struct Tuple : Tuplo {
	};

	struct PAIR : Tuple {
		Class 
		Object * LHS;
		Object * RHS;
	};

	struct 

}; // namespace
