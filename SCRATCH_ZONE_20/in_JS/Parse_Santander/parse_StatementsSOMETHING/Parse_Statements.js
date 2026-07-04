#!/usr/bin/env nodejs 

// Parse_Statements.js // nodejs LIBR // Parse Santander //

const NULL = null

// https://nodejs.org/learn/manipulating-files/reading-files-with-nodejs
// FUNC on_COMPLETE
// SYNC
// PROM on COMPLETE
// STREAM promise got lines ... reparse over mid ridge

const fs = require('node:fs');
const file_name = 'StatementsSOMETHING.txt'
const file_cset = 'utf8'

class DATE_HOLDER {

 // The correct way to spell YEAR-MM-DD is sometimes YEAR_MM_DD
 // This simple code requires YOU PROVIDE MM and DD as "{ FMT %02d }" "01" 02
 // The date math is TODO get_time48 seconds since UNIX 1970 
 // The date math is TODO get_time48 seconds since YEAR_2000 i48_time_2000
 // The template overlay can be switched between clocks zero time to get Y2K0
 // i48_time_t _i48_time_one = BITFIELD_i48 held = i48_value << 16
 // i48_value = i64_MEM_WORD >> 16 // BITFIELD_MODULE does this

 constructor( YEAR, MM, DD ) {
   this.YEAR = YEAR	// "2026" from FMT "%d" // loose definitions history
   this.MM = MM		// "01" from FMT %02d // expect u4 
   this.DD = DD		// "01" from FMT %02d // expect u5 
 }

 get_YEAR_MM_DD() { // use this for everywhere DATE is eg filenames
	var YEAR_MM_DD = this.YEAR + "-" + this.MM + "-" + this.DD; 
 	return YEAR_MM_DD
 }

// discourage dd_mm_year // but parse it and regen it // GEN_PARSE
 get_dd_mm_year_str( date_1 ) {
 	// RELIC // recognise DIALECT 
	var DD_MM_YEAR = this.DD + "/" + this.MM + "/" + this.YEAR; 
	return DD_MM_YEAR // hint to caller what to call AVAR and SPEC_NAME
 }

}; // DATE_HOLDER CORE with added RELIC

// outside of DATE_HOLDER is a wrap that CALLS new DATE_HOLDER(YEAR,MM,DD)
function DATE_HOLDER_from_dd_mm_year( dd_mm_year ) { // "26/05/2026" // Parse
	var ptn = /(?<DD>[0-9]{2})\/(?<MM>[0-9]{2})\/(?<YEAR>[0-9]{4})/
	let match = dd_mm_year.match(ptn)
	if(match) {
	//	console.log(ptn);
	//	console.log("MATCH");
	//	console.log(match);
		var date_held = new DATE_HOLDER(
			match.groups.YEAR,
			match.groups.MM,
			match.groups.DD
		)
	//	console.log(date_held);
		return date_held;
	} else {
		return null
		// be silent
		// parser likes to call looking for first match
		// let next ptn look at line
		console.log("did not match");
		console.log(ptn);
		return null
	}
}

function YEAR_MM_DD_from_dd_mm_year( dd_mm_year ) { // "31/12/1999"
	date_held = DATE_HOLDER_from_dd_mm_year( dd_mm_year )
	// null when ptn didnt match, following will fail 
	return date_held.get_YEAR_MM_DD()
}

