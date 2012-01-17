#include "RSGGobi.h"
#include "RUtils.h"

#include <gtk/gtk.h>

#include "vars.h"

USER_OBJECT_
RS_GGOBI(createDisplay)(USER_OBJECT_ stype, USER_OBJECT_ svars, 
  USER_OBJECT_ datasetId, USER_OBJECT_ useWindow)
{
  GGobiData *d;
  ggobid *gg;
  displayd *display = NULL;
  GType type;
  GGobiExtendedDisplayClass *klass;
  gboolean use_window = asCLogical(useWindow);

  d = toData(datasetId);
  g_return_val_if_fail(GGOBI_IS_DATA(d), NULL_USER_OBJECT);
  gg = d->gg;

  type = g_type_from_name(asCString(stype));
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
       vars[i] = INTEGER_DATA(svars)[i];
     display = klass->createWithVars(use_window, false, nvars, vars, d, gg);
  } else if(klass->create)
     display = klass->create(use_window, false, NULL, d, gg);

  if(!display) {
    PROBLEM "Couldn't create the display"
    ERROR;
  }

  display_add(display, gg);

  gdk_flush();
  
  return(RS_displayInstance(display));
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
  USER_OBJECT_ ans = NEW_LOGICAL(1);
  ggobid *gg;
  displayd *display;  

  gg = toGGobi(ggobiId);
  g_return_val_if_fail(GGOBI_IS_GGOBI(gg), NULL_USER_OBJECT);
  display = toDisplay(dpy);
  g_return_val_if_fail(GGOBI_IS_DISPLAY(display), NULL_USER_OBJECT);
  
  display_tailpipe(display, FULL,  gg);
  
  LOGICAL_DATA(ans)[0] = TRUE;
  return(ans);
}

/* Ideally the GGobi would be a property of the display, allowing access
  with RGtk2, but this is not so... */
USER_OBJECT_
RS_GGOBI(getGGobiForDisplay)(USER_OBJECT_ s_display)
{
  displayd *display = toDisplay(s_display);
  g_return_val_if_fail(GGOBI_IS_DISPLAY(display), NULL_USER_OBJECT);
  
  return RS_ggobiInstance(display->ggobi);
}

/*
 Returns the number of splotd objects contained within a
 a given displayd object.
 */

USER_OBJECT_
RS_GGOBI(getNumPlotsInDisplay)(USER_OBJECT_ dpy)
{
  displayd *display;
  gint len;
  
  USER_OBJECT_ ans = NEW_INTEGER(1);
  display = toDisplay(dpy);
	g_return_val_if_fail(GGOBI_IS_DISPLAY(display), NULL_USER_OBJECT);
  len = g_list_length(display->splots);
  INTEGER_DATA(ans)[0] = len;

  return(ans);
}

USER_OBJECT_
RS_GGOBI(getDisplayOptions)(USER_OBJECT_ which)
{
  USER_OBJECT_ ans, names;
  gint NumOptions = 8;
  DisplayOptions *options;
  
  if (GET_LENGTH(which) == 0)
    options = GGOBI(getDefaultDisplayOptions)();
  else {
    displayd *display = toDisplay(which);
    g_return_val_if_fail(GGOBI_IS_DISPLAY(display), NULL_USER_OBJECT);
    options = &(display->options);
  }
  
  g_return_val_if_fail(options != NULL, NULL_USER_OBJECT);

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
RS_GGOBI(setDisplayOptions)(USER_OBJECT_ which, USER_OBJECT_ values)
{
  gint i;
  DisplayOptions *options;
  displayd *display = NULL;
  int apply = 0;

  g_return_val_if_fail(GET_LENGTH(values) == 8, NULL_USER_OBJECT);
  
  if(GET_LENGTH(which) == 0) {
     options = GGOBI(getDefaultDisplayOptions)();
  } else {
     display = toDisplay(which);
     g_return_val_if_fail(GGOBI_IS_DISPLAY(display), NULL_USER_OBJECT);
     options = &(display->options);
     g_return_val_if_fail(options != NULL, NULL_USER_OBJECT);
     apply = 1;
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
    set_display_options(display, display->ggobi);
  }

  return (NULL_USER_OBJECT);
}

static void
toggle_display_variables(displayd *display, USER_OBJECT_ vars, gboolean active)
{
  gint i, j;
  for (j = 0; j < 3; j++) {
    USER_OBJECT_ varIds = VECTOR_ELT(vars, j);
    for (i = 0; i < GET_LENGTH(varIds); i++) {
      gint var = INTEGER_DATA(varIds)[i];
      GtkWidget *wid = varpanel_widget_get_nth(j, var, display->d);
      if (!GTK_IS_WIDGET(wid))
        error("Unknown variable");
      if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wid)) == active) {
        varsel(wid, &display->cpanel, display->current_splot, var, j, 
          -1, 0, 0, 0, display->d, display->ggobi);
      }
    }
  }
}

