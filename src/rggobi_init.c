#include <R.h>
#include <Rinternals.h>
#include <stdlib.h> // for NULL
#include <R_ext/Rdynload.h>

/* .C calls */
extern void RS_GGOBI_getNumGGobiInstances(void *);

/* .Call calls */
extern SEXP RS_GGOBI_addData(SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP);
extern SEXP RS_GGOBI_addVariable(SEXP, SEXP, SEXP, SEXP, SEXP, SEXP);
extern SEXP RS_GGOBI_close(SEXP);
extern SEXP RS_GGOBI_closeDisplay(SEXP, SEXP);
extern SEXP RS_GGOBI_createDisplay(SEXP, SEXP, SEXP, SEXP, SEXP);
extern SEXP RS_GGOBI_createPlots(SEXP, SEXP, SEXP, SEXP, SEXP, SEXP);
extern SEXP RS_GGOBI_datasetDim(SEXP, SEXP);
extern SEXP RS_GGOBI_getActiveColorScheme(SEXP);
extern SEXP RS_GGOBI_getCaseColors(SEXP, SEXP, SEXP);
extern SEXP RS_GGOBI_getCaseGlyphs(SEXP, SEXP, SEXP);
extern SEXP RS_GGOBI_getCaseIds(SEXP, SEXP);
extern SEXP RS_GGOBI_getCasesHidden(SEXP, SEXP);
extern SEXP RS_GGOBI_getConnectedEdges(SEXP, SEXP, SEXP);
extern SEXP RS_GGOBI_getData(SEXP, SEXP);
extern SEXP RS_GGOBI_getDataset(SEXP, SEXP);
extern SEXP RS_GGOBI_getDatasetNames(SEXP);
extern SEXP RS_GGOBI_getDescription(SEXP);
extern SEXP RS_GGOBI_getDisplayDataset(SEXP, SEXP);
extern SEXP RS_GGOBI_getDisplays(SEXP);
extern SEXP RS_GGOBI_getDisplayTypes(SEXP);
extern SEXP RS_GGOBI_getDisplayVariables(SEXP, SEXP);
extern SEXP RS_GGOBI_getDisplayWidget(SEXP, SEXP);
extern SEXP RS_GGOBI_getExcludedIndices(SEXP, SEXP);
extern SEXP RS_GGOBI_getGGobi(SEXP, SEXP);
extern SEXP RS_GGOBI_getGGobiForDisplay(SEXP, SEXP);
extern SEXP RS_GGOBI_getIModeName(SEXP, SEXP);
extern SEXP RS_GGOBI_getIModeNames(SEXP, SEXP);
extern SEXP RS_GGOBI_getMainWindow(SEXP);
extern SEXP RS_GGOBI_getNumPlotsInDisplay(SEXP, SEXP);
extern SEXP RS_GGOBI_getPModeName(SEXP, SEXP);
extern SEXP RS_GGOBI_getPModeNames(SEXP, SEXP);
extern SEXP RS_GGOBI_getRowNames(SEXP, SEXP);
extern SEXP RS_GGOBI_getSelectedIndices(SEXP, SEXP);
extern SEXP RS_GGOBI_getStructSizes(SEXP);
extern SEXP RS_GGOBI_getSymbolicEdges(SEXP, SEXP);
extern SEXP RS_GGOBI_getTourProjection(SEXP, SEXP, SEXP);
extern SEXP RS_GGOBI_getVariableNames(SEXP, SEXP, SEXP);
extern SEXP RS_GGOBI_getVersionInfo(SEXP);
extern SEXP RS_GGOBI_ggobi_find_data_file(SEXP, SEXP);
extern SEXP RS_GGOBI_init(SEXP, SEXP);
extern SEXP RS_GGOBI_isValid(SEXP);
extern SEXP RS_GGOBI_setActiveColorScheme(SEXP, SEXP);
extern SEXP RS_GGOBI_setCaseColors(SEXP, SEXP, SEXP, SEXP);
extern SEXP RS_GGOBI_setCaseGlyphs(SEXP, SEXP, SEXP, SEXP, SEXP);
extern SEXP RS_GGOBI_setCasesHidden(SEXP, SEXP, SEXP, SEXP);
extern SEXP RS_GGOBI_setDisplayEdges(SEXP, SEXP, SEXP, SEXP, SEXP);
extern SEXP RS_GGOBI_setDisplayVariables(SEXP, SEXP, SEXP, SEXP);
extern SEXP RS_GGOBI_setEdges(SEXP, SEXP, SEXP, SEXP, SEXP);
extern SEXP RS_GGOBI_setExcludedIndices(SEXP, SEXP, SEXP);
extern SEXP RS_GGOBI_setFile(SEXP, SEXP, SEXP, SEXP);
extern SEXP RS_GGOBI_setIDs(SEXP, SEXP, SEXP);
extern SEXP RS_GGOBI_setIMode(SEXP, SEXP, SEXP);
extern SEXP RS_GGOBI_setPMode(SEXP, SEXP, SEXP);
extern SEXP RS_GGOBI_setRowNames(SEXP, SEXP, SEXP, SEXP);
extern SEXP RS_GGOBI_setTourProjection(SEXP, SEXP, SEXP, SEXP);
extern SEXP RS_GGOBI_setVariableNames(SEXP, SEXP, SEXP, SEXP);
extern SEXP RS_GGOBI_setVariableValues(SEXP, SEXP, SEXP, SEXP, SEXP, SEXP);

