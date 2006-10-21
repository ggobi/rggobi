#ifndef RSGGOBI_H
#define RSGGOBI_H

#include "RSCommon.h"
#include "GGobiAPI.h"
#include "conversion.h"

/* Identifies a routine that interfaces directly with R */
#define RS_GGOBI(a) RS_GGOBI_##a

/* Identifies a routine that cannot be called directly from
   R/S via a .C or .Call, but that is part of this package's
   symbol table.
*/
#define RS_INTERNAL_GGOBI(a) RSint_GGOBI_##a

USER_OBJECT_ RS_INTERNAL_GGOBI(getDisplay)(gboolean full, displayd *display, ggobid *gg);

void RGGobiErrorHandler(int status);
int isMissingValue(double d);

#include "externs.h"

USER_OBJECT_ RS_INTERNAL_GGOBI(getVariableNames)(GGobiData *d);
USER_OBJECT_ RS_INTERNAL_GGOBI(getColorName)(gint cid, ggobid *gg);

void RS_INTERNAL_GGOBI(limited_event_handle)(gint max);

#include "RSGGobiEntryPoints.h"

#endif
