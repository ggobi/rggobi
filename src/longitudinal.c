#include "RSGGobi.h"

USER_OBJECT_
RS_GGOBI(setAsTimeVariables)(USER_OBJECT_ vars, USER_OBJECT_ values, USER_OBJECT_ datasetId)
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

  PROTECT(ans = NEW_LOGICAL(num));

  for (i = 0; i < num; i++) {
      which = INTEGER_DATA(vars)[i];
  }

  UNPROTECT(1);
  return(ans);
}
