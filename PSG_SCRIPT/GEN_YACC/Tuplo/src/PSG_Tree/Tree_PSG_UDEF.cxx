
#include "Tree_PSG_UDEF.h"
#include "Tree_PSG_LEX_PUNCT_NAME.h"


// SEE: RUN_at_CT // to get PREBUILT machine at init_val_one
// SEE: STO_fold_away // elf_segment expands to .bss or MMAP_fd
// MMAP_fd (is usually /dev/zero to get it to give up solo-owned clean pages)
// MMAP_fd = API_LINK VFS simulating ROM_FILE init_1 data then
// MMAP_fd = API_LINK PKT simulating ROM_FILE_DATA_XFER in API_STREAM
// SERVER delivers REQ_VER_WHEN DIR/FILE/DATA + META_DATA
// SERVER filters requests through CHECKER_FILTER
// RELAY filters are either inside or outside, or vetted

// Tree_PSG_UDEF is a BASE CLASS with OPTS eg C_EXPR


bool Tree_PSG_UDEF:: build_tree() {
	if(! build_tree_lex() ) return FAIL_FAILED(); 
	if(! build_tree_yacc() ) return FAIL_FAILED(); 
	return true;
}

bool Tree_PSG_UDEF:: build_tree_lex() {
	return FAIL("UNWRITTEN");
	return true;
}
bool Tree_PSG_UDEF:: build_tree_yacc() {
	return FAIL("UNWRITTEN");
#if 0
	Tree_PSG_RULE_SEQ * SEQ = new Tree_PSG_RULE_SEQ( "expr_ident" );
	SEQ -> name = "expr_ident";
	SEQ -> add_step_LEX( "WS" );
	SEQ -> add_step_RULE( "expr_ident" );
	SEQ -> set_code(" $$ = $2; ");
#endif
	// bool add_yacc_for_C_EXPR();
	return true;
}
