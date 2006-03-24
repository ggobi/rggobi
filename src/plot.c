#include "RSGGobi.h"
#include "RUtils.h"

#include <gtk/gtk.h>

#include "vars.h"
#include "parcoordsClass.h"
#include "scatterplotClass.h"

USER_OBJECT_
RS_GGOBI(createParcoordsPlot)(USER_OBJECT_ display, USER_OBJECT_ dims, USER_OBJECT_ ggobiId)
{
	ggobid *gg;
	displayd *dpy;
	splotd *plot;
	USER_OBJECT_ ans;
    
	dpy = GetDisplay(display, ggobiId, &gg);

	plot = ggobi_parcoords_splot_new(dpy, gg);

	ans = toRPointer(plot, "GGobiParcoordsPlot");

	return(ans);
}



USER_OBJECT_
RS_GGOBI(createScatterPlot)(USER_OBJECT_ dims, USER_OBJECT_ display, USER_OBJECT_ ggobiId)
{
	ggobid *gg;
	displayd *dpy, *odpy;
	splotd *plot = NULL;
	GGobiData *d;

	USER_OBJECT_ ans;
	gint els[] = {0,1};

	gg = GGOBI_GGOBI(toGGobi(ggobiId));

	if(GET_LENGTH(display)) {
  	   odpy = GetDisplay(display, ggobiId, &gg);
	   d = odpy->d;
	} else
	   d = (GGobiData *) gg->d->data;

	dpy = g_object_new(GGOBI_TYPE_EMBEDDED_DISPLAY, NULL);
	display_set_values(dpy, d, gg);
	display_add(dpy, gg);

	plot = g_object_new(GGOBI_TYPE_SCATTER_SPLOT, NULL);
	splot_init(plot, dpy, gg);

	createScatterplot(dpy, TRUE/*dpy->missing_p*/, plot, 2, els, dpy->d, gg);

/*	ans = R_MakeExternalPtr(plot, Rf_install("GtkScatterSPlot"), NULL_USER_OBJECT); */
	ans = toRPointer(dpy, "GGobiScatterSPlot");

	return(ans);
}

#if 0

splotd *RS_GGOBI(createSPlot)(displayd *display, GGobiData *data, SEXP desc, gint *dims, ggobid *gg, displayd** embedded);

displayd *RS_GGOBI(createEmbeddedPlot)(gint numRows, gint numCols, GGobiData *d, displayd *, ggobid *gg);
/*
  Lays out the plots given in the list `plotDescList' 
  in a grid/table format of dimension
 */
SEXP
RS_GGOBI(createPlots)(SEXP plotDescList, SEXP dims, SEXP cells, SEXP rdisplay, SEXP dataset)
{
  SEXP ans;
  int n = GET_LENGTH(plotDescList), i;
  int numRows, numCols;
  splotd *sp;
  displayd *display, *embeddedDisplay;
  GtkWidget *cell;
  GGobiData *d = NULL;
  ggobid *gg = NULL;
  gint *plotDims;
  gint left, top, bottom, right, ctr;
 
  d = GGOBI_DATA(toData(dataset));
  gg = d->gg;

  display = GetDisplay(rdisplay, gobiId, NULL);

  numRows = INTEGER_DATA(dims)[0];
  numCols = INTEGER_DATA(dims)[1];

  if(!display) {
    display = display_alloc_init (scatmat, false, d, gg);
    plotDims = createScatmatWindow(numRows, numCols, display, gg, true);
    display->splots = NULL;
  } else {
    plotDims = (gint *)g_malloc(2 * sizeof(int));
    if(display->splots  && display->splots->data) {
      splotd *tmpSP = (splotd *)display->splots->data;
      plotDims[0] = tmpSP->max.x;
      plotDims[1] = tmpSP->max.y;
    } else {
      plotDims[0] = plotDims[1] = 200;
    }
    d = display->d;
  }


  ctr = 0;
  for(i = 0; i < n ; i++, ctr+=4) {
    embeddedDisplay = (displayd *)NULL;
    sp = RS_GGOBI(createSPlot)(display, d, VECTOR_ELT(plotDescList, i), plotDims, gg, &embeddedDisplay);

    if(sp == (splotd *) NULL && embeddedDisplay == (displayd*) NULL)
      continue;
    if(sp != NULL) {
      display->splots = g_list_append(display->splots, (gpointer) sp);      
      cell = sp->da;
    } else {
        /* Register the embedded display with the ggobi. */
      GList *tmp = embeddedDisplay->splots;
      display_add(embeddedDisplay, gg);
      cell = embeddedDisplay->table;
        /* Add each of the splots to their outer parent. */
      while(tmp != (GList*) NULL) {
        display->splots = g_list_append(display->splots, (gpointer) tmp->data);      
        tmp = tmp->next;
      }
    }

    left = INTEGER_DATA(cells)[ctr];
    right = INTEGER_DATA(cells)[ctr+1];
    top = INTEGER_DATA(cells)[ctr+2];
    bottom = INTEGER_DATA(cells)[ctr+3];

    gtk_table_attach(GTK_TABLE (display->table), 
        cell, 
        left, right, top, bottom,
        (GtkAttachOptions) (GTK_SHRINK|GTK_FILL|GTK_EXPAND), 
        (GtkAttachOptions) (GTK_SHRINK|GTK_FILL|GTK_EXPAND),
        1, 1);
    gtk_widget_show (cell);
  }

/*
  display->scatmat_cols = NULL;
  for (j=0; j<scatmat_ncols; j++)
    display->scatmat_cols = g_list_append (display->scatmat_cols,
                                           GINT_TO_POINTER (j));
  display->scatmat_rows = NULL;
  for (i=0; i<scatmat_nrows; i++)
    display->scatmat_rows = g_list_append (display->scatmat_rows,
                                           GINT_TO_POINTER (i));
*/
  if(n > 0) {
      gtk_widget_show(display->table);
      if(GGOBI_IS_WINDOW_DISPLAY(display))
	  gtk_widget_show_all (GGOBI_WINDOW_DISPLAY(display)->window);
  } else
      display->scatmat_cols = NULL;

  g_free(plotDims);

  if(INTEGER_DATA(rdisplay)[0] < 0) {
    PROTECT(ans = NEW_INTEGER(1));
    INTEGER_DATA(ans)[0] = display_add(display, gg);
    UNPROTECT(1);
  } else {
    ans = rdisplay;
    INTEGER_DATA(ans)[0]++;
  }

  gdk_flush();

 return(ans);
}


