#include "RSGGobi.h"
#include "vars.h"

/* Gets the active plot in a GGobi context */
/* Returns a list containing the display reference and the plot index */
USER_OBJECT_
RS_GGOBI(getActivePlot)(USER_OBJECT_ ggobiId)
{
  USER_OBJECT_ ans;
  ggobid *gg = toGGobi(ggobiId);
  g_return_val_if_fail(GGOBI_IS_GGOBI(gg), NULL_USER_OBJECT);

  PROTECT(ans = NEW_LIST(2));

  SET_VECTOR_ELT(ans, 0, RS_displayInstance(gg->current_display));
  SET_VECTOR_ELT(ans, 1, asRInteger(GGOBI(getCurrentPlotIndex)(gg)));

  UNPROTECT(1);

  return(ans);
}

/* Sets a plot as the active plot given a display and a plot index */
USER_OBJECT_
RS_GGOBI(setActivePlot)(USER_OBJECT_ s_display, USER_OBJECT_ s_plot)
{
  USER_OBJECT_ ans = NEW_LOGICAL(1);
  displayd *display;
  splotd *sp;
  
  display = toDisplay(s_display);
  g_return_val_if_fail(GGOBI_IS_DISPLAY(display), NULL_USER_OBJECT);
  
  display_set_current(display, display->ggobi);

  sp = GGOBI(getPlot)(display, INTEGER_DATA(s_plot)[0]);
  g_return_val_if_fail(sp != NULL, NULL_USER_OBJECT);
  GGOBI(splot_set_current_full)(display, sp, display->ggobi);
  LOGICAL_DATA(ans)[0] = 1;

  gdk_flush();
   
  return (ans);
}
