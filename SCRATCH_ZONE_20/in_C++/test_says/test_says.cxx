#include <stdio.h>

// #name is name as a string "name"
#define P( name ) printf( "%7.3f %s\n", name, #name );
#define NL() printf("\n");

int main()
{
	float      have_virus_test_correct = 0.94;
	float dont_have_virus_test_correct = 0.96;
	// or make them equal
	           have_virus_test_correct = 
	      dont_have_virus_test_correct = 0.95;
	// 95% claim, must be manufactered using <RULES> ? 50% 50%
	//  5% error, 5% of LHS, 5% of RHS,
	// ( LHS, RHS ) ( 4, 96 ) ( 70, 30 ) ( 1, 99 ) { uninfected, infected }
	// { False, True }
	//
	// Distribution curve, mostly bell shaped
	// VAL is within R2 of AVG // presumes AVG is representative // tis
	// P( (VAL-AVG) += 95% accurate // area under curve // one-sided // 5%
	// test works by concentration of particle triggering dye // no //
	// one variance from noise due to stray chemicals, production, batch
	// one variance from noise due to sampling air contaminated
	// BOOL detected = false; // certainty == NULL 
	// ( LHS, RHS ) = i( float LHS_VAL = 50; float RHS_VAL = 100 - LHS_VAL )
	// 
	float have_virus = 0.03;
//	float have_virus = 0.70;
	      have_virus = 0.20; // 20 % coverage
	      have_virus = 0.05; // 05 % infected
	      have_virus = 0.01; //    % infected

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
	P(      have_virus );
	P( dont_have_virus );
	NL();

	P( test_says_have );
	P( test_says_dont_have );

	return 0;

}