function match_From_to( line ) {

	var ptn = /From:.(?<LHS>[0-9/]+).to.(?<RHS>[0-9/]+)/
	// the dots are A0 which is changing number to FD // I cant see how !
	// same for dots in other ptns // fragile hand crafted
	// and I certainly cant get the RE to work with utf8 or something
	// probably the same as 0xA0 becoming 0xFD // mc view_hex says A0
	// nodejs has loaded the file somehow so that they appear as 0xFD
	// CHAR 253 'ý' 0xFD \375
	//	return null
	let match = line.match(ptn)
	if(match) {
// label From line # CHAR 70 'F' 
// label From line # CHAR 114 'r' 
// label From line # CHAR 111 'o' 
// label From line # CHAR 109 'm' 
// label From line # CHAR 58 ':' 
// label From line # CHAR 253 'ý' <-- SEE ! impossible
// label From line # CHAR 48 '0' 
// label From line # CHAR 49 '1' 
// label From line # CHAR 47 '/' 
// 

	 if(0) {
	  if(1)	dump_str("From line", line )
		console.log(ptn);
		console.log("MATCH");
		console.log(match);
// console.log(match)
//	var got_date_0 = YEAR_MM_DD_from_dd_mm_year( match.groups.LHS );
//	var got_date_1 = YEAR_MM_DD_from_dd_mm_year( match.groups.RHS );
	 }
	} else {

	// NO MATCH // normal behaviour is fast silent "{ return null }"
	//	return null
	// BUT if line1 is not From: this script says NO_MATCH
	// So make some debug noise

		console.log("did not match { from_upto not found }");
		console.log(ptn);
		console.log(line);
	
	// then return silent null "NULL means FALSE means NO_MATCH"
		return null
	}

	// still here prepare return data
	// slightly optimistic with MATCH being correct
	// console.log(match.groups.LHS);
	// console.log(match.groups.RHS);
	var got_date_0 = YEAR_MM_DD_from_dd_mm_year( match.groups.LHS );
	var got_date_1 = YEAR_MM_DD_from_dd_mm_year( match.groups.RHS );
	//
	// RET_VAL
	//
	return [ got_date_0, got_date_1 ]
}

/*
	TELL API
	console.log("Account " + Account + " ACCOUNT '" + ACCOUNT + "'" )
	TELL API
	"Account " + Account 
	"ACCOUNT '" + ACCOUNT + "'" 
	TELL API
	"ACCOUNT '${ACCOUNT}'"

		because of $AVAR asking its TEXT to be printed for FILTER Q1
	
		   Q0 FILTER  # ARGV_SCRIPT # Parse local keywords i8_idx 
		'{ Q1_FILTER }'
		"{ Q2_FILTER }"
*/


function match_Account( line ) {
	var ptn = /Account:.(?<ACCOUNT>[0-9 X-]+)/
	let match = line.match(ptn)
	if(match) {
	 // GEN // debug TRACE AVAR TEXT // Bench RE_EXPR
	 if(0) {
		console.log(ptn);
		console.log("MATCH");
		console.log(match);
	 }
	} else {
		return null
		console.log("did not match Account");
	// GEN // TRACE AVAR // REPORT NO MATCH // so retry next // see null
		console.log(ptn);
		console.log("'"+line+"'");
		return null
	}
	var ACCOUNT = match.groups.ACCOUNT
	ACCOUNT = ACCOUNT.replaceAll(' ',"_")
	return ACCOUNT 
}

function match_Date( line ) {
	var ptn = /Date:.(?<Date>[0-9/]+)/
	let match = line.match(ptn)
	if(match) {
	 // GEN // debug TRACE AVAR TEXT // Bench RE_EXPR
	 if(0) {
		console.log(ptn);
		console.log("MATCH");
		console.log(match);
	 }
	} else {
		return null
		console.log("did not match Date");
	// GEN // TRACE AVAR // REPORT NO MATCH // so retry next // see null
		console.log(ptn);
		console.log("'"+line+"'");
		return null
	}
	var dd_mm_year = match.groups.Date // named optimistically
	var dated = DATE_HOLDER_from_dd_mm_year( dd_mm_year );
	if(!dated) {
		console.log("# FAIL # failed");
		return null
	}
	return dated
}

function match_Description( line ) {
	var ptn = /Description:.(?<Description>.*)/
	let match = line.match(ptn)
	if(match) {
	 if(0) {
		console.log(ptn);
		console.log("MATCH");
		console.log(match);
	 }
	} else {
		return null
		console.log("did not match Description");
		console.log(ptn);
		return null
	}
	return match.groups.Description 
}

function match_Amount( line ) {
	var ptn = /Amount:.(?<AMOUNT>[0-9]+\.[0-9]{2})./
	let match = line.match(ptn)
	if(match) {
	 if(0) {
		console.log(ptn);
		console.log("MATCH");
		console.log(match);
	 }
	} else {
		return null
		console.log("did not match Amount");
		console.log(ptn);
		return null
	}
	return match.groups.AMOUNT 
	// return match.groups.AMOUNT 
}

function match_Balance( line ) {
	var ptn = /Balance:.(?<Balance>[0-9]+\.[0-9]{2})./
	let match = line.match(ptn)
	if(match) {
	 if(0) {
		console.log(ptn);
		console.log("MATCH");
		console.log(match);
	 }
	} else {
		return null
		console.log("did not match Balance");
		console.log(ptn);
		return null
	}
	return match.groups.Balance 
	return "40.21"
	// return match.groups.AMOUNT 
}

