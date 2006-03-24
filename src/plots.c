#include "RSGGobi.h"
#include "vars.h"

USER_OBJECT_
RS_GGOBI(getActivePlot)(USER_OBJECT_ ggobiId)
{
  USER_OBJECT_ ans;
  ggobid *gg = GGOBI_GGOBI(toGGobi(ggobiId));

  PROTECT(ans = NEW_INTEGER(2));

  INTEGER_DATA(ans)[0] = GGOBI(getCurrentDisplayIndex)(gg);
  INTEGER_DATA(ans)[1] = GGOBI(getCurrentPlotIndex)(gg);

  UNPROTECT(1);

  return(ans);
}

/*
  which is a vector length 1 or 2.
  The first element is the displayd object to make active.
  The second argument (if present) identifies the plot that is to be made
  active.
 */
USER_OBJECT_
RS_GGOBI(setActivePlot)(USER_OBJECT_ which, USER_OBJECT_ ggobiId)
{
  ggobid *gg = GGOBI_GGOBI(toGGobi(ggobiId));
  gint n;
  USER_OBJECT_ ans;

  n = GET_LENGTH(which);

  PROTECT(ans = NEW_LOGICAL(n));

  GGOBI(setCurrentDisplay)(INTEGER_DATA(which)[0], gg);
  LOGICAL_DATA(ans)[0] = 1;

  if (n > 1) {
    displayd *display;
    splotd *sp = NULL;
    display = GGOBI(getCurrentDisplay)(gg);
    if (display != NULL)
      sp = GGOBI(getPlot)(display, INTEGER_DATA(which)[1]);

    if (sp) {
      GGOBI(splot_set_current_full)(display, sp, gg);
      LOGICAL_DATA(ans)[1] = 1;
    }
  }
  gdk_flush();
   
  UNPROTECT(1); 
  return (ans);
}
