#include "RSGGobi.h"

USER_OBJECT_
RS_datasetInstance(GGobiData *d) 
{
  USER_OBJECT_ ans;
  
  ans = toRPointer(d, "GGobiData");
 
  return(ans);
}

GGobiData *
toData(USER_OBJECT_ d)
{
  if(inherits(d, "GGobiData")) {
    GGobiData *data = getPtrValue(d);
    g_return_val_if_fail(GGOBI_IS_DATA(data), NULL);
    g_return_val_if_fail(ValidateGGobiRef(data->gg, false) != NULL, NULL);
    return(ValidateDatadRef(data, data->gg, false));
  }
  g_critical("An R GGobi dataset object must inherit from GGobiData");
  return(NULL);
}

USER_OBJECT_
RS_GGOBI(getVariableNames)(USER_OBJECT_ transformed, USER_OBJECT_ datasetId)
{
  GGobiData *d = NULL;

  d = toData(datasetId);
  g_return_val_if_fail(GGOBI_IS_DATA(d), NULL_USER_OBJECT);

  if(d)
    return(RS_INTERNAL_GGOBI(getVariableNames)(d));
  else
    return(NULL_USER_OBJECT);
}

USER_OBJECT_
RS_INTERNAL_GGOBI(getVariableNames)(GGobiData *d)
{
  USER_OBJECT_ ans;
  gint n, i;

  n =  d->ncols;
  PROTECT(ans = NEW_CHARACTER(n));

  for (i = 0; i < n ; i++) {
    SET_STRING_ELT(ans, i, COPY_TO_USER_STRING(ggobi_data_get_col_name(d, i)));
  }

  UNPROTECT(1);

  return(ans);
}

USER_OBJECT_
RS_GGOBI(getCaseIds)(USER_OBJECT_ datasetId)
{
 GGobiData *d;
 USER_OBJECT_ ans;
 int n, m;

 d = toData(datasetId);
  g_return_val_if_fail(GGOBI_IS_DATA(d), NULL_USER_OBJECT);

 if(!d) {
  PROBLEM "No such dataset"
  ERROR;
 }

 if(!d->rowIds) {
    return(NULL_USER_OBJECT);
 }


 n = d->nrows;

 PROTECT(ans = NEW_CHARACTER(n));
 for(m = 0; m < n ; m++) {
    if(d->rowIds[m])
     SET_STRING_ELT(ans, m, COPY_TO_USER_STRING(d->rowIds[m]));
 }

 UNPROTECT(1);

 return(ans);
}

/* returns selected indices, labeled by rownames */
USER_OBJECT_
RS_GGOBI(getSelectedIndices)(USER_OBJECT_ datasetId)
{
 GGobiData *d;
 USER_OBJECT_ ans, names;

  d = toData(datasetId);
  if(d) {
    gint nr, i, m, ctr;
   gchar *name;
       nr = d->npts_under_brush;
       
       if (nr < 1)
      	 return(NULL_USER_OBJECT);
      	
       PROTECT(ans = NEW_INTEGER(nr));
       PROTECT(names = NEW_CHARACTER(nr));
       ctr = 0;
       for(m = 0 ; m < d->nrows ; m++) {
         i = d->rows_in_plot.els[m];
      	 if (d->pts_under_brush.els[i]) {
           INTEGER_DATA(ans)[ctr] = i + 1; /* Make these 1 based */
           name = (gchar *) g_array_index (d->rowlab, gchar *, i);
           if(name && name[0]) 
             SET_STRING_ELT(names, ctr, COPY_TO_USER_STRING (name));
           ctr++;
      	 }
       }
       
       SET_NAMES(ans, names);
       UNPROTECT(2);
   } else 
     ans = NULL_USER_OBJECT;

 return (ans);
}