function match_SIX_TAB_GAP_LINE(line){
	return line == "						"
}

function dump_str( label, latin1String ) {
	// const latin1String = "café"; // Contains Latin-1 character é
	const binaryString = latin1String.split('').map(char => {
		var TEXT = "# label " + label + " # "
		var C1 = String.fromCharCode(char.charCodeAt(0) & 0xFF);
 //		TEXT += "# grow_text # \n";
		TEXT += "CHAR " + ( char.charCodeAt(0) & 0xFF ) +" '"+ C1 + "' \n";
 // NaN		TEXT += "CHAR.1 " + ( char.charCodeAt(1)        ) +" '"+ C1 + "' \n";
 // see hidden fields 	TEXT += "char " + ( char.prototype ) +" '"+ C1 + "' \n";
 //		TEXT += "CHAR " + ( char.charCodeAt(0)        ) + " no 0xFF \n";
		return TEXT
	}).join('|');

	console.log("{"+binaryString+"}") // want drop trail nl .log adds 1
// 2. Encode the binary string into a UTF-8 Uint8Array
	return binaryString
}

function UTF8_from_Latin1( latin1String ) {
	// const latin1String = "café"; // Contains Latin-1 character é
	const binaryString = latin1String.split('').map(char => {
		return String.fromCharCode(char.charCodeAt(0) & 0xFF);
	}).join('');

// 2. Encode the binary string into a UTF-8 Uint8Array
	const utf8Bytes = new TextEncoder().encode(binaryString);

	return utf8Bytes;
}

function func_header_holding_stray_A0_junk( file_name, file_cset, file_data ) {
//  console.log(file_data);
//  console.log("LOGGED");

	A0_utf8 = "\302\240"
	A0 = A0_utf8 

	// CHAR 160 ' ' 0xA0 \240 // NBSP (using SP)
	// CHAR 189 '½' 0xBD \275
	// CHAR 195 'Ã' 0xC3 \303
	// CHAR 253 'ý' 0xFD \375

	// convert at A0 to SP // not worth the emulation multi SP
	const file_data_NBSP = file_data 
  //	var A0_utf8_str = String.fromCharCode(0xA0)
  	var A0_utf8_str = String.fromCharCode(0xFD) ;// single char value as str
	// ACTUALLY I think A0_utf8_str is already a utf8 String
	// POSSIBLY A0_utf8 is only BYTE oriented, so effectively Latin1

//	A0 = A0_utf8
	A0 = A0_utf8_str 
	dump_str("A0_utf8_str", A0_utf8_str)
	// A0 = UTF8_from_Latin1( A0_utf8_str ) // double encoding
	console.log("A0_utf8_str == '" + A0_utf8_str + "'")
	console.log("A0          == '" + A0 + "'")
	console.log("240         == '\240'")
	console.log("240 240     == '\240\240'")
	dump_str("HELLO", "hello")
	console.log(A0_utf8_str) ;// ý //
	console.log(A0) // Uint8Array(2) [ 195, 189 ] //
	dump_str("A0", A0)

	// daft set A0 to "0" and see NBSP everywhere
	// test visible in date // A0 = "0"

	var RE_A0 = new RegExp(A0,"g") // RE from pattern STRING, flags+=g


	if( RE_A0.global == true ) {
	// as it should be because
	// RegExp flag "g" set RE_A0.global == true
	// TESTED without "g" got RE_A0.global == false
	//	console.log( "# RE_A0.global = " + RE_A0.global +" #" )
	} else {
		console.log( "# RE_A0.global = " + RE_A0.global +" # ODD" )
	}
	// daft // RE_A0 = "0"
//	const file_data_NO_NBSP = file_data_NBSP.replaceAll(/\060/g, " ZERO ")
//	const file_data_NO_NBSP = file_data_NBSP.replaceAll(/\240/g, " NBSP ")
	const file_data_NO_NBSP = file_data_NBSP.replaceAll(RE_A0, " (NBSP)")
	file_data = file_data_NO_NBSP
//	file_data = "From: 01/01/2020 to 26/05/2026" // works fine
	// this slightly longer chain of _NO_NBSP forms zero cost moments STEPS
} // JUNK FUNC holding A0 code

