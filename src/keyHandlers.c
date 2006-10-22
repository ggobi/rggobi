#include "RSGGobi.h"
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include "RSEval.h"

/*
 Takes care of registering, unregistering and invoking handlers for key press events
 from ggobi for the numbered keys (i.e. 0, ..., 9)
 */

gboolean RS_GGOBI(GenericKeyPressHandler)(guint keyval, GtkWidget *w, GdkEventKey *event,  cpaneld *cpanel, splotd *sp, ggobid *gg, void *userData);

USER_OBJECT_ RS_INTERNAL_GGOBI(createRSGdkEvent)(GdkEventKey *event);

USER_OBJECT_
RS_GGOBI(setNumberedKeyHandler)(USER_OBJECT_ handler, USER_OBJECT_ sdescription, USER_OBJECT_ ggobiId)
{
 extern void R_PreserveObject(USER_OBJECT_);
  ggobid *gg = toGGobi(ggobiId);
  USER_OBJECT_ ans = NULL_USER_OBJECT;
  g_return_val_if_fail(GGOBI_IS_GGOBI(gg), NULL_USER_OBJECT);
  

  if(GET_LENGTH(handler)) { 
    char *description = CHAR_DEREF(STRING_ELT(sdescription, 0));
    R_PreserveObject(handler);
    GGOBI(registerNumberedKeyEventHandler)(RS_GGOBI(GenericKeyPressHandler), handler, 
                                           description, NULL, gg, R);
  } else {
    KeyEventHandler *old = GGOBI(removeNumberedKeyEventHandler)(gg);
    g_free(old->description);
    g_free(old);
  }

  return(ans);
}

#define NUM_HANDLER_ARGS 4
/*
 The invocation handler function within the handler object must be last.
 */

gboolean
RS_GGOBI(GenericKeyPressHandler)(guint keyval, GtkWidget *w, GdkEventKey *event,  cpaneld *cpanel, splotd *sp, ggobid *gg, void *userData)
{
  USER_OBJECT_ myHandler;
  USER_OBJECT_ skey, argList, fun;

  myHandler = (USER_OBJECT_) userData;
  PROTECT(argList =  NEW_LIST(NUM_HANDLER_ARGS));
   SET_VECTOR_ELT(argList, 0, skey = NEW_INTEGER(1));
   INTEGER_DATA(skey)[0] = keyval - GDK_0;

   SET_VECTOR_ELT(argList, 2, RS_ggobiInstance(gg));
   SET_VECTOR_ELT(argList, 3, toRPointer(event, "GdkEventKey"));
  
   fun = VECTOR_ELT(myHandler, GET_LENGTH(myHandler)-1);

   RS_GGOBI(callFunctionWithArgs)(fun, argList);
  UNPROTECT(1);

  return(true);
}
/*
USER_OBJECT_
RS_INTERNAL_GGOBI(createRSGdkEvent)(GdkEventKey *event)
{
 USER_OBJECT_ ans, klassName;
  PROTECT(ans = NEW_NUMERIC(1));
  NUMERIC_DATA(ans)[0] = (double) (long) event;
  PROTECT(klassName = NEW_CHARACTER(1));
  SET_STRING_ELT(klassName, 0, COPY_TO_USER_STRING("GdkEventKey"));
  SET_CLASS(ans, klassName);

  UNPROTECT(2);
  return(ans);
}
*/

USER_OBJECT_
RS_GGOBI(getNumberedKeyHandler)(USER_OBJECT_ ggobiId)
{
 USER_OBJECT_ ans = NULL_USER_OBJECT;

   ggobid *gg = toGGobi(ggobiId);
   KeyEventHandler *h;
  g_return_val_if_fail(GGOBI_IS_GGOBI(gg), NULL_USER_OBJECT);
   

   if(gg == NULL) {
      PROBLEM "Error in accessing a ggobi instance"
      ERROR;
   }

   h = gg->NumberedKeyEventHandler;
   if(h == NULL)
     return(ans);

   if(h->language == R) {
     ans = (USER_OBJECT_) h->userData;
   } else {
     PROBLEM "The register key handler is not an R handler. Cannot currently handle this!"
     ERROR;
   }
   return(ans);
}

