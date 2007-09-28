#include "conversion.h"
#include <glib-object.h>

char **
asCStringArray(USER_OBJECT_ svec)
{
    char **els = NULL;

    int i, n;

    n = GET_LENGTH(svec);
    if(n > 0) {
    els = (char **) R_alloc(n+1, sizeof(char*));
    for(i = 0; i < n; i++) {
        els[i] = (char *)CHAR_DEREF(STRING_ELT(svec, i));
    }
        els[n] = NULL;
    }

    return(els);
}

gboolean
asCLogical(USER_OBJECT_ s_log)
{
    if (GET_LENGTH(s_log) == 0)
		return(FALSE);
	return(LOGICAL_DATA(s_log)[0]);
}
int
asCInteger(USER_OBJECT_ s_int)
{
	if (GET_LENGTH(s_int) == 0)
		return(0);
    return(INTEGER_DATA(s_int)[0]);
}
guchar
asCRaw(USER_OBJECT_ s_raw)
{
	if (GET_LENGTH(s_raw) == 0)
		return(0);
    return(RAW(s_raw)[0]);
}
double
asCNumeric(USER_OBJECT_ s_num)
{
	if (GET_LENGTH(s_num) == 0)
		return(0);
    return(NUMERIC_DATA(s_num)[0]);
}
/* This function takes a STRSXP or CHARSXP to a C string */
const char *
asCString(USER_OBJECT_ s_str)
{
    if (IS_VECTOR(s_str)) {
      if (GET_LENGTH(s_str) == 0)
        return(NULL);
      s_str = STRING_ELT(s_str, 0);
    } 
    return(CHAR_DEREF(s_str));
    /*return(CHAR_DEREF(STRING_ELT(s_str, 0)));*/
}
char
asCCharacter(USER_OBJECT_ s_char)
{
    return(asCString(s_char)[0]);
}

USER_OBJECT_
asRLogical(Rboolean val)
{
  USER_OBJECT_ ans;
  ans = NEW_LOGICAL(1);
  LOGICAL_DATA(ans)[0] = val;

  return(ans);
}
USER_OBJECT_
asRRaw(guchar val)
{
  USER_OBJECT_ ans;
  ans = NEW_RAW(1);
  RAW(ans)[0] = val;

  return(ans);
}
USER_OBJECT_
asRInteger(int val)
{
  USER_OBJECT_ ans;
  ans = NEW_INTEGER(1);
  INTEGER_DATA(ans)[0] = val;

  return(ans);
}
USER_OBJECT_
asRNumeric(double val)
{
  USER_OBJECT_ ans;
  ans = NEW_NUMERIC(1);
  NUMERIC_DATA(ans)[0] = val;

  return(ans);
}
USER_OBJECT_
asRCharacter(char c)
{
    char str[] = { c, '\0' };
    return(asRString(str));
}
USER_OBJECT_
asRString(const char *val)
{
  USER_OBJECT_ ans;
  
  if (!val)
	  return(NULL_USER_OBJECT);
  
  PROTECT(ans = NEW_CHARACTER(1));
  if(val)
      SET_STRING_ELT(ans, 0, COPY_TO_USER_STRING(val));
  UNPROTECT(1);

  return(ans);
}

void *
getPtrValue(USER_OBJECT_ sval)
{
  if(sval == NULL_USER_OBJECT)
      return(NULL);

  return(R_ExternalPtrAddr(sval));
}

USER_OBJECT_
R_internal_getGTypeHierarchy(GType type)
{
  USER_OBJECT_ ans;
  int n = 0;
  GType orig = type;

  while(type != 0 && type != G_TYPE_INVALID) {
     type = g_type_parent(type);
     n++;
  }

  PROTECT(ans = NEW_CHARACTER(n));
  n = 0;
  type = orig;
  while(type != G_TYPE_INVALID) {
     const char *val;
     val = g_type_name(type);
     SET_STRING_ELT(ans, n, COPY_TO_USER_STRING(val));
     n++;
     type =  g_type_parent(type);
  }
  UNPROTECT(1);

  return(ans);
}
USER_OBJECT_
R_internal_getInterfaces(GType type)
{
    SEXP list;
    GType *interfaces;
    guint n, i;

    interfaces = g_type_interfaces(type, &n);
    PROTECT(list = NEW_CHARACTER(n));
    for(i = 0; i < n; i++)
        SET_STRING_ELT(list, i, COPY_TO_USER_STRING(g_type_name(interfaces[i])));

    g_free(interfaces);

    UNPROTECT(1);

    return(list);
}

USER_OBJECT_
toRPointer(void *val, const char *typeName)
{
    USER_OBJECT_ ans, klass = NULL;
	GType type = 0;
	
    if(val == NULL)
       return(NULL_USER_OBJECT);

    PROTECT(ans = R_MakeExternalPtr(val, Rf_install(typeName), NULL_USER_OBJECT));
	
	if (typeName)
        type = g_type_from_name(typeName);
    if(type) {
        if (G_TYPE_IS_INSTANTIATABLE(type) || G_TYPE_IS_INTERFACE(type))
            type = G_TYPE_FROM_INSTANCE(val);
        if (G_TYPE_IS_DERIVED(type)) {
            setAttrib(ans, install("interfaces"), R_internal_getInterfaces(type));
            PROTECT(klass = R_internal_getGTypeHierarchy(type));
        }
    }
    if (!klass && typeName) {
        PROTECT(klass = asRString(typeName));
    }
	
    if(klass) {
		SET_CLASS(ans, klass);
		UNPROTECT(1);
    }
	
    UNPROTECT(1);
  
    return(ans);
}
