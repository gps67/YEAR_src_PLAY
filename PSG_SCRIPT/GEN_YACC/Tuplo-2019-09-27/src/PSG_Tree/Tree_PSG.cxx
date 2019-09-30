
#include "Tree_PSG.h"

Tree_PSG::
Tree_PSG()
{
	POOL_PUNCT.PFX = "PUNCT"; // OK omit trailing _
	POOL_RW   .PFX = "RW";   // OK omit trailing _
	POOL_LEX  .PFX = "LEX"; // OK omit trailing _
}

Tree_PSG::
~ Tree_PSG()
{
}

