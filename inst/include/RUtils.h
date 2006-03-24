#ifndef RUTILS_H
#define RUTILS_H

#include "RSGGobi.h"

int R_IS(USER_OBJECT_ obj, const char * const name);
void RS_throwError(char *msg);

USER_OBJECT_ createFactor(USER_OBJECT_ vals, vartabled *vt, GGobiData *d, int which);

#endif
