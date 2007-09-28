#include "RSGGobi.h"
#include <R_ext/Arith.h> /* For the R_IsNaN() routine. */

/**
  This gets and sets the colorschemes in a GGobi instance or
  the session options.
 */
USER_OBJECT_ RSGGobi_Internal_getColorSchemes(GList *schemes);
USER_OBJECT_ RSGGobi_Internal_getColorScheme(colorschemed *scheme);
USER_OBJECT_ RSGGobi_Internal_getColor(gfloat *vals, colorscaletype type, int n);
colorschemed *RS_createGGobiScheme(USER_OBJECT_ scheme, USER_OBJECT_ name);

gfloat *RS_setGGobiColor(USER_OBJECT_ colors);

static GList *
RSGGobi_Internal_getSchemeFromGGobi(USER_OBJECT_ gobiId)
{
    GList *schemes = NULL;
	
    if(GET_LENGTH(gobiId)) {
		ggobid *gg;
		gg = toGGobi(gobiId);
    g_return_val_if_fail(GGOBI_IS_GGOBI(gg), NULL);
		schemes = gg->colorSchemes;
    } else if(sessionOptions) {
		schemes = sessionOptions->colorSchemes;
	} else {
		g_critical("GGobi has not been initialized yet. Please create an instance!");
	}

    return(schemes);
}

USER_OBJECT_
RS_GGOBI(getColorSchemes)(USER_OBJECT_ gobiId)
{
    GList *schemes;

    schemes = RSGGobi_Internal_getSchemeFromGGobi(gobiId);

    return(RSGGobi_Internal_getColorSchemes(schemes));
}

USER_OBJECT_
RSGGobi_Internal_getColorSchemes(GList *schemes)
{
    int i, n;
    USER_OBJECT_ ans, names;
    colorschemed *s;

    n = g_list_length(schemes);
    PROTECT(ans = NEW_LIST(n));
    PROTECT(names = NEW_CHARACTER(n));

    for(i = 0; i < n ; i++) {
		s = (colorschemed *) g_list_nth_data(schemes, i);
		SET_STRING_ELT(names, i, COPY_TO_USER_STRING(s->name));
		SET_VECTOR_ELT(ans, i, RSGGobi_Internal_getColorScheme(s));
    }

    SET_NAMES(ans, names);
    UNPROTECT(2);
    return(ans);
}


enum {
  CS_COLORS, CS_BACKGROUND, CS_ANNOTATIONS, CS_CRITICAL_VALUE, CS_DESCRIPTION, 
  CS_TYPE, CS_SYSTEM, CS_NAME, CS_NUM_SLOTS
};
static gchar *cs_names[] = { 
  "colors", "background", "annotations", "criticalvalue",
  "description", "type", "system", "name"
};

USER_OBJECT_
RSGGobi_Internal_getColorScheme(colorschemed *scheme)
{
    USER_OBJECT_ ans, tmp, names;
    int i, n;

    PROTECT(ans = NEW_LIST(CS_NUM_SLOTS));

    n = scheme->n;
    PROTECT(names = NEW_CHARACTER(n));
    PROTECT(tmp = NEW_LIST(n));
    for(i = 0; i < n; i++) {
      SET_VECTOR_ELT(tmp, i, RSGGobi_Internal_getColor(scheme->data[i], scheme->type, 3));
      SET_STRING_ELT(names, i, COPY_TO_USER_STRING(g_array_index(scheme->colorNames, gchar *, i)));
    }
    SET_NAMES(tmp, names);
    SET_VECTOR_ELT(ans, CS_COLORS, tmp);
    UNPROTECT(2);

    SET_VECTOR_ELT(ans, CS_BACKGROUND, RSGGobi_Internal_getColor(scheme->bg, scheme->type, 3));
    SET_VECTOR_ELT(ans, CS_ANNOTATIONS, RSGGobi_Internal_getColor(scheme->accent, scheme->type, 3));

    PROTECT(tmp = NEW_INTEGER(1));
    INTEGER_DATA(tmp)[0] = scheme->criticalvalue;
    SET_VECTOR_ELT(ans, CS_CRITICAL_VALUE, tmp);
    UNPROTECT(1);

    PROTECT(tmp = NEW_CHARACTER(1));
    if(scheme->description)
        SET_STRING_ELT(tmp, 0, COPY_TO_USER_STRING(scheme->description));
    SET_VECTOR_ELT(ans, CS_DESCRIPTION, tmp);
    UNPROTECT(1);


    PROTECT(names = NEW_CHARACTER(1));
    PROTECT(tmp = NEW_INTEGER(1));
    {
        char *schemeName = "";
        switch(scheme->type) {
    	   case rgb:
   	      schemeName = "diverging";
   	     break;
    	   case hsv:
   	      schemeName = "sequential";
         break;
         case cmy:
   	      schemeName = "spectral";
         break;
         case cmyk:
   	      schemeName = "qualitative";
         break;
         default:
         break;
       }
        SET_STRING_ELT(names, 0, COPY_TO_USER_STRING(schemeName));
    }
    INTEGER_DATA(tmp)[0] = scheme->type;
    SET_NAMES(tmp, names);
    SET_VECTOR_ELT(ans, CS_TYPE, tmp);
    UNPROTECT(2);



    PROTECT(names = NEW_CHARACTER(1));
    PROTECT(tmp = NEW_INTEGER(1));
    {
        char *schemeName = "";
        switch(scheme->system) {
    	   case rgb:
   	      schemeName = "rgb";
         break;
    	   case hsv:
   	      schemeName = "hsv";
         break;
         case cmy:
   	      schemeName = "cmy";
         break;
         case cmyk:
   	      schemeName = "cmyk";
         break;
         default:
         break;
       }
        SET_STRING_ELT(names, 0, COPY_TO_USER_STRING(schemeName));
    }
    INTEGER_DATA(tmp)[0] = scheme->system;
    SET_NAMES(tmp, names);
    SET_VECTOR_ELT(ans, CS_SYSTEM, tmp);
    UNPROTECT(2);

    PROTECT(tmp = NEW_CHARACTER(1));
    SET_STRING_ELT(tmp, 0, COPY_TO_USER_STRING(scheme->name));
    SET_VECTOR_ELT(ans, CS_NAME, tmp);
    UNPROTECT(1);

    SET_NAMES(ans, asRStringArrayWithSize(cs_names, CS_NUM_SLOTS));
    
    UNPROTECT(1);
    return(ans);
}