static const R_CMethodDef CEntries[] = {
    {"RS_GGOBI_getNumGGobiInstances", (DL_FUNC) &RS_GGOBI_getNumGGobiInstances, 1},
    {NULL, NULL, 0}
};

static const R_CallMethodDef CallEntries[] = {
    {"RS_GGOBI_addData",              (DL_FUNC) &RS_GGOBI_addData,              8},
    {"RS_GGOBI_addVariable",          (DL_FUNC) &RS_GGOBI_addVariable,          6},
    {"RS_GGOBI_close",                (DL_FUNC) &RS_GGOBI_close,                1},
    {"RS_GGOBI_closeDisplay",         (DL_FUNC) &RS_GGOBI_closeDisplay,         2},
    {"RS_GGOBI_createDisplay",        (DL_FUNC) &RS_GGOBI_createDisplay,        5},
    {"RS_GGOBI_createPlots",          (DL_FUNC) &RS_GGOBI_createDisplay,        6},
    {"RS_GGOBI_datasetDim",           (DL_FUNC) &RS_GGOBI_datasetDim,           2},
    {"RS_GGOBI_getActiveColorScheme", (DL_FUNC) &RS_GGOBI_getActiveColorScheme, 1},
    {"RS_GGOBI_getCaseColors",        (DL_FUNC) &RS_GGOBI_getCaseColors,        3},
    {"RS_GGOBI_getCaseGlyphs",        (DL_FUNC) &RS_GGOBI_getCaseGlyphs,        3},
    {"RS_GGOBI_getCaseIds",           (DL_FUNC) &RS_GGOBI_getCaseIds,           2},
    {"RS_GGOBI_getCasesHidden",       (DL_FUNC) &RS_GGOBI_getCasesHidden,       2},
    {"RS_GGOBI_getConnectedEdges",    (DL_FUNC) &RS_GGOBI_getConnectedEdges,    3},
    {"RS_GGOBI_getData",              (DL_FUNC) &RS_GGOBI_getData,              2},
    {"RS_GGOBI_getDataset",           (DL_FUNC) &RS_GGOBI_getDataset,           2},
    {"RS_GGOBI_getDatasetNames",      (DL_FUNC) &RS_GGOBI_getDatasetNames,      1},
    {"RS_GGOBI_getDescription",       (DL_FUNC) &RS_GGOBI_getDescription,       1},
    {"RS_GGOBI_getDisplayDataset",    (DL_FUNC) &RS_GGOBI_getDisplayDataset,    2},
    {"RS_GGOBI_getDisplays",          (DL_FUNC) &RS_GGOBI_getDisplays,          1},
    {"RS_GGOBI_getDisplayTypes",      (DL_FUNC) &RS_GGOBI_getDisplayTypes,      1},
    {"RS_GGOBI_getDisplayVariables",  (DL_FUNC) &RS_GGOBI_getDisplayVariables,  2},
    {"RS_GGOBI_getDisplayWidget",     (DL_FUNC) &RS_GGOBI_getDisplayWidget,     2},
    {"RS_GGOBI_getExcludedIndices",   (DL_FUNC) &RS_GGOBI_getExcludedIndices,   2},
    {"RS_GGOBI_getGGobi",             (DL_FUNC) &RS_GGOBI_getGGobi,             2},
    {"RS_GGOBI_getGGobiForDisplay",   (DL_FUNC) &RS_GGOBI_getGGobiForDisplay,   2},
    {"RS_GGOBI_getIModeName",         (DL_FUNC) &RS_GGOBI_getIModeName,         2},
    {"RS_GGOBI_getIModeNames",        (DL_FUNC) &RS_GGOBI_getIModeNames,        2},
    {"RS_GGOBI_getMainWindow",        (DL_FUNC) &RS_GGOBI_getMainWindow,        1},
    {"RS_GGOBI_getNumPlotsInDisplay", (DL_FUNC) &RS_GGOBI_getNumPlotsInDisplay, 2},
    {"RS_GGOBI_getPModeName",         (DL_FUNC) &RS_GGOBI_getPModeName,         2},
    {"RS_GGOBI_getPModeNames",        (DL_FUNC) &RS_GGOBI_getPModeNames,        2},
    {"RS_GGOBI_getRowNames",          (DL_FUNC) &RS_GGOBI_getRowNames,          2},
    {"RS_GGOBI_getSelectedIndices",   (DL_FUNC) &RS_GGOBI_getSelectedIndices,   2},
    {"RS_GGOBI_getStructSizes",       (DL_FUNC) &RS_GGOBI_getStructSizes,       1},
    {"RS_GGOBI_getSymbolicEdges",     (DL_FUNC) &RS_GGOBI_getSymbolicEdges,     2},
    {"RS_GGOBI_getTourProjection",    (DL_FUNC) &RS_GGOBI_getTourProjection,    3},
    {"RS_GGOBI_getVariableNames",     (DL_FUNC) &RS_GGOBI_getVariableNames,     3},
    {"RS_GGOBI_getVersionInfo",       (DL_FUNC) &RS_GGOBI_getVersionInfo,       1},
    {"RS_GGOBI_ggobi_find_data_file", (DL_FUNC) &RS_GGOBI_ggobi_find_data_file, 2},
    {"RS_GGOBI_init"   ,              (DL_FUNC) &RS_GGOBI_init,                 2},
    {"RS_GGOBI_isValid",              (DL_FUNC) &RS_GGOBI_isValid,              1},
    {"RS_GGOBI_setActiveColorScheme", (DL_FUNC) &RS_GGOBI_setActiveColorScheme, 2},
    {"RS_GGOBI_setCaseColors",        (DL_FUNC) &RS_GGOBI_setCaseColors,        4},
    {"RS_GGOBI_setCaseGlyphs",        (DL_FUNC) &RS_GGOBI_setCaseGlyphs,        5},
    {"RS_GGOBI_setCasesHidden",       (DL_FUNC) &RS_GGOBI_setCasesHidden,       4},
    {"RS_GGOBI_setDisplayEdges",      (DL_FUNC) &RS_GGOBI_setDisplayEdges,      5},
    {"RS_GGOBI_setDisplayVariables",  (DL_FUNC) &RS_GGOBI_setDisplayVariables,  4},
    {"RS_GGOBI_setEdges",             (DL_FUNC) &RS_GGOBI_setEdges,             5},
    {"RS_GGOBI_setExcludedIndices",   (DL_FUNC) &RS_GGOBI_setExcludedIndices,   3},
    {"RS_GGOBI_setFile",              (DL_FUNC) &RS_GGOBI_setFile,              4},
    {"RS_GGOBI_setIDs",               (DL_FUNC) &RS_GGOBI_setIDs,               3},
    {"RS_GGOBI_setIMode",             (DL_FUNC) &RS_GGOBI_setIMode,             3},
    {"RS_GGOBI_setPMode",             (DL_FUNC) &RS_GGOBI_setPMode,             3},
    {"RS_GGOBI_setRowNames",          (DL_FUNC) &RS_GGOBI_setRowNames,          4},
    {"RS_GGOBI_setTourProjection",    (DL_FUNC) &RS_GGOBI_setTourProjection,    4},
    {"RS_GGOBI_setVariableNames",     (DL_FUNC) &RS_GGOBI_setVariableNames,     4},
    {"RS_GGOBI_setVariableValues",    (DL_FUNC) &RS_GGOBI_setVariableValues,    6},
    {NULL, NULL, 0}
};

void R_init_rggobi(DllInfo *dll)
{
    R_registerRoutines(dll, CEntries, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
