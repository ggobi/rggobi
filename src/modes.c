#include "RSGGobi.h"

#include <libxml/parser.h>
#include <libxml/xpath.h>

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

USER_OBJECT_
RS_INTERNAL_GGOBI(getModeNames)(USER_OBJECT_ s_display_type, const gchar* s_mode_type)
{
  USER_OBJECT_ ans;
  GType type = g_type_from_name(asCString(s_display_type));
  GGobiExtendedDisplayClass *display_class;
  xmlDocPtr doc;
  xmlXPathContextPtr ctx;
  xmlXPathObjectPtr result;
  gchar *expr;
  gint i;
  
  g_return_val_if_fail(type != G_TYPE_INVALID, NULL_USER_OBJECT);
  
  expr = g_strdup_printf("//menu[@action = '%s']/menuitem/@action", s_mode_type);
  
  display_class = g_type_class_peek(type);

  doc = xmlParseDoc(BAD_CAST(display_class->mode_ui_get(NULL)));
  ctx = xmlXPathNewContext(doc);
  result = xmlXPathEvalExpression(BAD_CAST(expr), ctx);
  
  PROTECT(ans = NEW_CHARACTER(xmlXPathNodeSetGetLength(result->nodesetval)));
  for (i = 0; i < GET_LENGTH(ans); i++) {
    xmlChar *str = xmlXPathCastNodeToString(xmlXPathNodeSetItem(result->nodesetval, i));
    SET_STRING_ELT(ans, i, mkChar((gchar *)str));
    free(str);
  }
  
  xmlXPathFreeObject(result);
  xmlXPathFreeContext(ctx);
  xmlFreeDoc(doc);
  g_free(expr);
  
  UNPROTECT(1);
  
  return ans;
}

USER_OBJECT_
RS_GGOBI(getPModeNames)(USER_OBJECT_ s_type_name)
{
  USER_OBJECT_ ans;
  
  ans = RS_INTERNAL_GGOBI(getModeNames)(s_type_name, "PMode");
  
  return(ans);
}

USER_OBJECT_
RS_GGOBI(getIModeNames)(USER_OBJECT_ s_type_name)
{
  USER_OBJECT_ ans;
  
  ans = RS_INTERNAL_GGOBI(getModeNames)(s_type_name, "IMode");
  
  return(ans);
}
