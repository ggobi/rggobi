/*
  Contains routines for invoking R/S functions
  and evaluating expressions.
 */

#include "RSGGobi.h"
#include "RSEval.h"


USER_OBJECT_ 
RS_GGOBI(callFunction)(USER_OBJECT_ fn, USER_OBJECT_ argList)
{
 SEXP e, ans;
  e = allocVector(LANGSXP, 2);
  SETCAR(e, fn);
  SETCAR(CDR(e), argList);

 PROTECT(e);
  ans = eval(e, R_GlobalEnv);
 UNPROTECT(1);

 return(ans);
}

USER_OBJECT_ 
RS_GGOBI(callUserFunction)(const char *name, USER_OBJECT_ args)
{
 USER_OBJECT_ ans, funcName, fn;

 PROTECT(args);

 funcName = install((char*) name);
 fn = findVar(funcName, R_GlobalEnv);
 if(!isFunction(fn)) {
    return(R_UnboundValue);
 }

 ans = RS_GGOBI(callFunction)(fn, args);

 UNPROTECT(1);

 return(ans);
}



USER_OBJECT_ 
RS_GGOBI(callFunctionWithArgs)(USER_OBJECT_ fn, USER_OBJECT_ argList)
{
 SEXP e, ans, tmp;
 int numArgs, i;

   numArgs = GET_LENGTH(argList);
  PROTECT(e = allocVector(LANGSXP, numArgs+1));
  SETCAR(e, fn);
  tmp = e;
  for(i = 0; i < numArgs; i++) {  
    SETCAR(CDR(tmp), VECTOR_ELT(argList, i));
    tmp = CDR(tmp);
  }

  ans = eval(e, R_GlobalEnv);
 UNPROTECT(1);

 return(ans);
}



USER_OBJECT_
RS_GGOBI(findFunction)(const char *name)
{
 SEXP funcName = install((char*) name);
 SEXP fn = findVar(funcName, R_GlobalEnv);
 if(!isFunction(fn)) {
    return(R_UnboundValue);
 }
 return(fn);
}