function got_file_copy( file_name, file_cset, file_data ) {
/*
	After the 10K file was split into 1K bricks and sent over LAN
	it has now arrived, transport says checksum was good, maybe
	all bricks present in a row, file probably 15K or 2K occasionally 1M
	 
	promise deliver PROVIDE file_copy = file_data
		options of an outside file and internal filename, and fd
*/

	// now split into lines
	var lines = file_data.split("\n")

/*
	HERE A0_0A fell flat on its face
	and failed to match the A0 in the text
	PROBABLY because UTF8(0xA0) is a pair of bytes
	ARGUABLY C should have converted "\240" to A0_utf8_str and A0_as_UTF8 _XX_YY

	BANK CSV TEXT is exactly A0 - they think of it as NBSP 
	NBSP Non Breaking Space A0 

	SCRIPT is in UTF8 mode, but "\240" is actually BYTE_8859_Latin_A0_NBSP
	Since we know this we can call BYTE_Latin1_to_UTF8 as UTF8_from_BYTE_Latin1
	Just this once 

	echo "\240" | UTF8_from_Latin1 | od -hx
	A0 == 'AO{194,160}'
	194 == \302 // UTF8 BYTE1 and 6 bits // using lower 2
	160 == \240 // A0 happens to be own tail

	A 1010 
	A0 1010 0000
	A0 10 100000
	i12_UTF8_PAIR
	   000010   100000
	 11------ 10------
	 11000010 10100000
	    C   2    A   0

	    C2 == \302
	    A0 == \240


	JUST found this

  var A0 = String.fromCharCode(0xA0)

// 2. Encode the binary string into a UTF-8 Uint8Array
const utf8Bytes = new TextEncoder().encode(binaryString);

console.log(utf8Bytes);



	// A0 = 160 = 8 * 20 = \240
	// var A0 = "\240" // failed because latin1 nomatch for UTF8 byte pair
  	var A0 = String.fromCharCode(0xA0) 
	A0_utf8 = "\302\240"
	A0 = A0_utf8 
//	wrapped_A0 = "{" + A0 + "}"
//	console.log(wrapped_A0)
*/

	var INFO = {}

	var KEPT = {}
	KEPT.INFO = INFO
	KEPT.LIST = [ "d", "e" ]
	KEPT.LIST = [  ]

	KEPT.INFO.file_name = file_name
	KEPT.INFO.file_cset = file_cset
	// file_data

	var pair = match_From_to( lines[0] ) 
	if( pair ) {
		var [ lhs, rhs ] = pair
	//	console.log( "date_upto " + rhs )
	//	console.log( "date_from " + lhs )
		KEPT.INFO.date_upto = rhs
		KEPT.INFO.date_from = lhs
	// printout order is creation order
	} else {
		return null
	}

	var acct = match_Account( lines[2] ) 
	if( acct ) {
		acct = acct.replaceAll(' ',"-") // SPELLING ADJUST
	//	console.log( "Account ALIAS " + acct )
		KEPT.INFO.Account = acct // accept SPELLING ADJUST early
	} else {
		return null
	}

	var LPOS = 4
	var LPOS_P2 = lines.length
	var ITEM = {}
	while( LPOS < LPOS_P2 ) { // WANT foreach starting at START_POS P1
	// Iterate over lines
	  var line = lines[LPOS++]

	  // Parse Line
	  // try various patterns // if RET is null dont disturb that ATTR
	  // try each until match found and continue to next line

	  if(match_SIX_TAB_GAP_LINE(line)){
	   // console.log("# match_SIX_TAB_GAP_LINE(line) " + LPOS)
	   check_ITEM( KEPT, ITEM )  // copies the fields over 
//	   if( ITEM = check_ITEM( KEPT, ITEM ) ) {} // copies the fields over 
	   // console.log(ITEM)
	   continue	
	  } // SIX_TAB // entire line //

	  if( RET = match_Date( line ) ) {
	   ITEM.Date = RET
	   continue;
	  }
	  if( RET = match_Description( line ) ) {
	   ITEM.Description = RET
	   continue;
	  }
	  if( RET = match_Amount( line ) ) {
	   ITEM.Amount = RET
	   continue;
	  }
	  if( RET = match_Balance( line ) ) {
	   ITEM.Balance = RET
	   continue;
	  }
	  
	  console.log("line not matched: " + line )

/*
Date:.07/04/2026
Description:.BANK GIRO CREDIT REF D012K00A08VVNSIB, IB CA371700
Amount:.25.56.
Balance:.199.81.
*/

	}
//	console.log("EOF so extra check_ITEM");
	check_ITEM( KEPT, ITEM )

 if(0) {
	console.log("--------------- KEPT.INFO ------------");
	console.log(KEPT.INFO);
	console.log("--------------- KEPT.LIST ------------");
	console.log(KEPT.LIST);
 }
//	KEPT.LIST.forEach( (ITEM) => {
//	 console.log(ITEM);
//	} )
	return KEPT
	return KEPT.LIST
}

