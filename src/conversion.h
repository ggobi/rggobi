#ifndef CONVERSION_H
#define CONVERSION_H

#include "RSGGobi.h"

typedef void* (*ElementConverter)(void *element);

/***** ARRAY CONVERSION MACROS ******/

/* converts an array, taking the reference of each element, so that conversion
	functions taking a pointer parameter will work (array elements are values) */
#define asRArrayRef(array, converter) \
__extension__ \
({ \
    asRArray(&array, converter); \
})

#define asRArrayRefWithSize(array, converter, n) \
__extension__ \
({ \
    asRArrayWithSize(&array, converter, n); \
})
/* converts an array directly using the conversion function to an R list */
#define asRArray(array, converter) \
__extension__ \
({ \
    _asRArray(array, converter, LIST, VECTOR); \
})

#define asRArrayWithSize(array, converter, n) \
__extension__ \
({ \
    _asRArrayWithSize(array, converter, n, LIST, VECTOR); \
})

/* converts primitive (numeric, integer, logical) arrays to R vectors */
#define _asRPrimArray(array, TYPE) \
__extension__ \
({ \
    int n = 0; \
	if (!array) \
		NULL; \
    while(array[n++]); \
    _asRPrimArrayWithSize(array, n-1, TYPE); \
})

#define _asRPrimArrayWithSize(array, n, TYPE) \
__extension__ \
({ \
    int i; \
    USER_OBJECT_ s_obj; \
    PROTECT(s_obj = NEW_ ## TYPE(n)); \
\
    for (i = 0; i < n; i++) { \
        TYPE ## _POINTER(s_obj)[i] = array[i]; \
	} \
\
    UNPROTECT(1); \
    s_obj; \
})

/* core converter, for converting string arrays and other arrays of pointer types */
#define _asRArray(array, converter, TYPE, SETTER_TYPE) \
__extension__ \
({ \
    int n = 0; \
	if (!array) \
		NULL; \
    while(array[n++]); \
    _asRArrayWithSize(array, converter, n-1, TYPE, SETTER_TYPE); \
})

#define _asRArrayWithSize(array, converter, n, TYPE, SETTER_TYPE) \
__extension__ \
({ \
    int i; \
    USER_OBJECT_ s_obj; \
    PROTECT(s_obj = NEW_ ## TYPE(n)); \
\
    for (i = 0; i < n; i++) { \
        SET_ ## SETTER_TYPE ## _ELT(s_obj, i, converter(array[i])); \
	} \
\
    UNPROTECT(1); \
    s_obj; \
})

/* Below are primitive array -> R vector converters */

#define asRStringArray(array) \
__extension__ \
({ \
    _asRArray(array, COPY_TO_USER_STRING, CHARACTER, STRING); \
})

#define asRStringArrayWithSize(array, n) \
__extension__ \
({ \
    _asRArrayWithSize(array, COPY_TO_USER_STRING, n, CHARACTER, STRING); \
})

#define asRIntegerArray(array) \
__extension__ \
({ \
    _asRPrimArray(array, INTEGER); \
})

#define asRIntegerArrayWithSize(array, size) \
__extension__ \
({ \
    _asRPrimArrayWithSize(array, size, INTEGER); \
})

#define RAW_POINTER(x)	RAW(x)

#define asRRawArray(array) \
__extension__ \
({ \
    _asRPrimArray(array, RAW); \
})

#define asRRawArrayWithSize(array, size) \
__extension__ \
({ \
    _asRPrimArrayWithSize(array, size, RAW); \
})


#define asRNumericArray(array) \
__extension__ \
({ \
    _asRPrimArray(array, NUMERIC); \
})

#define asRNumericArrayWithSize(array, size) \
__extension__ \
({ \
    _asRPrimArrayWithSize(array, size, NUMERIC); \
})

#define asRLogicalArray(array) \
__extension__ \
({ \
    _asRPrimArray(array, LOGICAL); \
})

#define asRLogicalArrayWithSize(array, size) \
__extension__ \
({ \
    _asRPrimArrayWithSize(array, size, LOGICAL); \
})

/* now from R to C */

#define asCArrayRef(s, type, converter) \
__extension__ \
({ \
    asCArray(s, type, * converter); \
})
#define asCArray(s_array, type, converter) \
__extension__ \
({ \
    int i; \
\
    type* array = (type*)R_alloc(GET_LENGTH(s_array), sizeof(type)); \
\
    for (i = 0; i < GET_LENGTH(s_array); i++) { \
        array[i] = (type)converter(VECTOR_ELT(s_array, i)); \
    } \
\
    array; \
})

gboolean asCLogical(USER_OBJECT_ s_log);
int asCInteger(USER_OBJECT_ s_int);
guchar asCRaw(USER_OBJECT_ s_raw);
double asCNumeric(USER_OBJECT_ s_num);
const char * asCString(USER_OBJECT_ s_str);
char asCCharacter(USER_OBJECT_ s_char);

USER_OBJECT_ asRLogical(Rboolean);
USER_OBJECT_ asRInteger(int);
USER_OBJECT_ asRRaw(guchar);
USER_OBJECT_ asRNumeric(double);
USER_OBJECT_ asRCharacter(char c);
USER_OBJECT_ asRString(const char *);

char ** asCStringArray(USER_OBJECT_ svec); /* NULL TERMINATES */

void *getPtrValue(USER_OBJECT_);
USER_OBJECT_ toRPointer(void*, const char *name);

/* GGobi specific converters */

/* R to C */
ggobid *toGGobi(USER_OBJECT_ s_ggobi);
GGobiData *toData(USER_OBJECT_ d);
displayd *toDisplay(USER_OBJECT_ rdisplay);

/* Wrapping GGobi objects as SEXP's */
USER_OBJECT_ RS_datasetInstance(GGobiData *d) ;
USER_OBJECT_ RS_ggobiInstance(ggobid *gg);
USER_OBJECT_ RS_displayInstance(displayd *display);

#endif

