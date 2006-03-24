#include "RSGGobi.h"

/* we need to split this stuff into interaction and projection modes */

USER_OBJECT_
RS_GGOBI(setIMode)(USER_OBJECT_ name, USER_OBJECT_ ggobiId)
{
  ggobid *gg = GGOBI_GGOBI(toGGobi(ggobiId));
  if(gg) {
    GGOBI(setIMode)(CHAR_DEREF(STRING_ELT(name, 0)), gg);
    /* Trying to get the main window updated so that the changes to the control
       panel are visible without having to move the mouse into the window to generate
       another event. */
    //gtk_widget_queue_draw(gg->main_window);
    //gtk_widget_queue_draw(gg->current_control_panel);
/*    gtk_widget_draw(gg->main_window); */
  }
  
  gdk_flush();

  return(NULL_USER_OBJECT);
}
USER_OBJECT_
RS_GGOBI(setPMode)(USER_OBJECT_ name, USER_OBJECT_ ggobiId)
{
  ggobid *gg = GGOBI_GGOBI(toGGobi(ggobiId));
  if(gg) {
    GGOBI(setPMode)(CHAR_DEREF(STRING_ELT(name, 0)), gg);
  }
  
  //gdk_flush();

  return(NULL_USER_OBJECT);
}

USER_OBJECT_
RS_GGOBI(getIModeName)(USER_OBJECT_ ggobiId)
{
  USER_OBJECT_ ans;
  const gchar *tmp;
  ggobid *gg = GGOBI_GGOBI(toGGobi(ggobiId));
  tmp = GGOBI(getIModeName)(imode_get(gg));

  PROTECT(ans = NEW_CHARACTER(1));
  if(tmp && tmp[0])
    SET_STRING_ELT(ans, 0, COPY_TO_USER_STRING(tmp));
  UNPROTECT(1);
  return(ans);
}
USER_OBJECT_
RS_GGOBI(getPModeName)(USER_OBJECT_ ggobiId)
{
  USER_OBJECT_ ans;
  const gchar *tmp;
  ggobid *gg = GGOBI_GGOBI(toGGobi(ggobiId));
  tmp = GGOBI(getPModeName)(pmode_get(gg->current_display, gg));

  PROTECT(ans = NEW_CHARACTER(1));
  if(tmp && tmp[0])
    SET_STRING_ELT(ans, 0, COPY_TO_USER_STRING(tmp));
  UNPROTECT(1);
  return(ans);
}

