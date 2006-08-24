#include "RSGGobi.h"
#include "RUtils.h"

#include <gtk/gtk.h>

#include "vars.h"
#include "parcoordsClass.h"
#include "scatmatClass.h"
#include "scatterplotClass.h"

/*
  Allows the R user to set the variables within a given plot.
  Returns the previous settings in place of those that were
  given by the user. This allows the values to be restored
  easily at a later time.
 */
USER_OBJECT_
RS_GGOBI(setPlotVariables)(USER_OBJECT_ varIds, USER_OBJECT_ dpy, USER_OBJECT_ plotId)
{
  displayd *display;
  splotd *sp;
  long oldx, oldy;
  USER_OBJECT_ ans;
  int n;

  display = toDisplay(dpy);
	g_return_val_if_fail(GGOBI_IS_DISPLAY(display), NULL_USER_OBJECT);
  
  sp = (splotd*)g_list_nth_data(display->splots, INTEGER_DATA(plotId)[0]);
  if(sp == NULL) {
    PROBLEM "No such plot %d within this ggobi display" ,
                   (int) INTEGER_DATA(plotId)[0]
    ERROR;
  }

  if (GGOBI_IS_SCATTERPLOT_DISPLAY(display) || GGOBI_IS_SCATMAT_DISPLAY(display)) {
    oldx = sp->xyvars.x + 1;
    oldy = sp->xyvars.y + 1;
    sp->xyvars.x = INTEGER_DATA(varIds)[0];
    sp->xyvars.y = INTEGER_DATA(varIds)[1];
    n = 2;
  } else if (GGOBI_IS_PAR_COORDS_DISPLAY(display)) {
    oldx = sp->p1dvar +1;
    sp->p1dvar = INTEGER_DATA(varIds)[0];
    n = 1;
  }

  ans = NEW_INTEGER(n);
  INTEGER_DATA(ans)[0] = oldx;
   
  if(n > 1) {
    INTEGER_DATA(ans)[1] = oldy;
  }

 return(ans);
}

USER_OBJECT_
RS_GGOBI(setPlotRange)(USER_OBJECT_ x, USER_OBJECT_ y, USER_OBJECT_ rdisplay, USER_OBJECT_ plot)
{
  ggobid *gg;
  displayd *display;
  USER_OBJECT_ ans;

  display = toDisplay(rdisplay);
	g_return_val_if_fail(GGOBI_IS_DISPLAY(display), NULL_USER_OBJECT);

  GGOBI(setPlotRange)(NUMERIC_DATA(x), NUMERIC_DATA(y), INTEGER_DATA(plot)[0]-1, 
    display, false, display->ggobi);

  gdk_flush();

  ans = NEW_LOGICAL(1);
  LOGICAL_DATA(ans)[0] = TRUE;
  return(ans);
}