USER_OBJECT_
RS_GGOBI(getRowNames)(USER_OBJECT_ data)
{
  USER_OBJECT_ ans = NULL_USER_OBJECT;
  gint nr, m;
  GGobiData *d;
  
 d = toData(data);
  g_return_val_if_fail(GGOBI_IS_DATA(d), NULL_USER_OBJECT);

  if(d) {
    nr = d->nrows;
    PROTECT(ans = NEW_CHARACTER(nr));

    for (m = 0 ; m < nr ; m++) {
      SET_STRING_ELT(ans, m,
      COPY_TO_USER_STRING(g_array_index (d->rowlab, gchar *, m)));
    }
    UNPROTECT(1);
  }

  return (ans);
}

USER_OBJECT_
RS_GGOBI(getRowsInPlot)(USER_OBJECT_ datasetId)
{
  USER_OBJECT_ ans = NULL_USER_OBJECT;
  GGobiData *d;
 
  d = toData(datasetId);
  g_return_val_if_fail(GGOBI_IS_DATA(d), NULL_USER_OBJECT);

  if(d) {
    gint i;

    PROTECT(ans = NEW_INTEGER(d->nrows_in_plot));

    for(i = 0; i < d->nrows_in_plot ; i++) {
      INTEGER_DATA(ans)[i] = d->rows_in_plot.els[i];
    }

    UNPROTECT(1);
  }

  return (ans);
}

/* shouldn't these be internal? */

USER_OBJECT_
RS_INTERNAL_GGOBI(getDataAttribute)(vector_b v)
{
  USER_OBJECT_ ans = NULL_USER_OBJECT;
 
  gint i;
  PROTECT(ans = NEW_LOGICAL(v.nels));

  for(i = 0; i < v.nels; i++) {
	  LOGICAL_DATA(ans)[i] = v.els[i];
  }

  UNPROTECT(1);

  return (ans);
}


void
RS_INTERNAL_GGOBI(setDataAttribute)(vector_b *v, USER_OBJECT_ vals, GGobiData *d)
{
 
  if(v) {
    gint i, n;
    n = GET_LENGTH(vals);
    if(n != d->nrows) {
	 PROBLEM "number of values must be the same as the number of records in the GGobi dataset"
	 ERROR;
    }
    vectorb_realloc(v, n);
    for(i = 0; i < n; i++) {
       v->els[i] = LOGICAL_DATA(vals)[i];
    }
  }
}


USER_OBJECT_
RS_GGOBI(getSampledIndices)(USER_OBJECT_ datasetId)
{
  GGobiData *d = toData(datasetId);
  g_return_val_if_fail(GGOBI_IS_DATA(d), NULL_USER_OBJECT);
  return(RS_INTERNAL_GGOBI(getDataAttribute)(d->sampled));
}

USER_OBJECT_
RS_GGOBI(setSampledIndices)(USER_OBJECT_ vals, USER_OBJECT_ datasetId)
{
  GGobiData *d = toData(datasetId);
  g_return_val_if_fail(GGOBI_IS_DATA(d), NULL_USER_OBJECT);
  RS_INTERNAL_GGOBI(setDataAttribute)(&(d->sampled), vals, d);
  rows_in_plot_set(d, d->gg);
  return(NULL_USER_OBJECT);
}

USER_OBJECT_
RS_GGOBI(getExcludedIndices)(USER_OBJECT_ datasetId)
{
  GGobiData *d = toData(datasetId);
  g_return_val_if_fail(GGOBI_IS_DATA(d), NULL_USER_OBJECT);
  return(RS_INTERNAL_GGOBI(getDataAttribute)(d->excluded));
}

USER_OBJECT_
RS_GGOBI(setExcludedIndices)(USER_OBJECT_ vals, USER_OBJECT_ datasetId)
{
  GGobiData *d = toData(datasetId);
  ggobid *gg = d->gg;
  g_return_val_if_fail(GGOBI_IS_DATA(d), NULL_USER_OBJECT);
  RS_INTERNAL_GGOBI(setDataAttribute)(&(d->excluded), vals, d);
  subset_apply(d, gg);
  GGOBI(update_data)(d, gg);
  displays_tailpipe (FULL, gg);
  return(NULL_USER_OBJECT);
}

