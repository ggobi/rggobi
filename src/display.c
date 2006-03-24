#include "RSGGobi.h"
#include "RUtils.h"

#include <gtk/gtk.h>

#include "vars.h"

USER_OBJECT_
RS_GGOBI(newScatterplot)(USER_OBJECT_ variables, USER_OBJECT_ datasetId)
{
 ggobid *gg;
 GGobiData *d;
 USER_OBJECT_ ans;
 displayd *display;

  d = GGOBI_DATA(toData(datasetId));
  gg = d->gg;

 display = GGOBI(newScatterplot)(INTEGER_DATA(variables)[0],
                                 INTEGER_DATA(variables)[1],
                                 d, gg);
 display_add(display, gg);  
 ans = RS_displayInstance(display, gg, -1);

 return (ans);
}


USER_OBJECT_
RS_GGOBI(newParcoords)(USER_OBJECT_ variables, USER_OBJECT_ datasetId)
{
 ggobid *gg;
 GGobiData *d; 
 displayd *display;
 USER_OBJECT_ ans;
 gint *ids, n, i;

  d = GGOBI_DATA(toData(datasetId));
  gg = d->gg;
 
 n  = GET_LENGTH(variables);
/*XXX make certain to free this. */
 ids = g_malloc0(n * sizeof (gint));
 for(i = 0; i < n; i++) 
   ids[i] = INTEGER_DATA(variables)[i];

 display = GGOBI(newParCoords)(ids, n, d, gg);
 display_add(display, gg);  
 ans = RS_displayInstance(display, gg, -1);

 return (ans);
}


/*
  Assumes the variables are specified in

 We want to add a general version that takes
 pairs of variables and generates the corresponding 
 plot for each of these pairs and then adds them to the
 scatter matrix.

 */
USER_OBJECT_
RS_GGOBI(newScatmat)(USER_OBJECT_ x, USER_OBJECT_ y, USER_OBJECT_ datasetId)
{
  ggobid *gg;
  GGobiData *d;
  displayd *display;
  USER_OBJECT_ ans;
  gint *rowIds, *colIds, nr, nc, i;

  d = GGOBI_DATA(toData(datasetId));
  gg = d->gg;

  nr = GET_LENGTH(x);
  nc = GET_LENGTH(y);
 
  rowIds = g_malloc0 (nr * sizeof (gint));
  colIds = g_malloc0 (nc * sizeof (gint));
  for (i = 0; i < nr; i++) 
    rowIds[i] = INTEGER_DATA(x)[i];

  for (i = 0; i < nc; i++) 
    colIds[i] = INTEGER_DATA(y)[i];

  display = GGOBI(newScatmat)(rowIds, colIds, nr, nc, d, gg);
  display_add(display, gg);  
  ans = RS_displayInstance(display, gg, -1);

  return(ans);
}
USER_OBJECT_
RS_GGOBI(createPlot)(USER_OBJECT_ stype, USER_OBJECT_ svars, USER_OBJECT_ datasetId)
{
  GGobiData *d;
  ggobid *gg;
  displayd *display = NULL;
  GType type;
  GGobiExtendedDisplayClass *klass;

  d = GGOBI_DATA(toData(datasetId));
  gg = d->gg;

  type = (GType) NUMERIC_DATA(stype)[0];
  klass = GGOBI_EXTENDED_DISPLAY_CLASS(g_type_class_peek(type));

  if(!klass) {
     PROBLEM "Unrecognized display type"
     ERROR;
  }

  if(klass->createWithVars && GET_LENGTH(svars)) {
     gint nvars, *vars, i;
     nvars = GET_LENGTH(svars);
     vars = g_malloc(sizeof(gint)*nvars);
     for(i = 0; i < nvars; i++)
       vars[i] = INTEGER_DATA(svars)[i] - 1;
     display = klass->createWithVars(false, nvars, vars, d, gg);
  } else if(klass->create)
     display = klass->create(false, NULL, d, gg);

  if(!display) {
        PROBLEM "Couldn't create the display"
	ERROR;
  }

  display_add(display, gg);

  return(RS_displayInstance(display, gg, -1));
}

USER_OBJECT_
RS_GGOBI(createDisplay)(USER_OBJECT_ smissing, USER_OBJECT_ dataset)
{
  ggobid *gg;
  GGobiData *data = NULL;
  displayd *dpy;
  USER_OBJECT_ ans;


  data = GGOBI_DATA(toData(dataset));
  gg = data->gg;

  dpy = g_object_new(GGOBI_TYPE_EMBEDDED_DISPLAY, NULL);
  display_set_values(dpy, data, gg);

  display_add(dpy, gg);

  ans = toRPointer(dpy, "GGobiDisplay");

  return(ans);
}

