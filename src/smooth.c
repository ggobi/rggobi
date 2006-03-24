/*
  Provides a mechanism for invoking an R-level
  function for performing the smoothing of a pair of variables
  and returning a single vector of numbers for the smoothed y.
  The function can be specified programmatically, allowing
  the user to experiment with different implementations.

  The function is called with thre arguments - x, y and 
  window width and is expected to return a Numeric vector
  with the same length as each of x and y.
  
 */
#include "RSGGobi.h"
#include "RSEval.h"

#include <gtk/gtk.h>

#include "vars.h"

USER_OBJECT_ RS_smoothFunction = NULL;

USER_OBJECT_ RS_GGOBI(variableToRS)(int index, ggobid *gg);


/**
   Computes the smoothed values for the y variable
   for the bivariate X and Y identified by column
   index for the data set in ggobi.
   This calls the R/S function currently registered
   in the variable RS_smoothFunction with three arguments:
     the numeric vectors x & y
     the desired window width as specified by the 
     the argument `width'.
 */
double *
RS_GGOBI(smooth)(int x_index, int y_index, double width, ggobid *gg)
{
  double *values;
  USER_OBJECT_  vals, tmp;
  USER_OBJECT_ e;

  if(RS_smoothFunction == NULL || RS_smoothFunction == R_UnboundValue)
    return(NULL);

     e = allocVector(LANGSXP, 4);
     PROTECT(e);
     SETCAR(e, RS_smoothFunction);
     SETCAR(CDR(e), RS_GGOBI(variableToRS)(x_index, gg));
     SETCAR(CDR(CDR(e)), RS_GGOBI(variableToRS)(y_index, gg));
     tmp = NEW_NUMERIC(1);
     NUMERIC_DATA(tmp)[0] = width;
     SETCAR(CDR(CDR(CDR(e))), tmp);

     vals = eval(e, R_GlobalEnv);


   PROTECT(vals);
   /*   PrintValue(vals); */
   values = asCArray(vals, double, asCNumeric);
   UNPROTECT(2);

   return(values);
}




/*
  Creates

   Note that we could use the conversion used
   by the .C() routine to convert the objects
   in both directions.
 */
USER_OBJECT_
RS_GGOBI(variableToRS)(gint index, ggobid *gg)
{
  GGobiData *d = NULL;
  gint n, i;
  USER_OBJECT_ obj;

  if (g_slist_length (gg->d) == 1)
    d = (GGobiData *) g_slist_nth_data (gg->d, 0);
  else return(NULL_USER_OBJECT);
  
  n = d->nrows;

  PROTECT(obj = NEW_NUMERIC(n));

  for(i = 0 ; i < n ; i++) {
    NUMERIC_DATA(obj)[i] = d->raw.vals[index][i];
  }

  UNPROTECT(1);

  return (obj);
}


USER_OBJECT_
RS_GGOBI(getSmoothFunction)()
{
  USER_OBJECT_ ans;
  if(RS_smoothFunction == NULL)
    ans = NULL_USER_OBJECT;
  else
    ans = RS_smoothFunction;

 return(ans);
}


USER_OBJECT_
RS_GGOBI(setSmoothFunction)(USER_OBJECT_ func)
{
  USER_OBJECT_ old;
  old = RS_smoothFunction;

  if(old != NULL)
    R_ReleaseObject(RS_smoothFunction);
  else
    old = NULL_USER_OBJECT;

  RS_smoothFunction = func;
  R_PreserveObject(RS_smoothFunction);

  return(old);
}

/*
  Replaces the smoother callback in Ggobi 
  that is invoked when the user drags the window width slider.
  Because of the way we link R.so with libGGobi.so, we get to 
  slide this ahead of the other one rather than having to register 
  different callbacks, etc.
 */
void width_cb (GtkAdjustment *adj, ggobid *gg) {
  RS_GGOBI(smooth)(0,1, adj->value, gg);
}
