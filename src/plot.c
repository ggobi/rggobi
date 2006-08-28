#include "RSGGobi.h"
#include "RUtils.h"

#include <gtk/gtk.h>

#include "vars.h"
#include "parcoordsClass.h"
#include "scatmatClass.h"
#include "scatterplotClass.h"
#include "tsdisplay.h"
#include "barchartDisplay.h"

/*
  Allows the R user to set the variables within a given plot.
 */
USER_OBJECT_
RS_GGOBI(setPlotVariables)(USER_OBJECT_ varIds, USER_OBJECT_ dpy, USER_OBJECT_ plotId)
{
  displayd *display;
  splotd *sp;
  USER_OBJECT_ ans = NULL_USER_OBJECT;
  gint i;

  display = toDisplay(dpy);
	g_return_val_if_fail(GGOBI_IS_DISPLAY(display), NULL_USER_OBJECT);
  
  sp = (splotd*)g_list_nth_data(display->splots, INTEGER_DATA(plotId)[0]);
  if(sp == NULL) {
    PROBLEM "No such plot %d within this ggobi display" ,
                   (int) INTEGER_DATA(plotId)[0]
    ERROR;
  }

  for (i = 0; i < GET_LENGTH(varIds); i++) {
    gint button = VARSEL_X;
    gint jvar = INTEGER_DATA(varIds)[i];
    if (i < 3) {
      GtkWidget *wid = varpanel_widget_get_nth(i, jvar, display->d);
      if (GTK_WIDGET_VISIBLE(wid))
        button = i;
    }
    varsel(NULL, &display->cpanel, sp, jvar, button, -1, 0, 0, 0, display->d, 
      display->ggobi);
    varpanel_refresh(display, display->ggobi);
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
