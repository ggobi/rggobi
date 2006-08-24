#include "RSGGobi.h"

/* we need to split this stuff into interaction and projection modes */

USER_OBJECT_
RS_GGOBI(setIMode)(USER_OBJECT_ name, USER_OBJECT_ s_display)
{
  displayd *display = toDisplay(s_display);
  g_return_val_if_fail(GGOBI_IS_DISPLAY(display), NULL_USER_OBJECT);
  GGOBI(setIMode)(CHAR_DEREF(STRING_ELT(name, 0)), display->ggobi);
  
  gdk_flush();

  return(NULL_USER_OBJECT);
}
USER_OBJECT_
RS_GGOBI(setPMode)(USER_OBJECT_ name, USER_OBJECT_ s_display)
{
  displayd *display = toDisplay(s_display);
  g_return_val_if_fail(GGOBI_IS_DISPLAY(display), NULL_USER_OBJECT);
  GGOBI(setPMode)(CHAR_DEREF(STRING_ELT(name, 0)), display->ggobi);
  
  return(NULL_USER_OBJECT);
}

USER_OBJECT_
RS_GGOBI(getIModeName)(USER_OBJECT_ s_display)
{
  USER_OBJECT_ ans;
  const gchar *tmp;
  displayd *display = toDisplay(s_display);
  g_return_val_if_fail(GGOBI_IS_DISPLAY(display), NULL_USER_OBJECT);
  tmp = GGOBI(getIModeName)(imode_get(display->ggobi));

  PROTECT(ans = NEW_CHARACTER(1));
  if(tmp && tmp[0])
    SET_STRING_ELT(ans, 0, COPY_TO_USER_STRING(tmp));
  UNPROTECT(1);
  return(ans);
}
USER_OBJECT_
RS_GGOBI(getPModeName)(USER_OBJECT_ s_display)
{
  USER_OBJECT_ ans;
  const gchar *tmp;
  displayd *display = toDisplay(s_display);
  g_return_val_if_fail(GGOBI_IS_DISPLAY(display), NULL_USER_OBJECT);
  tmp = GGOBI(getPModeName)(pmode_get(display, display->ggobi));

  PROTECT(ans = NEW_CHARACTER(1));
  if(tmp && tmp[0])
    SET_STRING_ELT(ans, 0, COPY_TO_USER_STRING(tmp));
  UNPROTECT(1);
  return(ans);
}

