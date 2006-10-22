#include "RSGGobi.h"
#include "vars.h"

USER_OBJECT_
RS_GGOBI(getSymbolicEdges)(USER_OBJECT_ edgesetId)
{
 GGobiData *e = toData(edgesetId);
 USER_OBJECT_ ans, dim;
 gint i, ctr, n;
 g_return_val_if_fail(GGOBI_IS_DATA(e), NULL_USER_OBJECT);
 
 n = e->edge.n;

 if(!e) {
   PROBLEM "Invalid ggobi dataset identifier(s)"
   ERROR;
 }

 PROTECT(ans = NEW_CHARACTER(n * 2));

 ctr = 0;
 for(i = 0; i < n; i++) {
   SET_STRING_ELT(ans, ctr, COPY_TO_USER_STRING(e->edge.sym_endpoints[i].a));
   SET_STRING_ELT(ans, ctr+n, COPY_TO_USER_STRING(e->edge.sym_endpoints[i].b));
   ctr++;
 }

 /* Now set the dim() on this vector to make it a matrix. 
    The S code will put the dimnames on it.
  */
 PROTECT(dim = NEW_INTEGER(2));
  INTEGER_DATA(dim)[0] = n;
  INTEGER_DATA(dim)[1] = 2;
  SET_DIM(ans, dim);
 UNPROTECT(2);

 return(ans); 
}


USER_OBJECT_
RS_GGOBI(getConnectedEdges)(USER_OBJECT_ edgesetId, USER_OBJECT_ datasetId)
{
 GGobiData *d = toData(datasetId);
 GGobiData *e = toData(edgesetId);
 gint ctr; 
 USER_OBJECT_ ans, dim;
 gint i, n;
 endpointsd *endpoints;

 g_return_val_if_fail(GGOBI_IS_DATA(d), NULL_USER_OBJECT);
 g_return_val_if_fail(GGOBI_IS_DATA(e), NULL_USER_OBJECT);
 
 n = e->edge.n;

 endpoints = resolveEdgePoints(e, d);
 if(!endpoints) 
    return(NULL_USER_OBJECT);

 PROTECT(ans = NEW_INTEGER(n * 2));

 ctr = 0;
 for(i = 0; i < n; i++) {
   INTEGER_DATA(ans)[ctr] = endpoints[i].a;  
   INTEGER_DATA(ans)[ctr+n] = endpoints[i].b; 
   ctr++;
 }

 /* Now set the dim() on this vector to make it a matrix. 
    The S code will put the dimnames on it.
  */
 PROTECT(dim = NEW_INTEGER(2));
  INTEGER_DATA(dim)[0] = n;
  INTEGER_DATA(dim)[1] = 2;
  SET_DIM(ans, dim);
 UNPROTECT(2);

 return(ans);
}

USER_OBJECT_
RS_GGOBI(createEdgeDataset)(USER_OBJECT_ numPoints, USER_OBJECT_ sname, USER_OBJECT_ ggobiId)
{
   ggobid *gg = toGGobi(ggobiId);
   GGobiData *d;
   g_return_val_if_fail(GGOBI_IS_GGOBI(gg), NULL_USER_OBJECT);

   if(!gg) {
      PROBLEM "Invalid reference to GGobi instance"
      ERROR;
   }

   d = ggobi_data_new(INTEGER_DATA(numPoints)[0], 0);
   if(!d) {
      PROBLEM "Invalid reference to GGobi instance"
      ERROR;
   }

   GGobi_setDataName(CHAR_DEREF(STRING_ELT(sname, 0)), d);
   pipeline_init(d, gg);

   return(RS_datasetInstance(d));
}

USER_OBJECT_
RS_GGOBI(setEdges)(USER_OBJECT_ x, USER_OBJECT_ y, USER_OBJECT_ append, USER_OBJECT_ datasetId)
{
  USER_OBJECT_ ans = NULL_USER_OBJECT;
  ggobid *gg;
  GGobiData *e;
  int num = GET_LENGTH(x);
  gint i;

  e = toData(datasetId);
  g_return_val_if_fail(GGOBI_IS_DATA(e), NULL_USER_OBJECT);
  gg = e->gg;
 
  if(!e) 
    return(ans);

  if(LOGICAL_DATA(append)[0] == FALSE) {
     g_free(e->edge.sym_endpoints);
     e->edge.n = 0;
  }

  if(num > 0) {
    
   edges_alloc (e->edge.n + num, e);

   for(i = 0; i < num; i++) {
     e->edge.sym_endpoints[i].a = g_strdup(CHAR_DEREF(STRING_ELT(x, i)));
     e->edge.sym_endpoints[i].b = g_strdup(CHAR_DEREF(STRING_ELT(y, i)));
     e->edge.sym_endpoints[i].jpartner = -1;
   }
  }

  unresolveAllEdgePoints(e);
  if(gg->current_display) {
    edgeset_add(gg->current_display);
    displays_plot(NULL, FULL, gg);
  }
  gdk_flush();

  ans = RS_datasetInstance(e);
  
  return(ans);
}


