
xzcat mozjs-112.0.0.tar.xz  \
| tar -xvf - -C /home/gps/YEAR/src_build/ \
2>&1 \
| tee $0.log


exit

LANGS
	SHELL PIPE ARGV 
	 QUOTE
	 GEN
	 RULES
	
	JS API # API_with_OBJ eg gdb_py_macros_for_GEN
	 OBJ = STRUCT OBT_t { FIELDS * } // MODULE of itself // IMPORT_EXPORT
	  GET OBJ.field1
	  SET OBJ.field2 ${RHS} // RHS = TUPLO CODE_POINT { SET_VAL_t SET_VAL }

	  SET OBJ.field2 ${RHS} // RHS =
	   TUPLO
	    CODE_POINT
	    { SET_VAL_t SET_VAL }
	    // DATA = CODE_POINT SET_VAL_expr // EXPR = SET_VAL // "%s" //

	    // NB that is the magic create-by-mentioning %s formula / it == "%s"
	    // SET_VAL == "SET_VAL"
	    // AUTO_TYPE = %s_t // reduce via actual obstructing { use_instead "%s" }
	    // AUTO_TYPE = %s // no Q2 required for this DIALECT WORD == "%s"
	    // MATCH DETECT // { NO Q2 } // NO_Q2_USED // BIND AUTO to MANUAL 
	    // ALIAS %s //
	    // LEX TOKEN // SET_VAL // ALIAS DIALECT 
	    // LEX_TOKEN "SET_VAL" //
	    // LEX_TOKEN SET_VAL { SET_VAL VAL } //
	    // ARG1 == { SET VAL } // CODE_POINT //
	    // CODE_POINT /* HERE */ "SET_VAL { SET VAL }" // NAME_TEXT_PARSED
	    // XPOS NAME_TEXT_PARSED //
	    //  NAME "NAME"
	    //  TEXT "TEXT"
	    //  VAL "VAL"
	    // BIND get_NAME // SPELLING %s // ANYSTR // WORD_KEY // PARSED_ALIAS //
	    // ALIAS = lookup ALIAS_EXPR get "ALIAS" // multi_field_GETTER
	    // CT // lookup == "get_alias ... " // get { ALIAS == "ALIAS" }  // 
	    // RT // if MATCH REXPR_PSG_SUBLEX_ITEM 
	    // RT // if MATCH CONDITION in VAR99 == "DETECT" code
	    // CT RT // VTBL funcs for BASE_CLASS // and use static C funcs CXX
	    // CXX_name_mangled // soz // ALIAS filter = UDEF //
	    // CXX_name_decode { LIST_of_demangeled }
	    // LIST_of_namespaces_and_class_and_funcs_and_argtypes { SCRIPT DATA}
	    // STO STR "%s"
	    // NAME STR // "%s" //
	    // "{ DATA %s }" // PARSER matches EXACT_STR_CMP // FILTER dialect
	    // { VAR_NAME "%s" } { VAR_SPEC_idx "%s" } { VAR_DATA %s } //
	    // { VAR_NAME "%s" } { VAR_DATA %s } { VAR_SPEC_idx "%s" } // IDX %s


	TEMPLATE DIAG

	 MK_SPEC DECL_LIST
	 MK_VAR DECL_LIST
	 mk_API DECL_TREE // by_KEY // by_IDX // by CT_RT // by RT_CT //
	    //
	    // LEX TOKEN // SET_VAL // ALIAS DIALECT "SET_VAL" SET_VAL { SET_VAL VAL } // { SET VAL } // CODE_POINT == "SET_VAL { SET VAL }" // DECL SOURCES
	    //
