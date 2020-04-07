#include <stdio.h>

// #name is name as a string "name"
#define P( name ) printf( "%7.4f %s\n", name, #name );
#define NL() printf("\n");

int main()
{
	float have_virus_test_correct = 0.94;
	float dont_have_virus_test_correct = 0.96;
	float have_virus = 0.03;
//	float have_virus = 0.70;

	float      have_virus_test_wrong = 1.0 -      have_virus_test_correct;
	float dont_have_virus_test_wrong = 1.0 - dont_have_virus_test_correct;
	float dont_have_virus = 1.0 - have_virus;

	float test_says_have
	=      have_virus *      have_virus_test_correct
	+ dont_have_virus * dont_have_virus_test_wrong;

	float test_says_dont_have
	= dont_have_virus * dont_have_virus_test_correct
	+      have_virus *      have_virus_test_wrong;


	P( have_virus_test_correct );
	P( have_virus_test_wrong );
	NL();
	P( dont_have_virus_test_correct );
	P( dont_have_virus_test_wrong );
	NL();
	P( have_virus );
	P( dont_have_virus );
	NL();

	P( test_says_have );
	P( test_says_dont_have );

	return 0;

}