USER_OBJECT_
RS_GGOBI(setCasesHidden)(USER_OBJECT_ vals, USER_OBJECT_ which, USER_OBJECT_ datasetId)
{
  gint i;
  ggobid *gg;
  GGobiData *d;
  USER_OBJECT_ ans = NEW_LOGICAL(1);
  d = toData(datasetId);
  g_return_val_if_fail(GGOBI_IS_DATA(d), NULL_USER_OBJECT);
  gg = d->gg;
  if(d) {
    int num = GET_LENGTH(vals);
    for (i = 0; i < num; i++) {
      GGOBI(setCaseHidden)(INTEGER_DATA(which)[i], LOGICAL_DATA(vals)[i], d, gg);
    }

   displays_plot (NULL, FULL, gg);
   gdk_flush();
   LOGICAL_DATA(ans)[0] = TRUE;
  }

  return(ans);
}



USER_OBJECT_
RS_GGOBI(getCasesHidden)(USER_OBJECT_ datasetId)
{
  USER_OBJECT_ ans = NULL_USER_OBJECT;
  ggobid *gg;
  GGobiData *d;
  d = toData(datasetId);
  g_return_val_if_fail(GGOBI_IS_DATA(d), NULL_USER_OBJECT);
  gg = d->gg;
    if(d) {
      gint num = d->nrows, m;

      PROTECT(ans = NEW_LOGICAL(num));
      for(m = 0; m < num; m++) {
        LOGICAL_DATA(ans)[m] = GGOBI(getCaseHidden)(m, d, gg);
      }

      UNPROTECT(1);
    }

  return(ans);
}

/*
 This allows us to add a variable to the existing dataset.
 If there are no variables currently in the dataset, the first
 one added will induce a call to GGOBI(setData) and
 datad_init() which will create the relevant tab in the
 notebook. However, it will also slight mess up with the
 addition of the variables into the control panel
 (e.g. create X, Y and Z buttons for no good reason.)

 We might want to inhibit this and wait until the end.
*/
USER_OBJECT_
RS_GGOBI(addVariable)(USER_OBJECT_ vals, USER_OBJECT_ name, USER_OBJECT_ levels,
		      USER_OBJECT_ values, USER_OBJECT_ datasetId)
{
  GGobiData *d;
  ggobid *gg;
  USER_OBJECT_ ans = NULL_USER_OBJECT;
  d = toData(datasetId);
  g_return_val_if_fail(GGOBI_IS_DATA(d), NULL_USER_OBJECT);
  gg = d->gg;


  if(d) {
    int i;
    PROTECT(ans = NEW_INTEGER(1));
    if(GET_LENGTH(levels)) {
     USER_OBJECT_ names;
     int n = GET_LENGTH(levels);
     char **levelValues;

     names = GET_NAMES(levels);
     levelValues = (char **) S_alloc(n, sizeof(char *));
     for(i = 0; i < n ; i++) {
         levelValues[i] = CHAR_DEREF(STRING_ELT(names, i));
     }

     INTEGER_DATA(ans)[0] = GGOBI(addCategoricalVariable)(NUMERIC_DATA(vals),
							      GET_LENGTH(vals), 
							      CHAR_DEREF(STRING_ELT(name,0)), 
							      levelValues, 
 	 						      INTEGER_DATA(values), INTEGER_DATA(levels),
							      n,
							      true, d, gg);
    } else {
         INTEGER_DATA(ans)[0] = GGOBI(addVariable)(NUMERIC_DATA(vals),
					       GET_LENGTH(vals), 
					       CHAR_DEREF(STRING_ELT(name,0)), 
					       true, d, gg);
    }
    UNPROTECT(1);
  } else {
    PROBLEM "Cannot resolve dataset in GGobi"
    ERROR;
  }

  return(ans);
}