USER_OBJECT_
RS_GGOBI(getDisplayVariables)(USER_OBJECT_ dpy)
{
  USER_OBJECT_ buttons, vars, ans;
  static gchar *button_names[] = { "X", "Y", "Z" };
  gint i;
  
  displayd *display = toDisplay(dpy);
  
  /* get the currently plotted variables */
  gint *plotted_vars = g_new (gint, display->d->ncols);
  gint nplotted_vars = GGOBI_EXTENDED_DISPLAY_GET_CLASS (display)->plotted_vars_get(
    display, plotted_vars, display->d, display->ggobi);
    
  PROTECT(ans = NEW_LIST(2));
  buttons = NEW_CHARACTER(nplotted_vars);
  SET_VECTOR_ELT(ans, 1, buttons);
  vars = NEW_INTEGER(nplotted_vars);
  SET_VECTOR_ELT(ans, 0, vars);
  
  for (i = 0; i < nplotted_vars; i++) {
    gint var = plotted_vars[i], j;
    for (j = 0; j < G_N_ELEMENTS(button_names); j++) {
      GtkWidget *wid = varpanel_widget_get_nth(j, var, display->d);
      if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wid)))
        SET_STRING_ELT(buttons, i, mkChar(button_names[j]));
    }
    INTEGER_DATA(vars)[i] = var;
  }
  
  UNPROTECT(1);
  g_free(plotted_vars);
  
  return(ans);
}

/*
  Allows the R user to set the variables within a given display
 */
USER_OBJECT_
RS_GGOBI(setDisplayVariables)(USER_OBJECT_ vars, USER_OBJECT_ varPrev, USER_OBJECT_ dpy)
{
  displayd *display, *prev_display;
  USER_OBJECT_ ans = NULL_USER_OBJECT;

  display = toDisplay(dpy);
	g_return_val_if_fail(GGOBI_IS_DISPLAY(display), NULL_USER_OBJECT);
  
  /* ensure that display is current before changing variables */
  prev_display = display->ggobi->current_display;
  display_set_current(display, display->ggobi);
  
  /* If any of the requested variables AREN'T selected, select them */
  toggle_display_variables(display, vars, false);
  varpanel_refresh(display, display->ggobi);
  /* If any of the obsolete variables ARE still selected, select (toggle) them */
  toggle_display_variables(display, varPrev, true);
  
  /* refresh everything */
  varpanel_refresh(display, display->ggobi);
  display_tailpipe(display, FULL,  display->ggobi);
  while(gtk_events_pending()) /* ensure display is redrawn */
    gtk_main_iteration();
  
  /* revert to previously selected display */
  display_set_current(prev_display, prev_display->ggobi);
  
  return(ans);
}

USER_OBJECT_
RS_GGOBI(getDisplayDataset)(USER_OBJECT_ dpy)
{
 displayd * display;
 USER_OBJECT_ ans;

  display = toDisplay(dpy);
	g_return_val_if_fail(GGOBI_IS_DISPLAY(display), NULL_USER_OBJECT);
  ans = RS_datasetInstance(display->d);
  return(ans);
}

USER_OBJECT_
RS_axesValueMatrix(displayd *display) 
{
  /* currently only works for Tour2D */
  int n = display->t2d.nsubset, k, j;
  vartabled *vt;
  USER_OBJECT_ matrix;
  
  PROTECT(matrix = allocMatrix(REALSXP, n, 4));
  for (k = 0; k < n; k++) {
    j = display->t2d.subset_vars.els[k];
    vt = vartable_element_get (j, display->d);
    REAL(matrix)[k] = display->t2d.F.vals[0][j]; /* x coeff */
    REAL(matrix)[k+n] = display->t2d.F.vals[1][j]; /* y coeff */
    REAL(matrix)[k+2*n] = vt->lim.max - vt->lim.min; /* range */
    REAL(matrix)[k+3*n] = j+1; /* variable index */
  }
  
  UNPROTECT(1);
  
  return(matrix);
}

void
RS_INTERNAL_GGOBI(getTourVectorsFromMode)(displayd *display, ProjectionMode mode, 
  gdouble **x, gdouble **y)
{
  tour *t;
  switch(mode) {
    case TOUR1D:
      t = &display->t1d;
    break;
    case TOUR2D:
      t = &display->t2d;
    break;
    case TOUR2D3:
      t = &display->t2d3;
    break;
    case COTOUR:
      t = &display->tcorr1;
    break;
    default:
      g_critical("Specified mode '%s' is not a tour", GGOBI(getPModeName)(mode));
      return;
  }
  
  *x = t->F.vals[0];
  *y = NULL;
  if (mode == COTOUR)
    *y = display->tcorr2.F.vals[0];
  else if (mode != TOUR1D)
    *y = t->F.vals[1];
}