USER_OBJECT_
RS_GGOBI(setDisplayWidth)(USER_OBJECT_ newDims, USER_OBJECT_ displayId, USER_OBJECT_ ggobiID)
{
 ggobid *gg; 
 displayd *dpy; 
 USER_OBJECT_ ans = NULL_USER_OBJECT;

 gg = GGOBI_GGOBI(toGGobi(ggobiID));
 dpy = GGOBI(getDisplay)(INTEGER_DATA(displayId)[0], gg);

 if(GGOBI_IS_WINDOW_DISPLAY(dpy)) {
	windowDisplayd *wdpy = GGOBI_WINDOW_DISPLAY(dpy);
    ans = NEW_INTEGER(2);
    g_object_get(wdpy->window, "width", INTEGER_DATA(ans), "height", INTEGER_DATA(ans)+1, NULL);

   if(GET_LENGTH(newDims)) {
     gtk_widget_set_size_request(wdpy->window, INTEGER_DATA(newDims)[0], INTEGER_DATA(newDims)[1]);
     gdk_flush();
   }
 } else {
   PROBLEM  "the specified ggobi display is not sizeable (it is embedded!)"
   ERROR;
 }

 return(ans);
}

/**
 This is called when we have reset all the variables in the different
 splots within a display.
 The intent is that this will recompute everything, including the 
 positions of the points/glyphs. Currently this is not doing that.
 Need to call some other method.
 */
USER_OBJECT_
RS_GGOBI(updateDisplay)(USER_OBJECT_ dpy, USER_OBJECT_ ggobiId)
{
 USER_OBJECT_  ans = NEW_LOGICAL(1);
  ggobid *gg;
  displayd *display;  

  if((display = GetDisplay(dpy, ggobiId, &gg))) {
      /*    ruler_ranges_set(display, sp, gg); */
    display_tailpipe(display, FULL,  gg);
    /*    displays_plot(NULL, FULL,  gg); */

    /*
      varpanel_refresh (display, gg);
    */

    gdk_flush();
    LOGICAL_DATA(ans)[0] = TRUE;
  }
  return(ans);
}


/*
  Returns a string identifying the 
  type of plots contained within a particular displayd
  object.
  (Note that this is not useful for programmatically created
   plots with non-homegeneous types.)
 */
SEXP
RS_GGOBI(getDisplayType)(SEXP dpy, SEXP ggobiId)
{
  displayd *display;
  const char *tmp;
  SEXP ans, names;

  display = GetDisplay(dpy, ggobiId, NULL);
   
  if(display) {
     tmp = ggobi_display_title_label(display);
     PROTECT(ans = NEW_CHARACTER(1));
     PROTECT(names = NEW_CHARACTER(1));
     SET_STRING_ELT(ans, 0, COPY_TO_USER_STRING(tmp));
     //SET_STRING_ELT(names, 0, COPY_TO_USER_STRING(getDisplayTypeName(display)));
     UNPROTECT(1);
  } else 
    ans = NULL_USER_OBJECT;

 return(ans);
}

/*
 Returns the number of splotd objects contained within a
 a given displayd object.
 */

USER_OBJECT_
RS_GGOBI(getNumPlotsInDisplay)(USER_OBJECT_ dpy, USER_OBJECT_ ggobiId)
{
  displayd *display;

  USER_OBJECT_ ans = NEW_INTEGER(1);
  display = GetDisplay(dpy, ggobiId, NULL);
  if(display) {
   int len;
      len = g_list_length(display->splots);
      INTEGER_DATA(ans)[0] = len;
  }

 return(ans);
}