USER_OBJECT_
RS_GGOBI(datad_init)(USER_OBJECT_ cleanup, USER_OBJECT_ datasetId)
{
  GGobiData *d;
  ggobid *gg;

  d = toData(datasetId);
  g_return_val_if_fail(GGOBI_IS_DATA(d), NULL_USER_OBJECT);
  gg = d->gg;

  if(d) {
     datad_init(d, gg, LOGICAL_DATA(cleanup)[0]);
  }

  return(NULL_USER_OBJECT);
}

USER_OBJECT_
RS_GGOBI(setVariableValues)(USER_OBJECT_ vals, USER_OBJECT_ rowIds,
                             USER_OBJECT_ colId, USER_OBJECT_ update, 
                               USER_OBJECT_ datasetId)
{
  ggobid *gg;
  GGobiData *d;
  gint i, num, var, row;
  
  d = toData(datasetId);
  g_return_val_if_fail(GGOBI_IS_DATA(d), NULL_USER_OBJECT);
  gg = d->gg;

  num = GET_LENGTH(rowIds);
  var = INTEGER_DATA(colId)[0];
  for (i=0;i < num; i++) {
    row = INTEGER_DATA(rowIds)[i];
    d->raw.vals[row][var] = d->tform.vals[row][var] = NUMERIC_DATA(vals)[i];
  }

/*  splot_reverse_pipeline (gg->current_splot, row, &gg->movepts.eps, true, true, gg); */
  /*
   * This call does too much. Need to find a routine that does
   * just what is needed.

  pipeline_init (d, gg);
  displays_plot(NULL, FULL, gg);

  */
  if(LOGICAL_DATA(update)[0]) {
      tform_to_world (d, gg);
      displays_tailpipe (FULL, gg);
      gdk_flush();
  }

  return(NULL_USER_OBJECT);
}

USER_OBJECT_
RS_GGOBI(setVariableNames)(USER_OBJECT_ vars, USER_OBJECT_ names, USER_OBJECT_ datasetId)
{
  gint i, which;
  ggobid *gg;
  GGobiData *d;
  int num = GET_LENGTH(vars);
  gchar **curNames; 
  USER_OBJECT_ ans;

  d = toData(datasetId);
  g_return_val_if_fail(GGOBI_IS_DATA(d), NULL_USER_OBJECT);
  gg = d->gg;

  PROTECT(ans = NEW_CHARACTER(num));
  curNames = GGOBI(getVariableNames)(false, d, gg);
  for (i = 0; i < num; i++) {
    which = INTEGER_DATA(vars)[i];
    SET_STRING_ELT(ans, i, COPY_TO_USER_STRING(curNames[which]));
    GGOBI(setVariableName)(which, CHAR_DEREF(STRING_ELT(names,i)), false, d, gg);
    GGOBI(setVariableName)(which, CHAR_DEREF(STRING_ELT(names,i)), true, d, gg);
  }

  UNPROTECT(1);
  return(ans);
}

USER_OBJECT_
RS_GGOBI(varpanel_populate)(USER_OBJECT_ datasetId)
{
  GGobiData *d;
  ggobid *gg;

  d = toData(datasetId);
  g_return_val_if_fail(GGOBI_IS_DATA(d), NULL_USER_OBJECT);
  gg = d->gg;

  if(d) {
     varpanel_populate(d, gg);
  }

  return(NULL_USER_OBJECT);
}

