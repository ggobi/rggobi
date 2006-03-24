#include "RSGGobi.h"

/**
  This returns a raw pointer to the Gtk window object
  associated with the given display, which is given
  as an S object of class ggobiDisplay.
 */
USER_OBJECT_
RS_GGOBI(getDisplayWindow)(USER_OBJECT_ display)
{
    displayd *dpy;
    dpy = (displayd*) R_ExternalPtrAddr((VECTOR_ELT(display, 1)));
    

    if(!dpy || GGOBI_IS_WINDOW_DISPLAY(dpy) == false)
		return(NULL_USER_OBJECT);

    return(toRPointer(GGOBI_WINDOW_DISPLAY(dpy)->window, "GtkWindow"));
}

/**
  Extracts a list of the splotd objects from the given displayd
  given as an S object of class ggobiDisplay.
  This returns the splotd objects as simple external pointers and 
  does not put a class on them.
 */
USER_OBJECT_
RS_GGOBI(getDisplayPlotWidgets)(USER_OBJECT_ display)
{
   displayd *dpy;
   int n;
   GList *tmp;
   USER_OBJECT_ els;

#if 1
    dpy =  R_ExternalPtrAddr(VECTOR_ELT(display, 1));
#else
    dpy = (displayd*) (long) NUMERIC_DATA(VECTOR_ELT(display, 1))[0];
#endif

    n = g_list_length(dpy->splots);
    PROTECT(els = NEW_LIST(n));

    tmp = dpy->splots;
    n = 0;
    while(tmp) {
	USER_OBJECT_ p;
	splotd *sp = (splotd *) tmp->data; 
	p = toRPointer(sp->da, "GtkWidget");
	SET_VECTOR_ELT(els, n++, p);
	tmp = tmp->next;
    }
    UNPROTECT(1);

    return(els);
}

USER_OBJECT_
RS_GGOBI(getMainMenubar)(USER_OBJECT_ gobi) {
  ggobid *gg = GGOBI_GGOBI(toGGobi(gobi));
  USER_OBJECT_ ans = NULL_USER_OBJECT;
  if(gg)
    ans = toRPointer(gg->main_menubar, "GtkWidget");
  return(ans);
}

USER_OBJECT_
RS_GGOBI(getDisplayMenubar)(USER_OBJECT_ display, USER_OBJECT_ gobiId) {
  displayd *dpy =  GetDisplay(display, gobiId, NULL);
  USER_OBJECT_ ans = NULL_USER_OBJECT;
  if(dpy)
    ans = toRPointer(dpy->menubar, "GtkWidget");
  return(ans);
}

USER_OBJECT_
RS_GGOBI(getMainWindow)(USER_OBJECT_ gobiId)
{
  ggobid *gg = GGOBI_GGOBI(toGGobi(gobiId));
  USER_OBJECT_ ans;

  ans = toRPointer(gg->main_window, "GtkWindow");

  return(ans);
}

