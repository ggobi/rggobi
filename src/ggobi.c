#include "RSGGobi.h"
#include "RUtils.h"

#ifndef G_OS_WIN32
#include <sys/time.h>
#include "R_ext/eventloop.h"
#include <gdk/gdkx.h>
#else
#include <process.h>
/*static void __cdecl GtkEventThreadHandler(void *display);*/
void R_gtk_handle_events();
extern void (*R_gtkdo)();
extern __declspec(dllimport) void (*R_tcldo)();
#endif

#include "R_ext/RS.h"

extern void RS_GGOBI(event_handle)(void *data);
void RS_GGOBI(limited_event_handle)(gint max);

/*XXX used to be R_IsNaNorNA, but no longer available for us.
      R_finite() excludes infinite values also. This is probably okay for now.
*/
int
isMissingValue(double d)
{
    return(!R_finite(d) || d == R_NaInt);
}

static void log_handler(const gchar *log_domain, GLogLevelFlags log_level,
								const gchar *message, gpointer user_data)
{
	if (log_level & (G_LOG_LEVEL_CRITICAL | G_LOG_FLAG_FATAL))
		error(message);
	else if (log_level & G_LOG_LEVEL_WARNING) 
		warning(message);
}
static void
registerErrorHandlers()
{
	g_log_set_handler("Gtk", G_LOG_LEVEL_MASK | G_LOG_FLAG_FATAL
            | G_LOG_FLAG_RECURSION, log_handler, NULL);
	g_log_set_handler("Gdk", G_LOG_LEVEL_MASK | G_LOG_FLAG_FATAL
            | G_LOG_FLAG_RECURSION, log_handler, NULL);
	g_log_set_handler("Pango", G_LOG_LEVEL_MASK | G_LOG_FLAG_FATAL
            | G_LOG_FLAG_RECURSION, log_handler, NULL);
	g_log_set_handler("GLib-GObject", G_LOG_LEVEL_MASK | G_LOG_FLAG_FATAL
            | G_LOG_FLAG_RECURSION, log_handler, NULL);
	g_log_set_handler("GLib", G_LOG_LEVEL_MASK | G_LOG_FLAG_FATAL
            | G_LOG_FLAG_RECURSION, log_handler, NULL);
	g_log_set_handler(NULL, (G_LOG_LEVEL_MASK | G_LOG_FLAG_FATAL
            | G_LOG_FLAG_RECURSION) & ~G_LOG_LEVEL_DEBUG, log_handler, NULL);
}

/*
  Create a ggobi instance and initialize.
 */
USER_OBJECT_
RS_GGOBI(init)(USER_OBJECT_ args, USER_OBJECT_ createInstance)
{

 gint which, i;
 char **c_args;
 int n = GET_LENGTH(args);
 USER_OBJECT_ ans;

   c_args = g_malloc(sizeof(char *)*n);
   for(i = 0; i < n ; i++) {
     c_args[i] = CHAR_DEREF(STRING_ELT(args, i));
   }
   if(LOGICAL_DATA(createInstance)[0]) {
     which = GGOBI(main)(n, c_args, false);
     ans = RS_ggobiInstance(ggobi_get(which-1));
   } else {
      ggobiInit(&n, &c_args);
      ans = NEW_LOGICAL(1);
      LOGICAL_DATA(ans)[0] = TRUE;
   }
   g_free(c_args);

#ifdef G_OS_WIN32 
   R_tcldo = R_gtk_handle_events;
#else
  addInputHandler (R_InputHandlers, ConnectionNumber(gdk_display),
                   RS_GGOBI(event_handle), -1);
#endif

	//registerErrorHandlers();
  GGobi_setMissingValueIdentifier(isMissingValue);
	
  gdk_flush();    

  return(ans);
}


/**
 Get a reference to a specific GGobi instance by indexing
 into the list of active/alive GGobis.
 */
USER_OBJECT_
RS_GGOBI(getGGobi)(USER_OBJECT_ which)
{
 USER_OBJECT_ ans;
 ggobid *gg;
 int n = GET_LENGTH(which), i, index;
 PROTECT(ans =  NEW_LIST(n));
 
 for(i = 0; i < n ; i ++) {
  index = INTEGER_DATA(which)[i];
  gg = ggobi_get(index-1);
  if(gg) {
    SET_VECTOR_ELT(ans,i, RS_ggobiInstance(gg));
  }
 }

 UNPROTECT(1);
 return(ans);
}

/*
   Handle all the events currently in the queue,
   until there are no more.
   This is called by the R event loop when input is 
   detected on the file descriptor associated with the 
   X connection.

   This needs a little more finessing to make it
   work correctly.
 */
void
RS_GGOBI(event_handle)(void *data)
{
  RS_GGOBI(limited_event_handle)(-1);
}

/**
  Process max number of events and then terminate.
 */
void
RS_GGOBI(limited_event_handle)(gint max) {
  gint ctr = 0;
  gboolean block =  (max > -1);

  while(g_main_iteration(block)) {
    /*  fprintf(stderr, "Event %d\n",ctr); */
    /*
    gtk_main_iteration_do(false);
    */
    /*   g_main_iteration((ctr < max));  */
    ctr++;
    block = max > -1 && ctr < max;

    if(max > -1 && ctr >= max)
       return;
  }
}