USER_OBJECT_
RS_GGOBI(getVariable)(int which, GGobiData *d)
{
  int nr = d->nrows_in_plot, i, m;
  float f;
    USER_OBJECT_ ans;
    vartabled *vt;
     vt = vartable_element_get (which, d);

     if(vt->vartype == categorical) {
       PROTECT(ans = NEW_INTEGER(nr));
     } else {
       PROTECT(ans = NEW_NUMERIC(nr));
     }
    for(m = 0; m < nr; m++) {
      i = d->rows_in_plot.els[m];
      f = d->raw.vals[i][which];
      if(vt->vartype == categorical)
	    INTEGER_DATA(ans)[m] = f;
      else
	    NUMERIC_DATA(ans)[m] = f;
    }
    if(vt->vartype == categorical) {
        USER_OBJECT_ levs;
        nr = vt->nlevels;
        PROTECT(levs = NEW_CHARACTER(nr));
	for(m = 0; m < nr; m++) {
          gchar* tmp;
          i = d->rows_in_plot.els[m];
          tmp = (gchar *) vt->level_names[i];
	  SET_STRING_ELT(levs, m, COPY_TO_USER_STRING(tmp));
	}
        SET_LEVELS(ans, levs);
        PROTECT(levs = NEW_CHARACTER(1));
        SET_STRING_ELT(levs, 0, COPY_TO_USER_STRING("factor"));
        SET_CLASS(ans, levs);
	UNPROTECT(2);
    }

    UNPROTECT(1);
    return(ans);
}
USER_OBJECT_
RS_GGOBI(getVariables)(USER_OBJECT_ which, USER_OBJECT_ datasetId)
{

  GGobiData *d = NULL;
  USER_OBJECT_ ans = NULL_USER_OBJECT;

  d = toData(datasetId);
  g_return_val_if_fail(GGOBI_IS_DATA(d), NULL_USER_OBJECT);

  if(d) {
      int n = GET_LENGTH(which), i;
 
      PROTECT(ans = NEW_LIST(n));

      for(i = 0; i < n; i++) {
	  SET_VECTOR_ELT(ans, i, RS_GGOBI(getVariable)(INTEGER_DATA(which)[i]-1, d));
      }
      UNPROTECT(1);
  } else {
   PROBLEM "Cannot identify dataset in GGobi"
   ERROR;
  }

  return(ans);
}

USER_OBJECT_
RS_GGOBI(createEmptyData)(USER_OBJECT_ snrow, USER_OBJECT_ name, USER_OBJECT_ description, USER_OBJECT_ gobiId)
{
  ggobid *gg = toGGobi(gobiId);
  GGobiData *d;
  g_return_val_if_fail(GGOBI_IS_GGOBI(gg), NULL_USER_OBJECT);

  if((gg = ValidateGGobiRef(gg, false)) == NULL) {
    return(R_NilValue);
  }

  d = ggobi_data_new(INTEGER_DATA(snrow)[0], 0);
  if(!d) {
     PROBLEM "Can't create GGobi dataste"
     ERROR;
  }

  GGobi_setDataName(CHAR_DEREF(STRING_ELT(name, 0)), d);

  if(!d->input) 
     d->input = g_malloc(sizeof(InputDescription));

  d->input->fileName = g_strdup(CHAR_DEREF(STRING_ELT(description, 0)));

  pipeline_init(d, gg);
  rows_in_plot_set (d, gg);

  return(RS_datasetInstance(d));
}


/*
  Set the names of rows/observations identified
  by the elements of the indices array which should
  be between 0 and nrows - 1.
 */

USER_OBJECT_
RS_GGOBI(setRowNames)(USER_OBJECT_ names, USER_OBJECT_ indices, USER_OBJECT_ datasetId)
{
  GGobiData *d;
  gchar *tmp, *lbl;
  gint i;
  USER_OBJECT_ ans = NULL_USER_OBJECT;

  d = toData(datasetId);
  g_return_val_if_fail(GGOBI_IS_DATA(d), NULL_USER_OBJECT);

  if(d) {
    gboolean getOldValues = true;
    int num = GET_LENGTH(names);
    int which;

    if(!d->rowlab->data) {
     rowlabels_alloc(d);
     getOldValues = false;
    }

    if(getOldValues)
	PROTECT(ans = NEW_CHARACTER(num));

     for(i = 0; i < num; i++) {
       which = INTEGER_DATA(indices)[i];
       if(getOldValues) {
	   tmp = (gchar *) g_array_index (d->rowlab, gchar *, which);

          /* Copy and free the old value! 
             Currently not freeing this. Bad things happen. Need
             to find out what the g_array_index and insert_val 
             are doing with memory management!
           */    
           if(tmp && tmp[0]) {
  	      SET_STRING_ELT(ans, i, COPY_TO_USER_STRING(tmp));
	      /*  	  g_free(tmp); */
              tmp = NULL;
            }
       }

       lbl = g_strdup(CHAR_DEREF(STRING_ELT(names, i)));
       g_array_insert_val (d->rowlab, which, lbl);
     }

     if(getOldValues)
	 UNPROTECT(1);
  }

  return(ans);
}