function check_ITEM_completely_empty( ITEM ) {
	   if(ITEM.Date) { return false; }
	   if(ITEM.Description) { return false; }
	   if(ITEM.Amount) { return false; }
	   if(ITEM.Balance) { return false; }
	   return true; // yes all are empty
}

function check_ITEM( KEPT, ITEM ) {
	   var RET_VAL = {}
	   if(ITEM.Date) {
	   //	console.log("copy Date " + ITEM.Date)
	   	RET_VAL.Date = ITEM.Date
	   } else {
		// allow all to be absent // return null say nothing
		if(check_ITEM_completely_empty(ITEM)) {
			return null; // quick silent no item present
		} // else stay and report stuff
		// Absent Date field
	   	console.log("ITEM has no Date field");
		return null
	   }
	   if(ITEM.Description) {
	   //	console.log("copy Description " + ITEM.Description)
	   	RET_VAL.Description = ITEM.Description
	   } else {
		// should throw, as null also means EOF
	   	console.log("ITEM has no Description field");
		return null
	   }
	   if(ITEM.Amount) {
	   	RET_VAL.Amount = ITEM.Amount
	   //	console.log("copy Amount " + ITEM.Amount)
	   } else {
	   	console.log("ITEM has no Amount field");
		return null
	   }
	   if(ITEM.Balance) {
	   // This is where Balance is lost
	   // Its retained for the next step parser to parse
	   // Its redacted for privacy cloak
	   // Dont even show old value, because that isnt private
	   //	console.log("copy Balance " + ITEM.Balance)
	   // Dont copy field value over, substitute a BLANK placebo here
	   //   RET_VAL.Balance = ITEM.Balance
	   	RET_VAL.Balance = "43.21"
	   //	console.log("NOTICE RET_VAL Balance " + RET_VAL.Balance)
	   } else {
	   	console.log("ITEM has no Balance field");
		return null
	   }

	   // console.log("LIST adds RET_VAL" );
	   // KEPT.LIST += RET_VAL // not array friendly
	   KEPT.LIST.push(RET_VAL) // append item to tail of array returns N
	   return RET_VAL
}

function got_file_tree( KEPT ) {
	console.log(KEPT.INFO)
	KEPT.LIST.forEach( (ITEM) => {
	 // console.log("ITEM");
	 // console.log(ITEM);
	 var LINE = ITEM.Date.get_YEAR_MM_DD() + " " + ITEM.Amount + " " + ITEM.Description 
	 console.log(LINE);
	} )
}

fs.readFile( file_name, file_cset, (err, file_data) => {
  // SCRIPT // SCOPE += ARGV_as_C_NAMES_as_cached_lookup_NAME_and_EXPR
  // EXPR // ASIF FILTER // PRE_CALL is the usual culprit also MID_CALL 
  // EXPR // ASIF EXPR // LIBR OPERATORS VANILLA FILTER CONVEY QUOTA N_threads
  // FLEX // AUTO INVOKE ASIF EXPR // SCOPE += LIBR_GLOBALS
  // EA_EXPR // can be used to hold a ref, or request ROM COPY of XPOS_and_TIME
  // WHEN // Second + 127/_i8 // QUOTA on LIBR can use lots of local heaps


  if (err) {
    console.error(err);
    return nul;
  }
  // console.log(file_data);
  // SCRIPT //
  
  // ALIAS is_more_recent // typical 1 < 2
  // CMP $1 < $2 
  // CMP LHS < RHS 
  // T = B - A
  // on_PLUS B > A
  // on_ZERO B == A
  // on_MINUS B < A // NOT A < B // local EMPH
  var KEPT = got_file_copy( file_name, file_cset, file_data )
//  console.log("---------- KEPT ---------")
//  console.log(KEPT)
  var KEPT_2 = got_file_tree( KEPT )
  return KEPT
});
