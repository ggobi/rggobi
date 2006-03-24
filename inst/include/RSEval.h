#ifndef RS_GGOBI_EVAL_H
#define RS_GGOBI_EVAL_H

#include "Rinternals.h"

USER_OBJECT_  RS_GGOBI(callFunction)(USER_OBJECT_ fn, USER_OBJECT_ argList);
USER_OBJECT_  RS_GGOBI(callUserFunction)(const char *name, USER_OBJECT_ args);
USER_OBJECT_  RS_GGOBI(findFunction)(const char *funcName);
USER_OBJECT_  RS_GGOBI(callFunctionWithArgs)(USER_OBJECT_ fn, USER_OBJECT_ argList);
#endif