/* Sets the IDs in a dataset - by mikel */
void
RS_GGOBI(setIDs)(USER_OBJECT_ ids, USER_OBJECT_ datasetId)
{
  GGobiData *d;
  gchar **g_ids;
  int n, i;
  
  d = toData(datasetId);
  g_return_if_fail(GGOBI_IS_DATA(d));

  if(d) {
	  n = GET_LENGTH(ids);
	  g_ids = (gchar **) S_alloc(n , sizeof(gchar*));
	  for(i = 0; i < n ; i++)
		g_ids[i] = CHAR_DEREF(STRING_ELT(ids, i));
      datad_record_ids_set(d, g_ids, true);
  }
}

/* The C-side of R's setData.. appends datasets
*/
USER_OBJECT_
RS_GGOBI(addData)(USER_OBJECT_ values, 
		  USER_OBJECT_ rowNames, USER_OBJECT_ colNames,
		  USER_OBJECT_ dims,
		  USER_OBJECT_ description,  
		  USER_OBJECT_ name, 
		  USER_OBJECT_ ids,
		  USER_OBJECT_ gobiID
)
{
  InputDescription *desc;

  ggobid *gg = toGGobi(gobiID);
  GGobiData *d = NULL;
  gint i, j;
  USER_OBJECT_ num;
  gchar *logical_levels[] = { "FALSE", "TRUE" };
  
  g_return_val_if_fail(GGOBI_IS_GGOBI(gg), NULL_USER_OBJECT); 
  
  desc = g_new0(InputDescription, 1);
  desc->fileName = g_strdup(asCString(description));
  desc->mode = Sprocess_data;

  d = ggobi_data_new(INTEGER_DATA(dims)[0], INTEGER_DATA(dims)[1]);
  ggobi_data_set_name(d, asCString(name), NULL);
  
  ggobi_data_set_row_labels(d, asCStringArray(rowNames));
  datad_record_ids_set(d, asCStringArray(ids), true); /* ids MUST be given */
  
  for (i = 0; i < GET_LENGTH(values); i++) {
	  USER_OBJECT_ vector = VECTOR_ELT(values, i);
	  vartabled *vt = vartable_element_get(i, d);
	  ggobi_data_set_col_name(d, i, asCString(STRING_ELT(colNames, i)));
	  if (TYPEOF(vector) == INTSXP) {
		  if (isFactor(vector)) {
			  USER_OBJECT_ levels = getAttrib(vector, install("levels"));
			  vartable_element_categorical_init(vt, GET_LENGTH(levels),
			  	asCStringArray(levels), NULL, NULL);
		  }
      for (j = 0; j < INTEGER_DATA(dims)[0]; j++)
        ggobi_data_set_raw_value(d, j, i, (gdouble)INTEGER_DATA(vector)[j]);
	  } else if (IS_NUMERIC(vector))
		  ggobi_data_set_raw_values(d, i, NUMERIC_DATA(vector));
    else if (IS_LOGICAL(vector)) {
      vartable_element_categorical_init(vt, 2, logical_levels, NULL, NULL);
      for (j = 0; j < LOGICAL_DATA(dims)[0]; j++)
        ggobi_data_set_raw_value(d, j, i, (gdouble)LOGICAL_DATA(vector)[j]);
    }
	  else g_critical("Unknown R data type in column %d", i);
  }

  gg->input = desc;
  datad_init (d, gg, false);
  
  num = NEW_INTEGER(1);
  INTEGER_DATA(num)[0] = g_slist_length(gg->d);

  gdk_flush();
  return(num);
}