/*
  A version of the event handling mechanism that one can call 
  directly from R to prohibit the prompt from 
  Note that selecting the Quit menu item causes this loop
  to exit and the routine to return control to the R event
  handling. Hence the prompt will return.

  Added for debugging purposes currently, but may have
  some value under special circumstances.
 */
void
RS_GGOBI(blockingLoop)(void)
{
  gtk_main();
}


USER_OBJECT_
RS_ggobiInstance(ggobid *gg) {
	return(toRPointer(gg, "ggobi"));
}

USER_OBJECT_
RS_GGOBI(getDescription)(USER_OBJECT_ ggobiId)
{
 ggobid *gg = GGOBI_GGOBI(toGGobi(ggobiId));
 GGobiData *d;
 gint numSlots = 3, numDatasets, i;
 DataMode mode;
 USER_OBJECT_ ans, names, tmp;
 const gchar *tmpname;

 if(gg == NULL) {
     RS_throwError("Invalid reference to GGobi instance");
 }

 if(!gg->d) {
     return(NULL_USER_OBJECT);
 }


 PROTECT(ans = NEW_LIST(numSlots));
 PROTECT(names = NEW_CHARACTER(numSlots));

 SET_VECTOR_ELT(ans, 0, NEW_CHARACTER(1));
 tmpname = GGOBI(getFileName(gg));
 if (tmpname)
   SET_STRING_ELT(VECTOR_ELT(ans, 0), 0,
     COPY_TO_USER_STRING((gchar *)tmpname));
 SET_STRING_ELT(names, 0, COPY_TO_USER_STRING("Filename"));


 SET_VECTOR_ELT(ans, 1, NEW_INTEGER(1));
 mode = GGOBI(getDataMode)(gg);
 INTEGER_DATA(VECTOR_ELT(ans, 1))[0] = mode;
 PROTECT(tmp = NEW_CHARACTER(1));

 SET_NAMES(VECTOR_ELT(ans, 1), tmp);
 UNPROTECT(1);
 SET_STRING_ELT(names, 1, COPY_TO_USER_STRING("Data mode"));


 numDatasets = g_slist_length (gg->d);

 SET_VECTOR_ELT(ans, 2,  tmp = allocMatrix(INTSXP, numDatasets, 2));
 for(i = 0; i < numDatasets ; i++) {
  d = (GGobiData *) g_slist_nth_data (gg->d, i);
  INTEGER_DATA(tmp)[i] = d->nrows;
  INTEGER_DATA(tmp)[i + numDatasets] = d->ncols;
 }
 SET_STRING_ELT(names, 2, COPY_TO_USER_STRING("Data dimensions"));

 SET_NAMES(ans, names);

 UNPROTECT(2);

 return(ans);
}

USER_OBJECT_
RS_GGOBI(isValid)(USER_OBJECT_ gobi)
{
 USER_OBJECT_ ans = NEW_LOGICAL(1);
 LOGICAL_DATA(ans)[0] = GGOBI_IS_GGOBI(toGGobi(gobi));
 return(ans);
}

ggobid *
toGGobi(USER_OBJECT_ s_ggobi)
{
 if(inherits(s_ggobi, "ggobi")) {
    ggobid *gg;
    gg = ValidateGGobiRef(getPtrValue(s_ggobi), false);
    return(gg);
 }

  return(NULL);
}

void
RS_GGOBI(getNumGGobiInstances)(glong *ans)
{
 ans[0] = GGOBI(getNumGGobis)();
}

USER_OBJECT_
RS_GGOBI(close)(USER_OBJECT_ gobi)
{
  ggobid *gg = GGOBI_GGOBI(toGGobi(gobi));
  USER_OBJECT_ ans = NEW_LOGICAL(1);
  if(gg) {
    LOGICAL_DATA(ans)[0] = GGOBI(close)(gg, true);
    gdk_flush();
  }

 return(ans);
}

/*
  Overrides the one in libGGobi.so 
  which is called when a window is deleted or explicitly
  closed.
 */
void
quit_ggobi(ggobid *gg)
{
  GGOBI(close)(gg, true);
}

#ifdef G_OS_WIN32

void
R_gtk_handle_events()
{
  RS_GGOBI(limited_event_handle)(-1);
}
#endif


#include "GGStructSizes.c"

USER_OBJECT_
RS_GGOBI(getStructSizes)(USER_OBJECT_ local)
{
   const GGobi_StructSize *sizes;
   int n = 0, i;
   USER_OBJECT_ ans, names;

   if(LOGICAL_DATA(local)[0]) {
     sizes = GGOBI(getGGobiStructs)(&n);
   } else {
     sizes = GGOBI(getStructs)(&n);
   }

   PROTECT(ans = NEW_INTEGER(n));
   PROTECT(names = NEW_CHARACTER(n));
   for(i = 0; i < n ; i++) {
      INTEGER_DATA(ans)[i] = sizes[i].size;
      SET_STRING_ELT(names, i, COPY_TO_USER_STRING(sizes[i].name));
   }

   SET_NAMES(ans, names);

   UNPROTECT(2);
   return(ans);
}

USER_OBJECT_
RS_GGOBI(ggobi_find_data_file)(USER_OBJECT_ name)
{
  return(asRString(ggobi_find_data_file(asCString(name))));
}

void
RS_GGOBI(flush)()
{
  gdk_flush();
}
