#if 0
 //
 // IMPORT Tuplo.uno_duo_tres
 //
 //	CTOR_STO_tuple.h
 //


 //	CTOR_STO_tuple.h

	/* CTOR */ Tuplo ( }; 	// MATCH A_R_G_S //
	/* CTOR */ Tuplo ( ITEM }; 	// MATCH .X. //
	/* CTOR */ Tuplo ( LHS RHS }; 	// MATCH .X. //
	/* CTOR */ Tuplo ( LHS RHS }; 	// CLASS PAIR._Tuplo_Specific //
	/* CTOR */ Tuplo ( LHS RHS MORE };  // CLASS TRIP._Tuplo_Specific //

	/* CTOR */ Tuplo ( }; 	// MATCH THIS_CLASS_VTBL // WITH VTBL
	/* CTOR */ Tuplo ( ITEM }; 	// MATCH .X. //
	/* CTOR */ Tuplo ( LHS RHS }; 	// MATCH .X. //
	/* CTOR */ Tuplo ( LHS RHS }; 	// CLASS PAIR._Tuplo_Specific //
	/* CTOR */ Tuplo ( LHS RHS MORE };  // CLASS TRIP._Tuplo_Specific //

	PAIR LHS RHS is not unique
	PAIR Tuplo_ology sometimes folds ROM EA_expr into 'SELF'
	PAIR Tuplo_ology sometimes stores STO EA_expr into 'typeof(SELF)'
	PAIR Tuplo_ology DIALECT STO EA_expr into 'typeof(SELF)'
	PAIR Tuplo_ology LEX STO EA_expr self self.type.get_EA()

	ARGV is a TUPLE not a Tuplo // dont you know
	CTXT + SCRIPT_POS + DATA // ARGV now a Tuplo
	// ARGV.class = "CLASS" ;
	// operator= ( STR0 ) { // lookup STR0 as Name
	// if( str == "CLASS" } {
	// Object * Obj = get_Obj_class( ... )
	// } else { ... }
	// }
	// SCRIPT_POS converts "ident99" or "expr" to LOCN/ITEM
	//

	TUPLO
	 PAIR
	 TRIP
	 QUAD
	 VECT

	SCRIPT can beam ITEM_CLASS = Loaded_Data_derived_type

	# If you need ABBR and BADE_Branch
	 TRIP Tupluo_TRES
	 QUAD Tuplo_QUAD

	TUPLO LHS RHS // CLASS = get_CLASS( CTXT, SELF, INTO )
	TUPLO LHS RHS // CLASS = LOCN_item LOCN ITEM // maybe deductable CACHE
	TUPLO LHS RHS // CLASS = LOCN_item LOCN ITEM CLASS_CT // strict typing
	TUPLO LHS RHS // CLASS = LOCN_item LOCN ITEM CLASS_RT // tag
	TUPLO LHS RHS // CLASS = get_CLASS( CTXT, SELF, INTO )

	TUPLO CLASS LHS RHS // CLASS = ARGV[0] // fold into type_of( Tuplo )
	TUPLO CLASS LHS RHS // CLASS = ARGV[1] // told as DATA // MOVE CSR++

	TUPLU CLASS	// too tricky to guess so EXACT ONLY // SCRIPTS SAYS
	TUPLU Class	// either by VTBL or INST or SCRIPT SAYS ...

	class TUPLO { ... }
	class Tuplo { ... }
	ASM FIELD STRUCT START // Name
	ASM	SPEC_start
	ASM	 META comes from a group with a basket of shapes
	ASM	 DOCS here is that basket, possibly ARGV[i] OBJV[i] N
	ASM	 struct Name { ... }
	ASM       field Name // flexible joint PTR -> LOCN
	ASM       field Type Name
	ASM       field Type Name Init_Value
	ASM       field Type Name setup_SCRIPT
	ASM       typedef Object * MEM_Object;
	ASM       typedef Object_OFFS u32_EXPR ; // DECODER //

	ASM FIELD STRUCT START // TUPLO
	ASM FIELD STRUCT EMPTY // OK NO DATA
	ASM FIELD STRUCT END

	ASM FIELD STRUCT START // Tuplo
	ASM FIELD u32_EA_SPEC = u32_hilo_abcd
	ASM FIELD STRUCT END

	ASM FIELD STRUCT IMPORT // CMNT
	ASM IMPORT // TUPLO
	ASM IMPORT // TUPLO_PAIR
	ASM IMPORT // TUPLO_TRIP
	ASM IMPORT // TUPLO_PAIR
	ASM IMPORT // TUPLO_ARGV
	ASM IMPORT // TUPLO_Tuplo // tag = lookup( STR0 ) // known by SCRIPT

	ASM IMPORT // Tuplo
	ASM IMPORT // Tuplo_PAIR
	ASM IMPORT // Tuplo_TRIP
	ASM IMPORT // Tuplo_ARGV
	ASM IMPORT // Tuplo_STREAM_CSR // CSR moves to NEXT // inline data

	ASM IMPORT // Object // SCRIPT Module //

	ASM FIELD Tuplo_Customised // gains all CLASS benefits
	ASM FIELD  OBJV[ 0 ] = VTBL_ea_expr // u32_es_expr ea = ADDR //

	ASM	KNOW MEM * CSR = SELF = tuplo.this; // obj.this // obj.ea
	ASM PKT frame START Object * P1 = yWObj.P0P2 .P0= =P;
	FSM  some_token = LOCN( ".P0=" )
	FSM  some_token is a known path through the FSM in one hop
	FSM  // P0P2.P0= P;
	FSM  // P0P2.P0 = P;
	FSM  // P0P2 .P0 = P;
	FSM  GOTO // P0P2 .P0 = P; // GOTO HERE // DOT
	ASM FIELD STRUCT START // Name
	ASM_FIELD u32_EA_SPEC = u32_hilo_abcd // OPTIONAL SWAP_abcd
	ASM FIELD STRUCT END

	ASM FIELD STRUCT END

	TUPLO Device `

	# If you jump to SCRIPTED SPEC Tuplo_ology
	# a basket of available PAIR morphologies
	# with-out NLP #
	# with-out SUBLEX #
	# with-out EXACT # MATCH module imported via this ENUM_VALUE
	# with-out CLASS = SCRIPTED_CLASS_node
	# with-out CLASS = PACKET_tag_CLASS_node_EA_expr; // TOKENISED (" X ")

	PSG_Tuplo

	 Typical Variable Names
	 	tuplo lhs rhs
		tuplo.type

	 Typical Data Specs
	 	Obj * objv[ N } = { const_data_in } // CHECK DATA

	#

#endf