USER_OBJECT_
RS_GGOBI(setDataName)(USER_OBJECT_ name, USER_OBJECT_ datasetId)
{
   GGobiData *d;
   d = toData(datasetId);
  g_return_val_if_fail(GGOBI_IS_DATA(d), NULL_USER_OBJECT);
   GGobi_setDataName(CHAR_DEREF(STRING_ELT(name, 0)), d);

   return(NULL_USER_OBJECT);
}

/*-------------------------------------------------------------------------*/
/*                              glyphs                                     */
/*-------------------------------------------------------------------------*/

USER_OBJECT_
RS_GGOBI(setCaseGlyphs)(USER_OBJECT_ vals, USER_OBJECT_ sizes, USER_OBJECT_ which, 
                     USER_OBJECT_ datasetId)
{
  ggobid *gg;
  GGobiData *d;
  d = toData(datasetId);
  g_return_val_if_fail(GGOBI_IS_DATA(d), NULL_USER_OBJECT);
  gg = d->gg;
 
  if(d) {
    gint i, id;
    int num = GET_LENGTH(which);
    for(i = 0; i < num; i++) {
      id = INTEGER_DATA(which)[i];
      GGOBI(setCaseGlyph)(id, INTEGER_DATA(vals)[i], INTEGER_DATA(sizes)[i], d, gg);
    } 

    clusters_set (d, gg);
    cluster_table_update (d, gg);  /* dfs, May 20 2005 */
    displays_plot(NULL, FULL, gg);
    gdk_flush();
  }

 return(NULL_USER_OBJECT);
}

USER_OBJECT_
RS_GGOBI(getCaseGlyphs)(USER_OBJECT_ ids, USER_OBJECT_ datasetID)
{
  USER_OBJECT_ ans, types, sizes, names;
  gint i, id, n;
  gboolean all = true;
  ggobid *gg;
  GGobiData *d;

  d = toData(datasetID);
  g_return_val_if_fail(GGOBI_IS_DATA(d), NULL_USER_OBJECT);
  if(!d)
    return(NULL_USER_OBJECT);
  gg = d->gg;

  if(GET_LENGTH(ids) > 0) { 
    n = GET_LENGTH(ids);
    all = false;
  } else
    n = d->nrows;

  PROTECT(types = NEW_INTEGER(n));
  PROTECT(sizes = NEW_INTEGER(n));
  PROTECT(names = NEW_CHARACTER(n));
  for(i = 0; i < n; i++) {
    gchar const *typeName;
    int type;
    if(all) {
      id = i;
    } else {
      id = INTEGER_DATA(ids)[i];
    }
      /* Don't convert to names. */
    INTEGER_DATA(types)[i] = type = GGOBI(getCaseGlyphType)(id, d, gg);
    typeName = GGOBI(getGlyphTypeName)(type);
    SET_STRING_ELT(names, i, COPY_TO_USER_STRING(typeName));
    INTEGER_DATA(sizes)[i] = GGOBI(getCaseGlyphSize)(id, d, gg);
  }

  SET_NAMES(types, names);

  PROTECT(ans = NEW_LIST(2));
  SET_VECTOR_ELT(ans, 0, types);
  SET_VECTOR_ELT(ans, 1, sizes);

  PROTECT(names = NEW_CHARACTER(2));
  SET_STRING_ELT(names, 0, COPY_TO_USER_STRING("type"));
  SET_STRING_ELT(names, 1, COPY_TO_USER_STRING("size"));
  SET_NAMES(ans, names);

  UNPROTECT(5);

  return(ans);
}

USER_OBJECT_
RS_GGOBI(setCaseColors)(USER_OBJECT_ vals, USER_OBJECT_ which, USER_OBJECT_ datasetId)
{
  gint i, id;
  USER_OBJECT_ ans = NULL_USER_OBJECT;
  gint num = GET_LENGTH(which);
  ggobid *gg;
  GGobiData *d;
  d = toData(datasetId);
  g_return_val_if_fail(GGOBI_IS_DATA(d), NULL_USER_OBJECT);
  gg = d->gg;
  
  if(d) {
       for(i = 0; i < num; i++) {
      	 id = INTEGER_DATA(which)[i];
      	 GGOBI(setCaseColor)(id, INTEGER_DATA(vals)[i] - 1, d, gg);
       }
      
       clusters_set (d, gg);          /* reactivated, May 20 2005 */
       cluster_table_update (d, gg);  /* added, May 20 2005 -- dfs */
       displays_plot(NULL, FULL, gg);
       gdk_flush();
  }

  return(ans);
}


