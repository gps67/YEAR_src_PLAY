#ifndef EXPRS_H
#define EXPRS_H

#include "EXPRS_STUBS.h"

// we are generating this file
extern const char * str_of_token( int token );

// stuff included for TOKEN
#warning "PSG_Name hardcoded for EXPRS.h - gen_e1_yacc.xy but it is AFM"
#include "gen_e1_yacc.tab.hh"
//#include "gen_AFM_yacc.tab.hh"


#endif

