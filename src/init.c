#include "RSGGobi.h"
#include "GGobiAPI.h"
#include <R_ext/Rdynload.h>

#define CENTRY(a, n) {#a, (DL_FUNC) &RS_GGOBI(a), n}


static R_CMethodDef cmethods[] = {
    CENTRY(getNumGGobiInstances, 1),
    CENTRY(flush, 0)
//    CENTRY(removeVariables)

   , {NULL}
};


static R_CallMethodDef callMethods[] =
{
   CENTRY(init, 2),
   CENTRY(getBrushColor, 0),
   CENTRY(setBrushColor, 1),
   CENTRY(getBrushGlyph, 0),
   CENTRY(setBrushGlyph, 1),
   CENTRY(setBrushSize, 2),
   CENTRY(setBrushLocation, 2),
   CENTRY(getColorSchemes, 0),
   CENTRY(addColorScheme, 4),
   CENTRY(getActiveColorScheme, 1),
   CENTRY(setActiveColorScheme, 2),

   CENTRY(setDisplayWidth, 3),
   CENTRY(setEdges, 5),

   CENTRY(getVariables, 3),
   CENTRY(getData, 2),

   CENTRY(setFile, 4),
   CENTRY(getNumDatasets, 1),

   CENTRY(varpanel_populate, 2),
   CENTRY(getDisplayDataset, 2),

   CENTRY(getDataset, 2),

   CENTRY(getCurrentDisplayType, 1),
   CENTRY(getDisplayOptions, 2),
   CENTRY(setDisplayOptions, 3),

   CENTRY(getActivePlot, 1),
   CENTRY(setActivePlot, 2),
   CENTRY(closeDisplay, 2),
   CENTRY(getDatasetNames, 1),
   CENTRY(getDescription, 1),

   CENTRY(getVariableNames, 3),
   CENTRY(getCaseGlyphs, 3),
   CENTRY(setCaseGlyphs, 5),
   CENTRY(getCaseColors, 3),
   CENTRY(setCaseColors, 4),
/*   CENTRY(setMissing), */
   CENTRY(close, 1),
   CENTRY(isValid, 1),
   CENTRY(setIMode, 2),
   CENTRY(setPMode, 2),
   CENTRY(getPModeName, 1),
   CENTRY(getIModeName, 1),
   CENTRY(getCasesHidden, 2),
   CENTRY(setCasesHidden, 4),


   CENTRY(getMainMenubar, 1),
   CENTRY(getDisplayMenubar, 2),
#if 0
   CENTRY(setPlotRange),
   CENTRY(getPlotRange),
#endif
   CENTRY(getSymbolicEdges, 2),
   CENTRY(getConnectedEdges, 3),
   CENTRY(createEdgeDataset, 3),
   CENTRY(setDisplayEdges, 5),
   CENTRY(raiseOrLowerDisplays, 4),
   CENTRY(getCurrentDisplay, 1),
   CENTRY(getMainWindow, 1),

   CENTRY(setVariableNames, 4),
   CENTRY(setVariableValues, 6),
   CENTRY(addVariable, 6),
/*   CENTRY(getPlugins, 1),
   CENTRY(getPluginInfo, 1),*/


   CENTRY(getExcludedIndices, 2),
   CENTRY(setExcludedIndices, 3),

   CENTRY(getSampledIndices, 2),
   CENTRY(setSampledIndices, 3),


   CENTRY(getRowsInPlot, 2),

   CENTRY(getRowNames, 2),
   CENTRY(setRowNames, 4),

   CENTRY(getDisplayTypes, 0),


   CENTRY(getNumPlotsInDisplay, 2),
   CENTRY(getNumDisplays, 1),


   CENTRY(getNumberedKeyHandler, 1),
   CENTRY(setNumberedKeyHandler, 3),
   
   CENTRY(setVariableTypes, 4),
   CENTRY(setAsTimeVariables, 4)  

#if 0
   ,
   CENTRY(updateDisplay, 2),
   CENTRY(createPlots, 6),
   CENTRY(setPlotVariables, 4),
   CENTRY(getDisplayType, 2)
#endif

   , {NULL}
};

void
R_init_Rggobi(DllInfo *dll)
{
	g_debug("Registering symbols");

   R_registerRoutines(dll, cmethods, callMethods, NULL, NULL);
   R_useDynamicSymbols(dll, TRUE);
}
