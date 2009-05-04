#include "RSGGobi.h"

#include <gtk/gtk.h>

#include "vars.h"

USER_OBJECT_
RS_GGOBI(setBrushSize)(USER_OBJECT_ dims, USER_OBJECT_ ggobiId)
{
  ggobid *gg;
  GGobiData *d = NULL;
  gint wd, ht;
  USER_OBJECT_ ans;

  gg = toGGobi(ggobiId);
  
  /*d = toData(datasetId);
  g_return_val_if_fail(GGOBI_IS_DATA(d), NULL_USER_OBJECT);
  gg = d->gg;*/

  if(d) {  

     GGOBI(getBrushSize(&wd, &ht, gg));
        /* We skip the setting if these values are both -1.
           Allows us to set or get with one function.
         */
     if(INTEGER_DATA(dims)[0] > -1 && INTEGER_DATA(dims)[1] > -1)
       GGOBI(setBrushSize(INTEGER_DATA(dims)[0], INTEGER_DATA(dims)[1], gg));
    
     ans = NEW_INTEGER(2);
     INTEGER_DATA(ans)[0] = wd;
     INTEGER_DATA(ans)[1] = ht;

   /* experiments */
   brush_reset(gg->current_display, 0);
   /*   displays_plot (NULL, FULL, gg); */
     gdk_flush();
  } else
    ans = NULL_USER_OBJECT;

  return(ans);
}

USER_OBJECT_
RS_GGOBI(setBrushLocation)(USER_OBJECT_ loc, USER_OBJECT_ datasetId)
{
  ggobid *gg;
  GGobiData *d;
  USER_OBJECT_ ans;
  d = toData(datasetId);
  g_return_val_if_fail(GGOBI_IS_DATA(d), NULL_USER_OBJECT);
  gg = d->gg;
  
  if(d) {
   gint xx, yy;
   GGOBI(getBrushLocation (&xx, &yy, gg));
   if(INTEGER_DATA(loc)[0] > -1 && INTEGER_DATA(loc)[1] > -1)
     GGOBI(setBrushLocation(INTEGER_DATA(loc)[0], INTEGER_DATA(loc)[1], gg));

   ans = NEW_INTEGER(2);  
   INTEGER_DATA(ans)[0] = xx;
   INTEGER_DATA(ans)[1] = yy;
   gdk_flush();
 } else
   ans = NULL_USER_OBJECT;

  return(ans);
}

USER_OBJECT_
RS_GGOBI(setBrushColor)(USER_OBJECT_ cid, USER_OBJECT_ ggobiId)
{
  USER_OBJECT_ ans = NULL_USER_OBJECT;
  ggobid *gg = toGGobi(ggobiId);
  g_return_val_if_fail(GGOBI_IS_GGOBI(gg), NULL_USER_OBJECT);
  if(gg) {
   ans = RS_GGOBI(getBrushColor)(ggobiId);
   (void) GGOBI(setBrushColor)(INTEGER_DATA(cid)[0], gg);
   brush_reset(gg->current_display, 0);
   gdk_flush();
  }

 return(ans);
}

USER_OBJECT_
RS_GGOBI(getBrushColor)(USER_OBJECT_ ggobiId)
{
  ggobid *gg = toGGobi(ggobiId);
  g_return_val_if_fail(GGOBI_IS_GGOBI(gg), NULL_USER_OBJECT);
  if(gg) {
    USER_OBJECT_ ans;
    gint cid;

     cid =  GGOBI(getBrushColor)(gg);
     PROTECT(ans = NEW_INTEGER(1));
     INTEGER_DATA(ans)[0] = cid;
     SET_NAMES(ans, RS_INTERNAL_GGOBI(getColorName)(cid, gg));
     UNPROTECT(1);
    return(ans);
  } else
    return(NULL);
}

USER_OBJECT_
RS_GGOBI(setBrushGlyph)(USER_OBJECT_ vals, USER_OBJECT_ ggobiId)
{
  ggobid *gg = toGGobi(ggobiId);
  g_return_val_if_fail(GGOBI_IS_GGOBI(gg), NULL_USER_OBJECT);
  if(gg) {
    GGOBI(setBrushGlyph)(INTEGER_DATA(vals)[0], INTEGER_DATA(vals)[1], gg);
    brush_reset(gg->current_display, 0);
    gdk_flush();
  }
 return(NULL_USER_OBJECT);
}
USER_OBJECT_
RS_GGOBI(getBrushGlyph)(USER_OBJECT_ ggobiId)
{
  gint t, s;
  ggobid *gg;
  USER_OBJECT_ ans;
  gg = toGGobi(ggobiId);
  g_return_val_if_fail(GGOBI_IS_GGOBI(gg), NULL_USER_OBJECT);
  if(gg) {
   GGOBI(getBrushGlyph)(&t, &s, gg);
   ans = NEW_INTEGER(2);
   INTEGER_DATA(ans)[0] = t;
   INTEGER_DATA(ans)[1] = s;
  } else
    ans = NULL_USER_OBJECT;

  return(ans);
}

USER_OBJECT_
RS_INTERNAL_GGOBI(getColorName)(gint cid, ggobid *gg)
{
  USER_OBJECT_ name;
  const gchar *colName;
    PROTECT(name = NEW_CHARACTER(1));
    colName = GGOBI(getColorName)(cid, gg, false);
    if(colName)
      SET_STRING_ELT(name, 0, COPY_TO_USER_STRING(colName));
    UNPROTECT(1);
  return(name);
}