/* Expects a 2 column matrix with the X and Y coefficients for each var */
USER_OBJECT_
RS_GGOBI(setTourProjection)(USER_OBJECT_ s_display, USER_OBJECT_ s_mode_name,
  USER_OBJECT_ matrix)
{
  displayd *display = toDisplay(s_display);
  ProjectionMode mode = GGOBI(getPModeId)(asCString(s_mode_name));
  gint k, n;
  gdouble *x = NULL, *y = NULL;
  
  g_return_val_if_fail(GGOBI_IS_DISPLAY(display), NULL_USER_OBJECT);
  
  RS_INTERNAL_GGOBI(getTourVectorsFromMode)(display, mode, &x, &y);
  g_return_val_if_fail(x != NULL, NULL_USER_OBJECT);
  
  n = display->d->ncols;
  for (k = 0; k < n; k++) {
    x[k] = REAL(matrix)[k];
    if (y)
      y[k] = REAL(matrix)[k+n];
  }
  
  display_tailpipe (display, FULL, display->ggobi);
  varcircles_refresh (display->d, display->ggobi);
  
  return NULL_USER_OBJECT;
}

USER_OBJECT_
RS_GGOBI(getTourProjection)(USER_OBJECT_ s_display, USER_OBJECT_ s_mode_name)
{
	displayd *display = toDisplay(s_display);
  ProjectionMode mode = GGOBI(getPModeId)(asCString(s_mode_name));
  gint k, n;
  USER_OBJECT_ matrix;
  gdouble *x = NULL, *y = NULL;
  
  g_return_val_if_fail(GGOBI_IS_DISPLAY(display), NULL_USER_OBJECT);
  
  RS_INTERNAL_GGOBI(getTourVectorsFromMode)(display, mode, &x, &y);
  g_return_val_if_fail(x != NULL, NULL_USER_OBJECT);
  
  n = display->d->ncols;
  PROTECT(matrix = allocMatrix(REALSXP, n, 3));
  for (k = 0; k < n; k++) {
    vartabled *vt = vartable_element_get (k, display->d);
    REAL(matrix)[k] = x[k]; /* x coeff */
    if (y)
      REAL(matrix)[k+n] = y[k]; /* y coeff */
    else REAL(matrix)[k+n] = 0;
    REAL(matrix)[k+2*n] = vt->lim.max - vt->lim.min; /* range */
  }
  
  UNPROTECT(1);
  
  return matrix;
}

USER_OBJECT_
RS_GGOBI(getDisplayWidget)(USER_OBJECT_ r_display) {
  displayd *display  = (displayd *) toDisplay(r_display);
  GtkWidget *widget = GTK_WIDGET(display);
  if (GGOBI_IS_WINDOW_DISPLAY(display) &&
      GGOBI_WINDOW_DISPLAY(display)->useWindow)
    widget = gtk_bin_get_child(GTK_BIN(GGOBI_WINDOW_DISPLAY(display)->window));
  return toRPointer(widget, "GtkWidget");
}

USER_OBJECT_
RS_displayInstance(displayd *display)
{
  USER_OBJECT_ ans;
  ans = toRPointer(display, "GGobiDisplay");
  return(ans);
}

USER_OBJECT_ 
RS_GGOBI(closeDisplay)(USER_OBJECT_ ref, USER_OBJECT_ ggobiId)
{
  ggobid *gg = toGGobi(ggobiId);
  USER_OBJECT_ ans = NEW_LOGICAL(1);
  displayd *display;
  g_return_val_if_fail(GGOBI_IS_GGOBI(gg), NULL_USER_OBJECT);

  if(!gg)
    return(ans);

  display  = (displayd *) R_ExternalPtrAddr(ref); 
  display = ValidateDisplayRef(display, gg, false);
  if(display) {
    display_free(display, true, gg);
    LOGICAL_DATA(ans)[0] = TRUE;
    gdk_flush();
  } 

  return(ans);
}

displayd *
toDisplay(USER_OBJECT_ rdisplay)
{
  if(inherits(rdisplay, "GGobiDisplay")) {
    displayd *display = getPtrValue(rdisplay);
    g_return_val_if_fail(GGOBI_IS_DISPLAY(display), NULL);
    g_return_val_if_fail(ValidateGGobiRef(display->ggobi, false) != NULL, NULL);
    return(ValidateDisplayRef(display, display->ggobi, false));
  }
  g_critical("An R GGobi display object must inherit from class 'GGobiDisplay'");
  return(NULL);
}
