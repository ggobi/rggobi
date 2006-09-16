#include "RSGGobi.h"
#include "RUtils.h"

#include <gtk/gtk.h>

#include "vars.h"
#include "parcoordsClass.h"
#include "scatmatClass.h"
#include "scatterplotClass.h"
#include "tsdisplay.h"
#include "barchartDisplay.h"

USER_OBJECT_
RS_GGOBI(setPlotScale)(USER_OBJECT_ x, USER_OBJECT_ y, USER_OBJECT_ rdisplay, USER_OBJECT_ plot)
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

USER_OBJECT_
RS_GGOBI(getPlotScale)(USER_OBJECT_ rdisplay, USER_OBJECT_ plot)
{
  displayd *display;
  splotd *sp;
  USER_OBJECT_ ans;

  display = toDisplay(rdisplay);
  g_return_val_if_fail(GGOBI_IS_DISPLAY(display), NULL_USER_OBJECT);
  
  sp = GGOBI(getPlot)(display, INTEGER_DATA(plot)[0]-1);
  g_return_val_if_fail(GGOBI_IS_SPLOT(sp), NULL_USER_OBJECT);
  
  ans = NEW_NUMERIC(2);
  NUMERIC_DATA(ans)[0] = sp->scale.x;
  NUMERIC_DATA(ans)[1] = sp->scale.y;
  
  return ans;
}