USER_OBJECT_ 
RSGGobi_Internal_getColor(gfloat *vals, colorscaletype type, int n)
{
    USER_OBJECT_ ans;
    int i;

    ans = NEW_NUMERIC(n);
    for(i = 0; i < n; i++) {
      NUMERIC_DATA(ans)[i] = vals ? vals[i] : NA_REAL;
    }

    return(ans);
}



USER_OBJECT_
RS_GGOBI(addColorScheme)(USER_OBJECT_ sscheme, USER_OBJECT_ name, USER_OBJECT_ overwrite, USER_OBJECT_ gobiId)
{
    USER_OBJECT_ ans;
    int index = 0;
    colorschemed *scheme;
    GList *schemes = RSGGobi_Internal_getSchemeFromGGobi(gobiId);

    scheme = RS_createGGobiScheme(sscheme, name);


       /* Perhaps we need to inform the Color scheme tool if it is open,
          and add it to the tree. Similarly, if we set the active scheme from
          R, we should tell everybody that might be interested. */
    if(LOGICAL_DATA(overwrite)[0]) {
	/* */
/*	g_list_find_custom(); */
    } else {
	schemes = g_list_append(schemes, scheme);
	index = g_list_length(schemes);
    }

    ans = NEW_INTEGER(1);
    INTEGER_DATA(ans)[0] = index;
    return(ans);
}

colorschemed *
RS_createGGobiScheme(USER_OBJECT_ sscheme, USER_OBJECT_ name)
{
 colorschemed *scheme;
 int n, i;
 USER_OBJECT_ colorNames;
 const char *str;
 gchar *gstr;
 SEXP colors;
 gfloat *tmp;

 scheme = alloc_colorscheme(sizeof(colorschemed));
 if(!scheme) {
  PROBLEM "Cannot allocate space for color scheme"
  ERROR;
 }

 scheme->type = INTEGER_DATA(GET_SLOT(sscheme, Rf_install("type")))[0];
 scheme->system = INTEGER_DATA(GET_SLOT(sscheme, Rf_install("system")))[0];
 scheme->criticalvalue = INTEGER_DATA(GET_SLOT(sscheme, Rf_install("criticalvalue")))[0];

 scheme->name = g_strdup(CHAR_DEREF(STRING_ELT(name, 0)));
 scheme->description = g_strdup(CHAR_DEREF(STRING_ELT(GET_SLOT(sscheme, Rf_install("description")), 0)));

 colors = GET_SLOT(sscheme, Rf_install("colors"));
 scheme->n = n = GET_LENGTH(colors);
 colorNames = GET_NAMES(colors);
 scheme->data = (gfloat **) g_malloc(sizeof(gfloat *) * n);
 for(i = 0; i < n; i++) {
    if(GET_LENGTH(colorNames) && (str = CHAR_DEREF(STRING_ELT(colorNames, i)))) {
	gstr = g_strdup(str);
	g_array_append_val(scheme->colorNames, gstr);
    }
    scheme->data[i] = RS_setGGobiColor(VECTOR_ELT(colors, i));  
 }

 tmp = RS_setGGobiColor(GET_SLOT(sscheme, Rf_install("background")));
 if(tmp)
    scheme->bg = tmp;

 tmp = RS_setGGobiColor(GET_SLOT(sscheme, Rf_install("annotations")));
 if(tmp)
    scheme->accent = tmp;

 return(scheme);
}