USER_OBJECT_
RS_GGOBI(getCaseColors)(USER_OBJECT_ ids, USER_OBJECT_ datasetId)
{
  USER_OBJECT_ colors, names;
  gint i, id, n, color;
  gboolean all = true;
  const gchar *colorName;

  ggobid *gg;
  GGobiData *d;
  d = toData(datasetId);
  g_return_val_if_fail(GGOBI_IS_DATA(d), NULL_USER_OBJECT);
  gg = d->gg;

  if(GET_LENGTH(ids) > 0) { 
    n = GET_LENGTH(ids);
    all = false;
  } else
    n = d->nrows;

  PROTECT(colors = NEW_INTEGER(n));
  PROTECT(names = NEW_CHARACTER(n));
  
  for(i = 0; i < n; i++) {
    if(all) {
      id = i;
    } else {
      id = INTEGER_DATA(ids)[i];
    }
      /* Don't convert to names. */
    INTEGER_DATA(colors)[i] = color = GGOBI(getCaseColor)(id, d, gg);
    colorName = GGOBI(getColorName)(color, gg, true);
    if(colorName && colorName[0])
      SET_STRING_ELT(names, i, COPY_TO_USER_STRING(colorName));
  }

  SET_NAMES(colors, names);

  UNPROTECT(2);

  return(colors);
}

USER_OBJECT_
RS_GGOBI(setVariableTypes)(USER_OBJECT_ vars, USER_OBJECT_ values, USER_OBJECT_ datasetId)
{
  gint i, which;
  ggobid *gg;
  GGobiData *d;
  int num;
  USER_OBJECT_ ans;

  d = toData(datasetId);
  g_return_val_if_fail(GGOBI_IS_DATA(d), NULL_USER_OBJECT);
  gg = d->gg;

  num = GET_LENGTH(vars);

  PROTECT(ans = NEW_INTEGER(num));

  for (i = 0; i < num; i++) {
      which = INTEGER_DATA(vars)[i];
  }

  UNPROTECT(1);
  return(ans);
}

/*
  Get the source name associated with the specified GGobiData
  In the case of loading an R data frame, for example,
  this returns a string like 'data.frame(data)'. If the data originated from
  a file, then the name of that file is returned.
 */

USER_OBJECT_
RS_GGOBI(getSourceName)(USER_OBJECT_ datasetId)
{
  ggobid *gg;
  USER_OBJECT_ ans = NULL_USER_OBJECT;
  GGobiData *d;

  if(GET_LENGTH(datasetId)) {
    d = toData(datasetId);
  g_return_val_if_fail(GGOBI_IS_DATA(d), NULL_USER_OBJECT);
	gg = d->gg;
	PROTECT(ans = NEW_CHARACTER(1));

	if(GGOBI(getFileName)(gg))
		SET_STRING_ELT(ans, 0, COPY_TO_USER_STRING(GGOBI(getFileName)(gg)));

	UNPROTECT(1);
  } 

  return(ans);
}

USER_OBJECT_
RS_GGOBI(datasetDim)(USER_OBJECT_ data)
{
  USER_OBJECT_ ans = NULL_USER_OBJECT;
  GGobiData *d;
   d = toData(data);
  g_return_val_if_fail(GGOBI_IS_DATA(d), NULL_USER_OBJECT);
   if(d) {
     ans = NEW_INTEGER(2);
     INTEGER_DATA(ans)[0] = GGOBI(nrecords)(d);
     INTEGER_DATA(ans)[1] = GGOBI(ncols)(d);
   }

  return(ans);
}
