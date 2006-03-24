#include "RSGGobi.h"
#include <gtk/gtk.h>

USER_OBJECT_
RS_GGOBI(getNumDisplays)(USER_OBJECT_ ggobiId)
{
  ggobid *gg = GGOBI_GGOBI(toGGobi(ggobiId));
  int len;
  USER_OBJECT_ ans = NEW_INTEGER(1);
  if(gg) {
   len = g_list_length(gg->displays);
   INTEGER_DATA(ans)[0] = len;
  }

 return(ans);
}
/* how about getting the current display and then getting its type? */
USER_OBJECT_
RS_GGOBI(getCurrentDisplayType)(USER_OBJECT_ ggobiId)
{
  ggobid *gg = GGOBI_GGOBI(toGGobi(ggobiId));
  USER_OBJECT_ ans;
  const gchar *name = GGOBI(getCurrentDisplayType)(gg);

  PROTECT(ans = NEW_CHARACTER(1));
  SET_STRING_ELT(ans, 0, COPY_TO_USER_STRING(name));

  UNPROTECT(1);

  return(ans);
}

/**
 queries ggobi's list of display types, assuming GGobi has been initialized.
*/
USER_OBJECT_
RS_GGOBI(getDisplayTypes)()
{
  GSList *l;
  gint n;
  USER_OBJECT_ ans, names;

  l = GGOBI(getExtendedDisplayTypes)();
 
  n = g_slist_length(l); 
  PROTECT(ans = NEW_LIST(n));
  PROTECT(names = NEW_CHARACTER(n));

  n = 0;
  for( ; l ; l = l->next, n++) {
     GGobiExtendedDisplayClass *klass;
     klass = GGOBI_EXTENDED_DISPLAY_CLASS((GtkObjectClass*) l->data);
     //SET_STRING_ELT(ans, n, COPY_TO_USER_STRING(klass->titleLabel));
	 SET_STRING_ELT(names, n, COPY_TO_USER_STRING(klass->titleLabel));
     SET_VECTOR_ELT(ans, n, asRString(g_type_name(G_OBJECT_CLASS_TYPE(klass))));
  }

  SET_NAMES(ans, names);
  UNPROTECT(2);

  return(ans);
}

USER_OBJECT_
RS_GGOBI(raiseOrLowerDisplays)(USER_OBJECT_ displays, USER_OBJECT_ iconify, USER_OBJECT_ raise, USER_OBJECT_ ggobiId)
{
    USER_OBJECT_ ans;
    int numDisplays = GET_LENGTH(displays), i;

    if(numDisplays == 0)
	return(NULL_USER_OBJECT);

    PROTECT(ans = NEW_LOGICAL(numDisplays));

    for(i = 0; i < numDisplays; i++) {
      displayd *display;
      windowDisplayd *wdpy;
      display = GetDisplay(VECTOR_ELT(displays, i), ggobiId, NULL);
      if(display == NULL || GGOBI_IS_WINDOW_DISPLAY(display) == false)
	  continue;

      wdpy = GGOBI_WINDOW_DISPLAY(display);

      if(LOGICAL_DATA(iconify)[0] == FALSE) {
        if(LOGICAL_DATA(raise)[0]) 
          gdk_window_raise(wdpy->window->window); 
        else
          gdk_window_lower(wdpy->window->window); 
      } else {
          /* This doesn't seem to iconify things, just kill them! 
             XIconifyWindow(GDK_WINDOW_XDISPLAY(GTK_WIDGET(window)->window),
                             GDK_WINDOW_XWINDOW(GTK_WIDGET(window)->window),
                             DefaultScreen (GDK_DISPLAY ()));             
           */
        if(LOGICAL_DATA(raise)[0]) 
          gtk_widget_show_all(wdpy->window); 
        else
          gtk_widget_hide_all(wdpy->window); 
      }

      LOGICAL_DATA(ans)[i] = TRUE;
    }

    UNPROTECT(1);

    gdk_flush();
    return(ans);
}

USER_OBJECT_
RS_INTERNAL_GGOBI(getDisplays)(ggobid *gg)
{
  USER_OBJECT_ ans; 
  gint n, i;
  GList *dlist;

  n = g_list_length(gg->displays);
  

  PROTECT(ans = NEW_LIST(n));
  i = 0;
  for (dlist = gg->displays; dlist ; dlist = dlist->next, i++) {
    SET_VECTOR_ELT(ans, i, RS_displayInstance((displayd*)dlist->data, gg, -1));
  }
  UNPROTECT(1);

 return(ans);
}

/*
  Get a list of the displays in the specified ggobi.
  This works recursively
 */
USER_OBJECT_
RS_GGOBI(getDisplays)(USER_OBJECT_ ggobiId)
{
  ggobid *gg = GGOBI_GGOBI(toGGobi(ggobiId));
  return(RS_INTERNAL_GGOBI(getDisplays)(gg));
}

USER_OBJECT_
RS_GGOBI(getCurrentDisplay)(USER_OBJECT_ gobiId)
{
  ggobid *gg = GGOBI_GGOBI(toGGobi(gobiId));
  USER_OBJECT_ ans;

  ans = toRPointer(gg->current_display, "GtkWidget");

  return(ans);
}
