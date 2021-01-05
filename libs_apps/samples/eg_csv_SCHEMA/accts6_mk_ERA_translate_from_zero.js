2020-08-05 Aug Wed 10:55 accts6_mk_ERA_translate_from_zero.js

	ACCT
	 accts6
	  mk_ERA = EXPR start_date next_date // date of next ERA // [T0[T2
	  mk_ERA = ETA start_date #same_day // date of next ERA // #tomorrow
	  english_token_enum_month_names
	  dialect tables[table_name] = LIST {
	  	"ALIAS", // VOID
		"ZERO", "ONE", "TWO", ..., "UPTO", "ALIAS==LAST.name_as_ALIAS"
	  }
	  /*
	  	ITEM[0] = NULL; // helps Jan be 1
		 ITEM[1] = { "Jan" "January" "01" }
		 ...
		 ITEM[12] = { "Dec" "December" "12" }

		ITEM[0] = ONE_OF { NULL, VOID, ALIAS, ROM_item_with_exprs }
		 STR_UNSET "-unset-" += expect "SP -unset- SP" // DIALECT "LF"

		// table_name months_names_in_dialect
		table_name months
		_names_in_dialect
			token_t & plain_from_dialect( token ); // layers
			token_t & dialect_from_plain( token ); // layers
			// use_token as varname in_dialect( token ); // += POOL
			// token = current_value // "CLONE_OF_ITEM_N" // LAST
			// compile away item_ZERO add LIST[ N ] //
			// ZERO_means_ALIAS // or some setting

		ITEM_is_LEAF_in_TREE
			select LANG_1 == en.GB // repeat of SYS_english filter
			select LANG_2 == VAR current_value = translated_LANG2 {
			 ITEM[] = LIST { 
			 // keep skipping ZERO 
			  ITEM[0] = "%s" ITEM[M] // N counting [%d] from 1
			 // stop skipping ZERO 
			 // OMIT dont EMIT line "ITEM[M] // M = N + 1 //"
			   // OMIT "ITEM[0] = TEXT" ; TEXT = get_TEXT("ITEM[M]")
			   // EMIT PSG MATCH TABLE_M_NAMES
			   // NOTE "NAME[M]"
			   // NOTE "NAME[]"
			   // NOTE "NAMES[]"
			}

		Translation Table of specific VALUES SPELLING_DATA 

			LEX_TOKEN_t SPELLING = Tokenised( POOL, "ITEM" )
			// POOL can be a HINT byte 
			//  view_as_Jan _Jan _jan _January _january _01
			// POOL = ENUM_GROUP
			// POOL_COVER = cloned_layer
			// LAYER = idx_lang /* LAYER_from_idx_lang( LANG ) */
			// idx = u32_TOKEN_t;
			// lang = DATA_FIELD 

		MATCH ITEM[0] = "LAST_ITEM" // ITEM[idx+1] // [N]
		MATCH ITEM[%2.2d] = { "Abbr" "abbr" "Fullname" "fullname" "dd" }
	  */