splotd *
RS_GGOBI(createSPlot)(displayd *display, GGobiData *data, SEXP desc, gint *dims, ggobid *gg, displayd** embeddedDisplay)
{
 splotd *sp = (splotd*) NULL;

 if(GET_LENGTH(desc) == 0)
   return((splotd*)NULL);

 if(R_IS(desc, "ScatmatrixPlot")) {
    gint r,c;
    int i,j;
    gint width, height;
    r = c = GET_LENGTH(VECTOR_ELT(desc, 0));
    width = dims[0]/c;
    height = dims[1]/r;
    *embeddedDisplay = RS_GGOBI(createEmbeddedPlot)(r, c, data, display, gg);
    for(i = 0; i < r ;  i++) {
       for(j = 0; j < c ;  j++) {
       sp = splot_new (*embeddedDisplay, width, height, gg);
       sp->xyvars.x = INTEGER_DATA(VECTOR_ELT(desc,0))[i] - 1; 
       sp->xyvars.y = INTEGER_DATA(VECTOR_ELT(desc, 0))[j] - 1; 
       sp->p1dvar = (sp->xyvars.x == sp->xyvars.y) ? sp->xyvars.x : -1;
  
       (*embeddedDisplay)->splots = g_list_append ((*embeddedDisplay)->splots, (gpointer) sp);
  
       gtk_table_attach (GTK_TABLE ((*embeddedDisplay)->table), sp->da, i, i+1, j, j+1,
  	 (GtkAttachOptions) (GTK_SHRINK|GTK_FILL|GTK_EXPAND), 
  	 (GtkAttachOptions) (GTK_SHRINK|GTK_FILL|GTK_EXPAND),
  	 1, 1);
       gtk_widget_show (sp->da);          
      }
    }
    return( (splotd *) NULL);
 } else if(R_IS(desc, "MultipleParallelCoordinates")) {
    gint r,c;
    int j;
    gint width, height;
    c = GET_LENGTH(VECTOR_ELT(desc, 0));
    r = 1;
    width = dims[0]/c;
    height = dims[1]/r;
    *embeddedDisplay = RS_GGOBI(createEmbeddedPlot)(r, c, data, display, gg);
    (*embeddedDisplay)->displaytype = parcoords;
    (*embeddedDisplay)->p1d_orientation = VERTICAL;
    (*embeddedDisplay)->cpanel.p1d.type = DOTPLOT;
     for(j = 0; j < c ;  j++) {
       sp = splot_new (*embeddedDisplay, width, height, gg);
       sp->xyvars.x = INTEGER_DATA(VECTOR_ELT(desc, 0))[j] - 1;
       sp->p1dvar = sp->xyvars.x;

       (*embeddedDisplay)->splots = g_list_append ((*embeddedDisplay)->splots, (gpointer) sp);
  
       gtk_table_attach (GTK_TABLE ((*embeddedDisplay)->table), sp->da, j, j+1, 0, 1,
  	 (GtkAttachOptions) (GTK_SHRINK|GTK_FILL|GTK_EXPAND), 
  	 (GtkAttachOptions) (GTK_SHRINK|GTK_FILL|GTK_EXPAND),
  	 1, 1);
       gtk_widget_show (sp->da);          
    }
    return( (splotd *) NULL);

 } else if(R_IS(desc, "TimeSeriesPlot")) {
   PROBLEM "Creating TimeSeriesPlot not handled yet!"
     WARN;
   return(NULL);
 } 


  sp = splot_new (display, dims[0], dims[1], gg);
  if(R_IS(desc, "ScatterPlot")) {
    USER_OBJECT_ varIds =  VECTOR_ELT(desc, 0);
     sp->xyvars.x = INTEGER_DATA(varIds)[0] - 1;
     sp->xyvars.y = INTEGER_DATA(varIds)[1] - 1;
     sp->p1dvar = -1;
  } else if(R_IS(desc, "ParallelCoordinates")) {
    USER_OBJECT_ varIds =  VECTOR_ELT(desc, 0);
     sp->p1dvar = INTEGER_DATA(varIds)[0];
  } else if(R_IS(desc, "GGobiAsh")) {
    USER_OBJECT_ varIds =  VECTOR_ELT(desc, 0);
     sp->p1dvar = INTEGER_DATA(varIds)[0];
  } 

 return(sp);
}


