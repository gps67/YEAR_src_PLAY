// #include "e1.tab.hh"

extern // "C"
int yyparse();

int main( int argc, char ** argv )
{
	yyparse();
	return 0;
}
