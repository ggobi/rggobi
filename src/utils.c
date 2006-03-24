#include "RUtils.h"
#include "RSGGobi.h"

#include "vars.h"

int R_IS(USER_OBJECT_ obj, const char * const name)
{
 int i, n;
 USER_OBJECT_ className;
 
 className = getAttrib(obj, R_ClassSymbol);
 n = GET_LENGTH(className);

  for(i=0;i<n;i++) {
    if(strcmp(name, CHAR(STRING_ELT(className, i))) == 0) {
#if LDEBUG      
printf("Is a %s\n", name);
PrintValue(obj);
#endif
       return(TRUE);
     }
  }
 return(FALSE); 
}  


SEXP
isSameRef(USER_OBJECT_ x, USER_OBJECT_ y)
{
   SEXP ans;
   void *a, *b;
   
   ans = NEW_LOGICAL(1);
   a = R_ExternalPtrAddr(x);
   b = R_ExternalPtrAddr(y);
   INTEGER_DATA(ans)[0] = (a == b);
   return(ans);
}

void
RS_throwError(char *msg)
{
  PROBLEM msg
  ERROR;
}

USER_OBJECT_
createFactor(USER_OBJECT_ vals, vartabled *vt, GGobiData *d, int which)
{
  USER_OBJECT_ labels, levels, ans, e;
  int i;

  PROTECT(levels = NEW_INTEGER(vt->nlevels));
  PROTECT(labels = NEW_CHARACTER(vt->nlevels));
  for(i = 0; i < vt->nlevels; i++) {
     INTEGER_DATA(levels)[i] = vt->level_values[i];
	 if (vt->level_names[i])
		 SET_STRING_ELT(labels, i, COPY_TO_USER_STRING(vt->level_names[i]));
  }

  PROTECT(e = allocVector(LANGSXP, 4));
  SETCAR(e, Rf_install("factor"));
  SETCAR(CDR(e), vals);
  SETCAR(CDR(CDR(e)), levels);
  SETCAR(CDR(CDR(CDR(e))), labels);

  ans = eval(e, R_GlobalEnv);

  UNPROTECT(3);
 
  return(ans);
}