displayd *
RS_GGOBI(createEmbeddedPlot)(gint numRows, gint numCols, GGobiData *d, displayd *owner, ggobid *gg)
{
  gint *plotDims;
  displayd *display;
    display = display_alloc_init(scatmat, false, d, gg);
    display->d = d;
/*XXX Handle the creation of a GtkGGobiDisplay, not a GtkGGobiWindowDisplay. */
    plotDims = createScatmatWindow(numRows, numCols, display, gg, false);
    display->splots = NULL;
    display->displaytype = scatmat;
 return(display);
}



/*
  Allows the R user to set the variables within a given plot.
  Returns the previous settings in place of those that were
  given by the user. This allows the values to be restored
  easily at a later time.
 */
USER_OBJECT_
RS_GGOBI(setPlotVariables)(USER_OBJECT_ varIds, USER_OBJECT_ dpy,
 			    USER_OBJECT_ plotId, USER_OBJECT_ ggobiId)
{
  displayd *display;
  splotd *sp;
  long oldx, oldy;
  USER_OBJECT_ ans;
  int n;

  display = GetDisplay(dpy, ggobiId, NULL);
  if(display == NULL)
    return(NULL_USER_OBJECT);
  
  sp = (splotd*)g_list_nth_data(display->splots, INTEGER_DATA(plotId)[0]);
  if(sp == NULL) {
    PROBLEM "No such plot %d within this ggobi display" ,
                   (int) INTEGER_DATA(plotId)[0]
    ERROR;
  }

  switch(display->displaytype) {
   case scatterplot:
   case scatmat:
     oldx = sp->xyvars.x + 1;
     oldy = sp->xyvars.y + 1;
     sp->xyvars.x = INTEGER_DATA(varIds)[0];
     sp->xyvars.y = INTEGER_DATA(varIds)[1];
     n = 2;
     break;
   case parcoords:
     oldx = sp->p1dvar +1;
     sp->p1dvar = INTEGER_DATA(varIds)[0];
     n = 1;
     break;
    default:
  }

  ans = NEW_INTEGER(n);
  INTEGER_DATA(ans)[0] = oldx;
   
  if(n > 1) {
    INTEGER_DATA(ans)[1] = oldy;
  }

 return(ans);
}

#endif

USER_OBJECT_
RS_GGOBI(setPlotRange)(USER_OBJECT_ x, USER_OBJECT_ y, USER_OBJECT_ plot,
                         USER_OBJECT_ rdisplay, USER_OBJECT_ ggobiId)
{
  ggobid *gg;
  displayd *display;
  USER_OBJECT_ ans;

  display = GetDisplay(rdisplay, ggobiId, &gg);
  if(!display)
    return(NULL_USER_OBJECT);

  GGOBI(setPlotRange)(NUMERIC_DATA(x), NUMERIC_DATA(y), INTEGER_DATA(plot)[0]-1, display, false, gg);

  gdk_flush();

  ans = NEW_LOGICAL(1);
  LOGICAL_DATA(ans)[0] = TRUE;
  return(ans);
}