USER_OBJECT_
RS_GGOBI(getDisplayOptions)(USER_OBJECT_ which, USER_OBJECT_ ggobiId)
{
  ggobid *gg = GGOBI_GGOBI(toGGobi(ggobiId));
  USER_OBJECT_ ans, names;
  gint NumOptions = 8;
  gint displayNum;
  DisplayOptions *options;

  if(gg == NULL)
    return(NULL_USER_OBJECT);
  
  displayNum = INTEGER_DATA(which)[0];

  options = GGOBI(getDisplayOptions)(displayNum, gg);
  if (options == NULL) {
    char error_buf[400];
    sprintf(error_buf,
	    "No display numbered %d", displayNum);
    Rf_error(error_buf);  
  }

  PROTECT(ans = NEW_LOGICAL(NumOptions));
  PROTECT(names = NEW_CHARACTER(NumOptions));

  LOGICAL_DATA(ans)[DOPT_POINTS] = options->points_show_p;
  SET_STRING_ELT(names, DOPT_POINTS, COPY_TO_USER_STRING("Show points"));
  LOGICAL_DATA(ans)[DOPT_AXES] = options->axes_show_p;
  SET_STRING_ELT(names, DOPT_AXES,  COPY_TO_USER_STRING("Show axes"));

  LOGICAL_DATA(ans)[DOPT_AXESLAB] = options->axes_label_p;
  SET_STRING_ELT(names, DOPT_AXESLAB,
    COPY_TO_USER_STRING("Show tour axes"));
  LOGICAL_DATA(ans)[DOPT_AXESVALS] = options->axes_values_p;
  SET_STRING_ELT(names, DOPT_AXESVALS,
    COPY_TO_USER_STRING("Show axes labels"));

  LOGICAL_DATA(ans)[DOPT_EDGES_U] = options->edges_undirected_show_p;
  SET_STRING_ELT(names, DOPT_EDGES_U, COPY_TO_USER_STRING("Undirected edges"));
  LOGICAL_DATA(ans)[DOPT_EDGES_A] = options->edges_arrowheads_show_p;
  SET_STRING_ELT(names, DOPT_EDGES_A, COPY_TO_USER_STRING("Arrowheads"));
  LOGICAL_DATA(ans)[DOPT_EDGES_D] = options->edges_directed_show_p;
  SET_STRING_ELT(names, DOPT_EDGES_D, COPY_TO_USER_STRING("Directed edges"));

  LOGICAL_DATA(ans)[DOPT_WHISKERS] = options->whiskers_show_p;
  SET_STRING_ELT(names, DOPT_WHISKERS,
    COPY_TO_USER_STRING("Show whiskers"));

/* unused
  LOGICAL_DATA(ans)[5] = options->missings_show_p;
  SET_STRING_ELT(names, 5, COPY_TO_USER_STRING("Missing Values"));
  LOGICAL_DATA(ans)[8] = options->axes_center_p;
  SET_STRING_ELT(names, 8,  COPY_TO_USER_STRING("Center axes"));
  LOGICAL_DATA(ans)[9] = options->double_buffer_p;
  SET_STRING_ELT(names, 9,  COPY_TO_USER_STRING("Double buffer"));
  LOGICAL_DATA(ans)[10] = options->link_p;
  SET_STRING_ELT(names, 10,  COPY_TO_USER_STRING("Link"));
*/

  SET_NAMES(ans, names);

  UNPROTECT(2);

  return(ans);
}
/*

 */
USER_OBJECT_
RS_GGOBI(setDisplayOptions)(USER_OBJECT_ which, USER_OBJECT_ values,
                              USER_OBJECT_ ggobiId)
{
  ggobid *gg = NULL;
  gint displayNum, i;
  DisplayOptions *options;
  int apply = 0;

  if(GET_LENGTH(ggobiId) == 0) {
	  /* XXX */
     options = GGOBI(getDefaultDisplayOptions)();
  } else {
     gg = GGOBI_GGOBI(toGGobi(ggobiId));
     displayNum = INTEGER_DATA(which)[0];
     options = GGOBI(getDisplayOptions)(displayNum, gg);

     if(options == NULL) {
	     char error_buf[400];
	     sprintf(error_buf,
		     "No display numbered %d", displayNum);
	     Rf_error(error_buf);
     }
     apply = 1;
  }


  if(GET_LENGTH(values) != 8) {
    PROBLEM "Incorrect length %d for options", 
	      GET_LENGTH(values)
    ERROR;
  }

  i = 0;
  options->points_show_p = LOGICAL_DATA(values)[i++];
  options->axes_show_p = LOGICAL_DATA(values)[i++];
  options->axes_label_p = LOGICAL_DATA(values)[i++];
  options->axes_values_p = LOGICAL_DATA(values)[i++];
  options->edges_undirected_show_p = LOGICAL_DATA(values)[i++];
  options->edges_arrowheads_show_p = LOGICAL_DATA(values)[i++];
  options->edges_directed_show_p = LOGICAL_DATA(values)[i++];
  options->whiskers_show_p = LOGICAL_DATA(values)[i++];
/* unused
  options->missings_show_p = LOGICAL_DATA(values)[i++];
  options->axes_center_p = LOGICAL_DATA(values)[i++];
  options->double_buffer_p = LOGICAL_DATA(values)[i++];
  options->link_p = LOGICAL_DATA(values)[i++];
*/

  if(apply) {
     displayd *display = GetDisplay(which, ggobiId, NULL);
     if(display)
      set_display_options(display, gg);
  }

  return (NULL_USER_OBJECT);
}

