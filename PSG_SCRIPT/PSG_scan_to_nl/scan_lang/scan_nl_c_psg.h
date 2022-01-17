
#include "scan_nl_c_lex.h"
/*!
*/
class scan_nl_C_PSG : public scan_nl_C_LEX 
{
 public:
	scan_nl_C_PSG();
	scan_nl_C_PSG( p0p2 zone );

	bool scan_typeexpr();

	bool builtin_type_name( str1 & name );
	bool known_type_name( str1 & name );
	bool type_expr_left( str1 & expr );

	// these should return what they find ? or send to STBL ?
	bool var_decl();
	bool cpp_hash_line(); //#define #if #endif ...
	bool scan_pp_include_( str1 & filename, bool & local );

	bool scan_file( const u8 * filename ); // p0p2 already set // name helps

};

