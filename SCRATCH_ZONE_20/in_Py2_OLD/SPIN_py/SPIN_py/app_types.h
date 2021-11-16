#ifndef APP_TYPES_H
#define APP_TYPES_H

#include "Python.h"

#define AppString	PyObject *
#define AppContentType	PyObject *
#define AppInt		PyObject *
#define AppStringList	PyObject *
#define AppAttr		PyObject *
#define AppAttrList	PyObject *
#define AppElemInfo	PyObject *
#define AppEnum		PyObject *
#define AppObject	PyObject *	// a bit loose
#define AppEntity	PyObject *
#define AppExternalDataEntityRef	PyObject *
#define AppExternalId	PyObject *
#define AppNotation	PyObject *
#define AppLocation	PyObject *
#define AppElem		PyObject *
//define AppElemInfo	PyObject *	// special subtype type

#endif