USER_OBJECT_
RS_GGOBI(getDisplayDataset)(USER_OBJECT_ dpy, USER_OBJECT_ ggobiId)
{
 displayd * display;
 ggobid *gg;
 USER_OBJECT_ ans;

  display = GetDisplay(dpy, ggobiId, &gg);
  if(display)
    ans = RS_datasetInstance(display->d, gg);
  else 
    ans = NULL_USER_OBJECT;
  return(ans);
}

/* Does it make sense to deal with the 'id' since displays can be destroyed? */
USER_OBJECT_
RS_displayInstance(displayd *display, ggobid *gg, int which)
{
 USER_OBJECT_ ans, names, tmp;

  PROTECT(ans = NEW_LIST(3));
  PROTECT(names = NEW_CHARACTER(3));

  SET_VECTOR_ELT(ans, 0, tmp = NEW_INTEGER(1));
  if(which < 0) {
    GList *l;
     which = 0;
     l = gg->displays;
    while(l) {
      if(l->data == display) {
        which++;
        break;
      }
      l = l->next;
    }
    if(which >= g_list_length(gg->displays)) {
      which = -1;
    }
  }
  INTEGER_DATA(tmp)[0] = which;
  SET_STRING_ELT(names, 0, COPY_TO_USER_STRING("id"));

  SET_VECTOR_ELT(ans, 1, toRPointer(display, "GGobiDisplay"));

  SET_STRING_ELT(names, 1, COPY_TO_USER_STRING("ref"));
  
  SET_VECTOR_ELT(ans, 2, RS_ggobiInstance(gg));
  SET_STRING_ELT(names, 2, COPY_TO_USER_STRING("ggobi"));

  PROTECT(tmp = NEW_CHARACTER(1));
  SET_STRING_ELT(tmp, 0, COPY_TO_USER_STRING("ggobiDisplay"));
  SET_CLASS(ans, tmp);
  SET_NAMES(ans, names);
  UNPROTECT(3);

  return(ans);
}

USER_OBJECT_ 
RS_GGOBI(closeDisplay)(USER_OBJECT_ ref, USER_OBJECT_ ggobiId)
{
  ggobid *gg = GGOBI_GGOBI(toGGobi(ggobiId));
  USER_OBJECT_ ans = NEW_LOGICAL(1);
  displayd *display;

  if(!gg)
    return(ans);

  display  = (displayd*) (long) NUMERIC_DATA(ref)[0];
  display = ValidateDisplayRef(display, gg, false);
  if(display) {
    display_free(display, true, gg);
    LOGICAL_DATA(ans)[0] = TRUE;
    gdk_flush();
  } 

  return(ans);
}

/* This function is of questionable use, since the user will always have
   a display object before calling any display functions from R. The R wrapper
   should send just the reference to the C side.
*/
displayd *
GetDisplay(USER_OBJECT_ rdisplay, USER_OBJECT_ ggobiId, ggobid **setgg)
{
 ggobid *gg;
 displayd *display = NULL;

  gg = GGOBI_GGOBI(toGGobi(ggobiId));
  if(gg == NULL)
    return(NULL);

  if(setgg)
    *setgg = gg;

  if(R_IS(rdisplay, "ggobiDisplay")) {
    rdisplay = VECTOR_ELT(rdisplay, 1);   
  } else if(IS_INTEGER(rdisplay)) {
   display =  GGOBI(getDisplay)(INTEGER_DATA(rdisplay)[0], gg);
  } 

  if(IS_NUMERIC(rdisplay)) {
     display  = (displayd*) (long) NUMERIC_DATA(rdisplay)[0];
  } else if(TYPEOF(rdisplay) == EXTPTRSXP) {
     display = (displayd *) R_ExternalPtrAddr(rdisplay);    
  }

  if(display) {
    display = ValidateDisplayRef(display, gg, false);
    if(!display)
      display = GGOBI(getDisplay)(((long) NUMERIC_DATA(rdisplay)[0] - 1), gg);
  }

 return(display);
}

/**
 Probably should be deprecated.
 */
void
RS_GGOBI(raiseDisplay)(glong *plotId, glong *numEls, glong *raiseOrIcon,
  glong *up, glong *ggobiId)
{
  ggobid *gg = ggobi_get(*ggobiId);
  gint i;

  for (i = 0; i < *numEls; i++) {
    plotId[i] = GGOBI(raiseWindow)(plotId[i], raiseOrIcon[0], up[0], gg);
  }
}
