#include "RSGGobi.h"
#include "RUtils.h"

#include <gtk/gtk.h>

#include "vars.h"
#include "parcoordsClass.h"
#include "scatmatClass.h"
#include "scatterplotClass.h"
#include "tsdisplay.h"
#include "barchartDisplay.h"

greal
RS_INTERNAL_GGOBI(tformToWorld)(GGobiData *d, gint j, greal min, greal max)
{
  vartabled *var = vartable_element_get(j, d);
  gdouble var_min = var->lim.min, var_max = var->lim.max;
  gdouble mid = (max - min) / 2.0 + min;
  greal ftmp;

  ftmp = -1.0 + 2.0 * ((greal) mid - var_min) / (var_max - var_min);
  
  return (greal) (PRECISION1 * ftmp);
}

void
RS_INTERNAL_GGOBI(getPlotRange)(displayd *display, splotd *sp, fcoords *tfmin, fcoords *tfmax)
{
  icoords scr;
  cpaneld *cpanel;
  
  cpanel = &display->cpanel;
  if (display->hrule == NULL)
    return;

  tfmin->x = tfmin->y = tfmax->x = tfmax->y = 0.0;

  scr.x = scr.y = 0;
  GGOBI_EXTENDED_SPLOT_GET_CLASS (sp)->screen_to_tform (cpanel, sp, &scr, tfmin, 
    display->ggobi);

  scr.x = sp->max.x;
  scr.y = sp->max.y;
  GGOBI_EXTENDED_SPLOT_GET_CLASS (sp)->screen_to_tform (cpanel, sp, &scr, tfmax, 
    display->ggobi);
}

USER_OBJECT_
RS_GGOBI(setPlotRange)(USER_OBJECT_ s_min_x, USER_OBJECT_ s_min_y, 
  USER_OBJECT_ s_max_x, USER_OBJECT_ s_max_y, USER_OBJECT_ rdisplay, USER_OBJECT_ plot)
{
  displayd *display;
  USER_OBJECT_ ans;
  fcoords tfmin, tfmax;
  splotd *sp;
  greal min_x = NUMERIC_DATA(s_min_x)[0], min_y = NUMERIC_DATA(s_min_y)[0];
  greal max_x = NUMERIC_DATA(s_max_x)[0], max_y = NUMERIC_DATA(s_max_y)[0];
  greal scale_x, scale_y;
  
  display = toDisplay(rdisplay);
	g_return_val_if_fail(GGOBI_IS_DISPLAY(display), NULL_USER_OBJECT);
  g_return_val_if_fail(pmode_get(display, display->ggobi) == XYPLOT, NULL_USER_OBJECT);

  sp = GGOBI(getPlot)(display, INTEGER_DATA(plot)[0]-1);
  g_return_val_if_fail(GGOBI_IS_SPLOT(sp), NULL_USER_OBJECT);
  
  RS_INTERNAL_GGOBI(getPlotRange)(display, sp, &tfmin, &tfmax);
  
  sp->pmid.x = RS_INTERNAL_GGOBI(tformToWorld)(display->d, sp->xyvars.x, min_x, max_x);
  sp->pmid.y = RS_INTERNAL_GGOBI(tformToWorld)(display->d, sp->xyvars.y, min_y, max_y);
  
  scale_x = sp->scale.x * (max_x - min_x) / (tfmax.x - tfmin.x);
  scale_y = sp->scale.y * (max_y - min_y) / (tfmax.y - tfmin.y);
  
  splot_zoom(sp, scale_x, scale_y);
  
  ans = NEW_LOGICAL(1);
  LOGICAL_DATA(ans)[0] = TRUE;
  return(ans);
}

USER_OBJECT_
RS_GGOBI(getPlotRange)(USER_OBJECT_ rdisplay, USER_OBJECT_ plot)
{
  /* This returns a list of two vectors, one for the X min/max and one for the Y */ 
  displayd *display;
  splotd *sp;
  USER_OBJECT_ ans, s_x, s_y;
  fcoords tfmin, tfmax;
  
  display = toDisplay(rdisplay);
  g_return_val_if_fail(GGOBI_IS_DISPLAY(display), NULL_USER_OBJECT);
  g_return_val_if_fail(pmode_get(display, display->ggobi) == XYPLOT, NULL_USER_OBJECT);
  
  sp = GGOBI(getPlot)(display, INTEGER_DATA(plot)[0]-1);
  g_return_val_if_fail(GGOBI_IS_SPLOT(sp), NULL_USER_OBJECT);
  
  RS_INTERNAL_GGOBI(getPlotRange)(display, sp, &tfmin, &tfmax);
  
  PROTECT(ans = NEW_LIST(2));
  s_x =  NEW_NUMERIC(2);
  SET_VECTOR_ELT(ans, 0, s_x);
  s_y =  NEW_NUMERIC(2);
  SET_VECTOR_ELT(ans, 1, s_y);
  
  NUMERIC_DATA(s_x)[0] = tfmin.x;
  NUMERIC_DATA(s_x)[1] = tfmax.x;
  NUMERIC_DATA(s_y)[0] = tfmax.y;
  NUMERIC_DATA(s_y)[1] = tfmin.y;
  
  UNPROTECT(1);
  
  return ans;
}
