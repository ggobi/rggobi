#include "RSGGobi.h"
#include "print.h"

PrintOptions *RSggobi_directPrintHandler(PrintOptions *options, displayd *dpy, ggobid *gg, GGobiPrintHandler *printHandler);
USER_OBJECT_ RSGggobi_convertPrintOptions(PrintOptions *opts);

USER_OBJECT_
RSggobi_setPrintHandler(USER_OBJECT_ func)
{
  extern void R_PreserveObject(SEXP);
  DefaultPrintHandler.callback = RSggobi_directPrintHandler;
  R_PreserveObject(func);
  DefaultPrintHandler.userData = (void *)func;

 return(NULL_USER_OBJECT);
}

PrintOptions *
RSggobi_directPrintHandler(PrintOptions *options, displayd *dpy, ggobid *gg, GGobiPrintHandler *printHandler)
{
  SEXP e, opts;
  
  PROTECT(e = allocVector(LANGSXP, 3));
  SETCAR(e, printHandler->userData);

  opts = RS_displayInstance(dpy);
  SETCAR(CDR(e), opts);

  opts = RSGggobi_convertPrintOptions(options);
  SETCAR(CDR(CDR(e)), opts);

  (void) eval(e, R_GlobalEnv);

  UNPROTECT(1);
 return(options);
}

USER_OBJECT_
RSGggobi_convertPrintOptions(PrintOptions *options)
{
  int numSlots = 0;
  USER_OBJECT_ ans, snames, tmp;

  char *names[] = {"dims", "fileName", "colors"};
  enum { PRINT_DIMS, PRINT_FILENAME, PRINT_COLORS};
  numSlots = sizeof(names)/sizeof(names[0]);
  PROTECT(ans = NEW_LIST(numSlots));
  PROTECT(snames = NEW_CHARACTER(numSlots));


  SET_VECTOR_ELT(ans, PRINT_DIMS, tmp = NEW_INTEGER(2));
   INTEGER_DATA(tmp)[0] = options->width;
   INTEGER_DATA(tmp)[1] = options->height;
  SET_STRING_ELT(snames, PRINT_DIMS, COPY_TO_USER_STRING(names[PRINT_DIMS]));


  SET_VECTOR_ELT(ans, PRINT_FILENAME, tmp = NEW_CHARACTER(1));
    SET_STRING_ELT(tmp, 0, COPY_TO_USER_STRING(options->file->fileName));   
  SET_STRING_ELT(snames, PRINT_DIMS, COPY_TO_USER_STRING(names[PRINT_FILENAME]));
  
  SET_NAMES(ans, snames);

  PROTECT(tmp = NEW_CHARACTER(1));
   SET_STRING_ELT(tmp, 0, COPY_TO_USER_STRING("ggobiPrintOptions"));
  SET_CLASS(ans, tmp);

  UNPROTECT(3);
  return(ans); 
}
