#include "RSGGobi.h"
#include "RUtils.h"


/*
   Adds a dataset using data from the specified file
 */
USER_OBJECT_
RS_GGOBI(setFile)(USER_OBJECT_ fileName, USER_OBJECT_ smode, USER_OBJECT_ add, USER_OBJECT_ gobiId)
{
 DataMode mode;
 ggobid *gg = toGGobi(gobiId);
 USER_OBJECT_ ans = NEW_INTEGER(1);
 const gchar *modeName = NULL;
 g_return_val_if_fail(GGOBI_IS_GGOBI(gg), NULL_USER_OBJECT);

 if(IS_CHARACTER(smode))
	 modeName = CHAR_DEREF(STRING_ELT(smode, 0));
 else if(IS_INTEGER(smode)) {
  mode = INTEGER_DATA(smode)[0];
  if(mode < 0)
    mode = unknown_data;
 }

  if(fileset_read_init(CHAR_DEREF(STRING_ELT(fileName, 0)), modeName, NULL, gg)) {
    INTEGER_DATA(ans)[0] = g_slist_length(gg->d)-1;
    display_menu_build(gg);
  } else
    INTEGER_DATA(ans)[0] = -1;

 return(ans);
}

USER_OBJECT_
RS_GGOBI(getNumDatasets)(USER_OBJECT_ gobiID)
{
 ggobid *gg  = toGGobi(gobiID);
 USER_OBJECT_  ans = NEW_INTEGER(1);
    if(gg != NULL)
      INTEGER_DATA(ans)[0] = g_slist_length(gg->d);

   return(ans);
}

/*
  Returns the names of the datasets within the specified
  ggobid object.
 */
USER_OBJECT_
RS_GGOBI(getDatasetNames)(USER_OBJECT_ gobiId)
{
  USER_OBJECT_ ans;
  ggobid *gg = toGGobi(gobiId);
  int i;
  GGobiData *d;
  GSList *tmp;
  int n;
  
  g_return_val_if_fail(GGOBI_IS_GGOBI(gg), NULL_USER_OBJECT);  
  
  tmp = gg->d;
  n = g_slist_length(gg->d);
  
  PROTECT(ans = NEW_CHARACTER(n));
  for(i = 0; i < n ; i++) {
    d =(GGobiData *) tmp->data;
    SET_STRING_ELT(ans, i, COPY_TO_USER_STRING(d->name));
    tmp = tmp->next;
  }

  UNPROTECT(1);
  return(ans);
}

USER_OBJECT_
RS_GGOBI(getData)(USER_OBJECT_ datasetId)
{
 GGobiData *d;
 USER_OBJECT_ ans, colnames, el;
 gint nr, nc;
 gint j, m;
 vartabled *vt;

 ans = NULL_USER_OBJECT;

 d = toData(datasetId);
  g_return_val_if_fail(GGOBI_IS_DATA(d), NULL_USER_OBJECT);
 if (!d) return(NULL_USER_OBJECT);

 nr = d->nrows;
 nc = d->ncols;

 if(nr == 0 || nc == 0)
   return(NULL_USER_OBJECT);
 
 PROTECT(colnames = NEW_CHARACTER(nc));
 PROTECT(ans = NEW_LIST(nc));

 for(j = 0; j < nc; j++) {
    vt = vartable_element_get(j, d);
    SET_STRING_ELT(colnames, j, COPY_TO_USER_STRING(ggobi_data_get_col_name(d, j)));
    
    PROTECT(el = NEW_NUMERIC(nr));
    for (m = 0; m < nr; m++) {
      if(ggobi_data_is_missing(d, m, j))
        NUMERIC_DATA(el)[m] = NA_REAL;
      else
        NUMERIC_DATA(el)[m] = d->raw.vals[m][j];
    }
    if(vt->vartype == categorical) {
      PROTECT(el = createFactor(el, vt, d, j));
    }
    SET_VECTOR_ELT(ans, j, el);
    UNPROTECT(1 + (vt->vartype == categorical));
 }

 SET_NAMES(ans, colnames);

 UNPROTECT(2);

 return(ans);
}


USER_OBJECT_
RS_GGOBI(getDataset)(USER_OBJECT_ which, USER_OBJECT_ gobiID)
{
  int i, n;
  ggobid *gg  = toGGobi(gobiID);
  USER_OBJECT_ ans = NULL_USER_OBJECT;
  if(gg == NULL)
    return(ans);

  n = GET_LENGTH(which);
  PROTECT(ans = NEW_LIST(n)); 
  for(i = 0; i < n; i++) {
    int val = INTEGER_DATA(which)[i];
    GGobiData *d = (GGobiData *) g_slist_nth_data(gg->d, val);
    if(d) {
      SET_VECTOR_ELT(ans, i, RS_datasetInstance(d));
    }
  }
  UNPROTECT(1);

  return(ans);
}
