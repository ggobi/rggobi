#include "RSGGobi.h"

#include <gtk/gtk.h>

#include "vars.h"

#include "R.h"
#include "Rinternals.h"

#include "RSEval.h"

/*
IdentifyProc RS_INTERNAL_GGOBI(identifyHandler);
*/

void
RS_INTERNAL_GGOBI(identifyHandler)(void *user_data, gint k, splotd *sp, GtkWidget *w, ggobid *gg);

USER_OBJECT_
RS_GGOBI(setIdentifyHandler)(USER_OBJECT_ func, USER_OBJECT_ ggobiId)
{
  ggobid *gg = toGGobi(ggobiId);
  USER_OBJECT_ ans = NULL_USER_OBJECT;
  g_return_val_if_fail(GGOBI_IS_GGOBI(gg), NULL_USER_OBJECT);
  

  if(gg == NULL) {
      PROBLEM "Invalid ggobi identifier specified."
      ERROR;
  }

#ifdef EXPLICIT_IDENTIFY_HANDLER
  if(gg->identify_handler.handler == &RS_INTERNAL_GGOBI(identifyHandler)) {
    ans = (USER_OBJECT_) gg->identify_handler.user_data;
    R_ReleaseObject(ans);
  }

  if(GET_LENGTH(func)) { 
    /* Make certain it knows to call us. */
    R_PreserveObject(func);
    GGOBI(setIdentifyHandler)(RS_INTERNAL_GGOBI(identifyHandler), func, gg);
  } else {
    GGOBI(setIdentifyHandler)(NULL, NULL, gg);
  }
#endif

  return(ans); 
}

/*
   This should pass the following values to the function:
    1)  the observation index
    2)  the plot index
    3)  display
    4)  the dataset.
 */
void
RS_INTERNAL_GGOBI(identifyHandler)(void *user_data, gint k, splotd *sp, GtkWidget *w, ggobid *gg)
{

  USER_OBJECT_ func = (USER_OBJECT_) user_data;
  USER_OBJECT_ e;
  USER_OBJECT_ tmp;

  PROTECT(e = allocVector(LANGSXP, 3));

  SETCAR(e, func);
  SETCAR(CDR(e), tmp = NEW_INTEGER(1));
  INTEGER_DATA(tmp)[0] = k;

  SETCAR(CDR(CDR(e)), RS_displayInstance(sp->displayptr));

  eval(e, R_GlobalEnv); 

  UNPROTECT(1);
}