/**
  Newer, but old,  version for the edges interface using multiple (at least 2) datasets for 
  the point set and the edge set.

  This sets the connections between nodes.
  We can either replace the existing edges or append to that collection.
  The source and destination variables identify the nodes in pairs.

 */
USER_OBJECT_
RS_GGOBI(setEdgeIndices)(USER_OBJECT_ x, USER_OBJECT_ y, USER_OBJECT_ append,
                          USER_OBJECT_ datasetId)
{
  USER_OBJECT_ ans = NULL_USER_OBJECT;
  GGobiData *e;
  int num = GET_LENGTH(x);
  gint i;
  e = toData(datasetId);
  g_return_val_if_fail(GGOBI_IS_DATA(e), NULL_USER_OBJECT);

  if(!e) 
    return(ans);

  if(LOGICAL_DATA(append)[0] == FALSE) {
     edges_free (e, e->gg);
     e->edge.n = 0;
#ifdef OLD_STYLE_IDS
     e->edge.old_endpoints = NULL;
#endif
  }

  edges_alloc (e->edge.n + num, e);

  for(i = 0; i < num; i++) {
#ifdef OBSOLETE_EDGE_CODE
     /* Without the update argument, this would be a very slow way of doing this.
   	We would end up re-allocating the edges_endpoint
   	each time. However, pre-allocating it to *num
   	means we have to tell setObservationEdge something
   	to avoid it reallocating space itself.
      */
     GGOBI(setObservationEdge)(INTEGER_DATA(x)[i], INTEGER_DATA(y)[i], e, e->gg, false);
#endif
#ifdef OLD_STYLE_IDS
     e->edge.old_endpoints[i].a = INTEGER_DATA(x)[i];
     e->edge.old_endpoints[i].b = INTEGER_DATA(y)[i];
     e->edge.old_endpoints[i].jpartner = -1;
#endif
  }

#ifdef OLD_STYLE_IDS
  setOldEdgePartners(e->edge.old_endpoints, e->edge.n);
#endif

  edgeset_add(e->gg->current_display);

  displays_plot(NULL, FULL, e->gg);
  gdk_flush();

  ans = RS_datasetInstance(e);

  return(ans);
}


USER_OBJECT_
RS_GGOBI(setDisplayEdges)(USER_OBJECT_ dpys, USER_OBJECT_ edgeData, USER_OBJECT_ directed, USER_OBJECT_ On, USER_OBJECT_ ggobiId)
{
    int i, n;
    USER_OBJECT_ ans;
    displayd *gdpy;
    GGobiData *edge = NULL;
    ggobid *gg = toGGobi(ggobiId);
    g_return_val_if_fail(GGOBI_IS_GGOBI(gg), NULL_USER_OBJECT);

    if (asCLogical(On)) {
      edge = toData(edgeData);
      g_return_val_if_fail(GGOBI_IS_DATA(edge), NULL_USER_OBJECT);
    }
    
    n = GET_LENGTH(dpys);                        
    PROTECT(ans = NEW_LIST(n));
    for(i = 0; i < n ; i++) {
      GGobiData *old;
      gdpy = toDisplay(VECTOR_ELT(dpys, i));
      g_return_val_if_fail(GGOBI_IS_DISPLAY(gdpy), NULL_USER_OBJECT);
      gdpy->options.edges_undirected_show_p = LOGICAL_DATA(On)[0];
	    if(GET_LENGTH(directed))
        gdpy->options.edges_arrowheads_show_p = LOGICAL_DATA(directed)[0];
	    if(edge) {
        old = setDisplayEdge(gdpy, edge);
        if(old) {
          SET_VECTOR_ELT(ans, i, RS_datasetInstance(old));
	      }
	    }
    }

    UNPROTECT(1);
    displays_plot(NULL, FULL, gg);
    gdk_flush();
    return(ans);
}
