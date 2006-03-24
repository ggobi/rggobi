#include "RSGGobi.h"

USER_OBJECT_
RS_GGOBI(setAsTimeVariables)(USER_OBJECT_ vars, USER_OBJECT_ values, USER_OBJECT_ datasetId)
{
  gint i, which;
  ggobid *gg;
  GGobiData *d;
  int num;

  USER_OBJECT_ ans;

  d = GGOBI_DATA(toData(datasetId));
  gg = d->gg;
  
  num = GET_LENGTH(vars);

  PROTECT(ans = NEW_LOGICAL(num));

  for (i = 0; i < num; i++) {
      which = INTEGER_DATA(vars)[i];
      //LOGICAL_DATA(ans)[i] = ggobi_data_set_time_var(d, INTEGER_DATA(vars)[i], LOGICAL_DATA(values)[i]);
  }

  UNPROTECT(1);
  return(ans);
}
