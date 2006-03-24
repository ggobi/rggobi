#include "RSGGobi.h"

/*
  R & S routines for querying the system values
  such as glyphs, color names, etc. in the X/GGobi
  session.
 */

USER_OBJECT_ 
RS_GGOBI(getGlyphTypes)()
{
 USER_OBJECT_ ans, names;
 int n = -1, i;
 gint *gtypes;
 const gchar * const *gnames;
 
  gtypes = GGOBI(getGlyphTypes)(&n); 
  gnames = GGOBI(getGlyphTypeNames)(&n); 

 PROTECT(ans = NEW_INTEGER(n));
 PROTECT(names = NEW_CHARACTER(n));

 for(i = 0; i < n; i++) {
  INTEGER_DATA(ans)[i] = gtypes[i];
  SET_STRING_ELT(names, i, COPY_TO_USER_STRING(gnames[i]));
 }

 SET_NAMES(ans, names);

 UNPROTECT(2);

 return(ans);
}


USER_OBJECT_
RS_GGOBI(getGlyphSizes)()
{
  int i;
  USER_OBJECT_ ans;

  PROTECT(ans = NEW_INTEGER(NGLYPHSIZES+1));
  for(i = 0;  i <= NGLYPHSIZES; i++)
    INTEGER_DATA(ans)[i] = i;

  UNPROTECT(1);

  return(ans);
}



USER_OBJECT_ 
RS_GGOBI(getPModeNames)()
{
 USER_OBJECT_ ans;
 int n = -1, i;
 const gchar *const *gnames;
 
 gnames = GGOBI(getPModeNames)(&n); 

 PROTECT(ans = NEW_CHARACTER(n));

 for(i = 0; i < n; i++) {
  SET_STRING_ELT(ans, i, COPY_TO_USER_STRING(gnames[i]));
 }

 UNPROTECT(1);

 return(ans);
}

USER_OBJECT_ 
RS_GGOBI(getIModeNames)()
{
 USER_OBJECT_ ans;
 int n = -1, i;
 const gchar *const *gnames;
 
 gnames = GGOBI(getIModeNames)(&n); 

 PROTECT(ans = NEW_CHARACTER(n));

 for(i = 0; i < n; i++) {
  SET_STRING_ELT(ans, i, COPY_TO_USER_STRING(gnames[i]));
 }

 UNPROTECT(1);

 return(ans);
}

USER_OBJECT_
RS_GGOBI(getVersionInfo)()
{
  USER_OBJECT_ ans, tmp;
  const int *versionNumbers;
  int i;
  PROTECT(ans = NEW_LIST(3));
  SET_VECTOR_ELT(ans, 0, tmp = NEW_CHARACTER(1));
    SET_STRING_ELT(tmp, 0, COPY_TO_USER_STRING(GGOBI(getVersionDate)()));

  SET_VECTOR_ELT(ans, 2, tmp = NEW_CHARACTER(1));
    SET_STRING_ELT(tmp, 0, COPY_TO_USER_STRING(GGOBI(getVersionString)()));

  SET_VECTOR_ELT(ans, 1, tmp = NEW_INTEGER(3));
    versionNumbers = GGOBI(getVersionNumbers)();
    for(i = 0; i < 3; i++) {
      INTEGER_DATA(tmp)[i] = versionNumbers[i];
    }

  UNPROTECT(1);

  return(ans);
}

USER_OBJECT_
RS_GGOBI(getDataModes)()
{
  USER_OBJECT_ ans, names;
  const gchar * const *modeNames;
  int n, i;

  modeNames = GGOBI(getDataModeNames)(&n);
  PROTECT(ans = NEW_INTEGER(n));
  PROTECT(names = NEW_CHARACTER(n));
  for(i = 0; i < n; i++) {
    INTEGER_DATA(ans)[i] = i;
    SET_STRING_ELT(names, i, COPY_TO_USER_STRING(modeNames[i]));
  }

  if(modeNames)
     g_free((gchar *)modeNames);

  SET_NAMES(ans, names);
  UNPROTECT(2);
  return(ans);
}
