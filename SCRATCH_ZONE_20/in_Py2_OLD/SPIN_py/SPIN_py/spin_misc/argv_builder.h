#ifndef ARGV_BUILDER
#define ARGV_BUILDER

#include "Python.h"	

class argv_builder {
 public:
	argv_builder(PyObject * obj1);
	~argv_builder();
	int argc;
	char ** argv;
	PyObject * holder;
};

/*
	argv_builder is a utility to get round a hassle
	it doesn't build argv, it converts from Py list of string
	it requires that the list+strings don't change
*/

#endif
