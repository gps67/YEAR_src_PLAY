#ifndef SP_PEG_KIT_H
#define SP_PEG_KIT_H

#include "ParserEventGeneratorKit.h"

// #include "app_types.h"
#include "PY_Object_base.h"

struct SP_peg_kit : public PY_Object_base
{
	ParserEventGeneratorKit * kit; 

	SP_peg_kit();
	~SP_peg_kit();

};

/* EXPORTED */

PyObject * new_SP_peg_kit( PyObject *, PyObject * args );

SP_peg_kit * as_SP_peg_kit( PyObject * obj ); // identity OR error

#endif