gfloat *
RS_setGGobiColor(USER_OBJECT_ colors)
{
    int j, numVals;
    gfloat *data;
    colors = GET_SLOT(colors, Rf_install(".Data"));
    numVals = GET_LENGTH(colors);
    data = g_malloc(sizeof(gfloat) * numVals);
    for(j = 0; j < numVals ; j++) {
        double val;
        val = NUMERIC_DATA(colors)[j];
        if(R_IsNaN(val)) {
  	   g_free(data);
	   return(NULL);
	}
	data[j] = val;
    }

    return(data);
}

static int
schemeNameCompare(gconstpointer scheme, gconstpointer data)
{
    return(strcmp(((colorschemed *)scheme)->name, (char *)data));
}


USER_OBJECT_
RS_GGOBI(getActiveColorScheme)(USER_OBJECT_ gobiId)
{
  USER_OBJECT_ ans = NULL_USER_OBJECT;
  colorschemed *scheme = NULL;
  
  /*PROTECT(ans = NEW_CHARACTER(1));
  SET_STRING_ELT(ans, 0, COPY_TO_USER_STRING(sessionOptions->activeColorScheme));
  
  UNPROTECT(1);
  return(ans);
  */
  if(GET_LENGTH(gobiId) == 0) {
      scheme = findColorSchemeByName(sessionOptions->colorSchemes, sessionOptions->activeColorScheme);
  } else {
      ggobid *gg = toGGobi(gobiId);
  g_return_val_if_fail(GGOBI_IS_GGOBI(gg), NULL_USER_OBJECT);
      scheme = gg->activeColorScheme;
  }
  if(scheme)
    ans = RSGGobi_Internal_getColorScheme(scheme);

  return(ans);
}



USER_OBJECT_
RS_GGOBI(setActiveColorScheme)(USER_OBJECT_ id, USER_OBJECT_ gobiId)
{
  USER_OBJECT_ ans;
  char *tmp = NULL;
  GList *schemes;
  ggobid *gg = NULL;
  colorschemed *newScheme = NULL;

  if(GET_LENGTH(gobiId) == 0) {
    if(sessionOptions) {
      tmp = sessionOptions->activeColorScheme;
      if(IS_INTEGER(id)) {
        if(sessionOptions->colorSchemes) {
          newScheme = g_list_nth_data(sessionOptions->colorSchemes, INTEGER_DATA(id)[0]);
          if(newScheme)
            sessionOptions->activeColorScheme = g_strdup(newScheme->name);
          else {
            PROBLEM "No such color scheme available in the session options."
            ERROR;
          }
        } else {
          PROBLEM "No color schemes available in the session options."
          ERROR;
        }
      } else if(IS_CHARACTER(id)) {
        sessionOptions->activeColorScheme = g_strdup(CHAR_DEREF(STRING_ELT(id, 0)));
        if(sessionOptions->colorSchemes)
          newScheme = findColorSchemeByName(sessionOptions->colorSchemes, sessionOptions->activeColorScheme);
        if(!newScheme) {
          PROBLEM "Setting default color scheme name in session options, but there is no such color scheme available."
          WARN;
        }
      }
    } else {
      PROBLEM "GGobi has not been initialized yet. Please initialize the engine (init.ggobi()) or create an instance (ggobi())!"
      ERROR;
    }
  } else {
    GList *scheme_el = NULL;
    
    gg = toGGobi(gobiId);
    g_return_val_if_fail(GGOBI_IS_GGOBI(gg), NULL_USER_OBJECT);
    schemes = RSGGobi_Internal_getSchemeFromGGobi(gobiId);
  
    if(!schemes) {
      PROBLEM "Cannot get color schemes list"
      ERROR;
    }
  
    if(gg->activeColorScheme)
      tmp = gg->activeColorScheme->name;
  
    if(IS_INTEGER(id))
      scheme_el = g_list_nth(schemes, INTEGER_DATA(id)[0]);
    else if(IS_CHARACTER(id))
      scheme_el = g_list_find_custom(schemes, CHAR_DEREF(STRING_ELT(id, 0)), 
        schemeNameCompare);
    
    if (scheme_el)
      newScheme = scheme_el->data;
  
    if(newScheme) {
      GGobiData *d = (GGobiData *) g_slist_nth_data(gg->d, 0);
      gg->activeColorScheme = newScheme;
      colorscheme_init(newScheme);
      displays_plot (NULL, FULL, gg);
      symbol_window_redraw (gg);
      cluster_table_update (d, gg);
      gdk_flush();
    }
  }

  if(tmp) {
    PROTECT(ans = NEW_CHARACTER(1));
    SET_STRING_ELT(ans, 0, COPY_TO_USER_STRING(gg->activeColorScheme->name));
    UNPROTECT(1);
  } else ans = NULL_USER_OBJECT;

  return(ans);
}
