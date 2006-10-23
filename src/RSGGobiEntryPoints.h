#ifndef RSGGOBI_ENTRY_POINTS
#define RSGGOBI_ENTRY_POINTS

void RS_GGOBI(getNumGGobiInstances)(glong *ans);

USER_OBJECT_ RS_GGOBI(getNumDisplays)(USER_OBJECT_ ggobiId);
USER_OBJECT_ RS_GGOBI(getNumberedKeyHandler)(USER_OBJECT_ ggobiId);
USER_OBJECT_ RS_GGOBI(setNumberedKeyHandler)(USER_OBJECT_ handler, USER_OBJECT_ sdescription, USER_OBJECT_ ggobiId);
USER_OBJECT_ RS_GGOBI(getNumPlotsInDisplay)(USER_OBJECT_ dpy);
SEXP RS_GGOBI(getDisplayType)(SEXP dpy, SEXP ggobiId);
USER_OBJECT_ RS_GGOBI(getDisplayTypes)();
USER_OBJECT_ RS_GGOBI(updateDisplay)(USER_OBJECT_ dpy, USER_OBJECT_ ggobiId);
USER_OBJECT_ RS_GGOBI(setRowNames)(USER_OBJECT_ names, USER_OBJECT_ indices, USER_OBJECT_ datasetId);
USER_OBJECT_ RS_GGOBI(getRowNames)(USER_OBJECT_ data);
USER_OBJECT_ RS_GGOBI(getRowsInPlot)(USER_OBJECT_ datasetId);

USER_OBJECT_ RS_GGOBI(getSampledIndices)(USER_OBJECT_ datasetId);
USER_OBJECT_ RS_GGOBI(setSampledIndices)(USER_OBJECT_ vals, USER_OBJECT_ datasetId);

USER_OBJECT_ RS_GGOBI(getExcludedIndices)(USER_OBJECT_ datasetId);
USER_OBJECT_ RS_GGOBI(setExcludedIndices)(USER_OBJECT_ vals, USER_OBJECT_ datasetId);


USER_OBJECT_ RS_GGOBI(setIMode)(USER_OBJECT_ name, USER_OBJECT_ ggobiId);
USER_OBJECT_ RS_GGOBI(setPMode)(USER_OBJECT_ name, USER_OBJECT_ ggobiId);
USER_OBJECT_ RS_GGOBI(getIModeName)(USER_OBJECT_ ggobiId);
USER_OBJECT_ RS_GGOBI(getPModeName)(USER_OBJECT_ ggobiId);

USER_OBJECT_ RS_GGOBI(setBrushColor)(USER_OBJECT_ cid, USER_OBJECT_ ggobiId);
USER_OBJECT_ RS_GGOBI(getBrushColor)(USER_OBJECT_ ggobiId);
USER_OBJECT_ RS_GGOBI(setBrushGlyph)(USER_OBJECT_ vals, USER_OBJECT_ ggobiId);
USER_OBJECT_ RS_GGOBI(getBrushGlyph)(USER_OBJECT_ ggobiId);
USER_OBJECT_ RS_GGOBI(setCasesHidden)(USER_OBJECT_ vals, USER_OBJECT_ which, USER_OBJECT_ datasetId);
USER_OBJECT_ RS_GGOBI(getCasesHidden)(USER_OBJECT_ datasetId);
USER_OBJECT_ RS_GGOBI(addVariable)(USER_OBJECT_ vals, USER_OBJECT_ name, USER_OBJECT_ levels,
				   USER_OBJECT_ values, 
				   USER_OBJECT_ datasetId);

USER_OBJECT_ RS_GGOBI(varpanel_populate)(USER_OBJECT_ datasetId);


USER_OBJECT_ RS_GGOBI(GGobiData_init)(USER_OBJECT_ cleanup, USER_OBJECT_ datasetId);

USER_OBJECT_
RS_GGOBI(setVariableValues)(USER_OBJECT_ vals, USER_OBJECT_ rowIds,
                            USER_OBJECT_ colId, USER_OBJECT_ update, 
			    USER_OBJECT_ datasetId);

USER_OBJECT_ RS_GGOBI(setVariableNames)(USER_OBJECT_ vars, USER_OBJECT_ names, USER_OBJECT_ datasetId);

USER_OBJECT_ RS_GGOBI(raiseOrLowerDisplays)(USER_OBJECT_ displays, USER_OBJECT_ iconify, USER_OBJECT_ raise, USER_OBJECT_ ggobiId);
USER_OBJECT_ RS_GGOBI(getVariables)(USER_OBJECT_ which, USER_OBJECT_ datasetId);

USER_OBJECT_ RS_GGOBI(getMainWindow)(USER_OBJECT_ gobiId);
USER_OBJECT_ RS_GGOBI(getCurrentDisplay)(USER_OBJECT_ gobiId);
USER_OBJECT_ RS_GGOBI(getPlugins)(USER_OBJECT_ gobiId);
USER_OBJECT_ RS_GGOBI(getPluginInfo)(USER_OBJECT_ gobiId);
USER_OBJECT_ RS_GGOBI(getDatasetNames)(USER_OBJECT_ gobiId);
USER_OBJECT_ RS_GGOBI(getVariableNames)(USER_OBJECT_ transformed, USER_OBJECT_ datasetId);


USER_OBJECT_ RS_GGOBI(getActiveColorScheme)(USER_OBJECT_ gobiId);
USER_OBJECT_ RS_GGOBI(setActiveColorScheme)(USER_OBJECT_ id, USER_OBJECT_ gobiId);
USER_OBJECT_ RS_GGOBI(addColorScheme)(USER_OBJECT_ sscheme, USER_OBJECT_ name, USER_OBJECT_ overwrite, USER_OBJECT_ gobiId);
USER_OBJECT_ RS_GGOBI(setDisplaySize)(USER_OBJECT_ newDims, USER_OBJECT_ displayId, USER_OBJECT_ ggobiID);
USER_OBJECT_ RS_GGOBI(setEdges)(USER_OBJECT_ x, USER_OBJECT_ y, USER_OBJECT_ append, USER_OBJECT_ datasetId);
USER_OBJECT_ RS_GGOBI(getDatasetRecordIds)(USER_OBJECT_ datasetId);

USER_OBJECT_ RS_GGOBI(getData)(USER_OBJECT_ datasetId);
USER_OBJECT_ RS_GGOBI(setFile)(USER_OBJECT_ fileName, USER_OBJECT_ smode, USER_OBJECT_ add, USER_OBJECT_ gobiId);

USER_OBJECT_ RS_GGOBI(getNumDatasets)(USER_OBJECT_ gobiID);
USER_OBJECT_ RS_GGOBI(getSourceName)(USER_OBJECT_ datasetId);
USER_OBJECT_ RS_GGOBI(getDisplayDataset)(USER_OBJECT_ dpy);
USER_OBJECT_ RS_GGOBI(getDataset)(USER_OBJECT_ which, USER_OBJECT_ gobiID);
USER_OBJECT_ RS_GGOBI(getCurrentDisplayType)(USER_OBJECT_ ggobiId);
USER_OBJECT_ RS_GGOBI(getDisplayOptions)(USER_OBJECT_ which);
USER_OBJECT_ RS_GGOBI(setDisplayOptions)(USER_OBJECT_ which, USER_OBJECT_ values);

USER_OBJECT_ RS_GGOBI(getActivePlot)(USER_OBJECT_ ggobiId);
USER_OBJECT_ RS_GGOBI(setActivePlot)(USER_OBJECT_ which, USER_OBJECT_ ggobiId);
USER_OBJECT_ RS_GGOBI(closeDisplay)(USER_OBJECT_ ref, USER_OBJECT_ ggobiId);
USER_OBJECT_ RS_GGOBI(getDescription)(USER_OBJECT_ ggobiId);

USER_OBJECT_ RS_GGOBI(setCaseGlyphs)(USER_OBJECT_ vals, USER_OBJECT_ sizes, USER_OBJECT_ which, 
				     USER_OBJECT_ datasetId);
USER_OBJECT_ RS_GGOBI(setCaseColors)(USER_OBJECT_ vals, USER_OBJECT_ which, USER_OBJECT_ datasetId);

USER_OBJECT_ RS_GGOBI(getCaseColors)(USER_OBJECT_ ids, USER_OBJECT_ datasetId);
USER_OBJECT_ RS_GGOBI(getCaseGlyphs)(USER_OBJECT_ ids, USER_OBJECT_ datasetID);
USER_OBJECT_ RS_GGOBI(close)(USER_OBJECT_ gobi);
USER_OBJECT_ RS_GGOBI(isValid)(USER_OBJECT_ gobi);

USER_OBJECT_ RS_GGOBI(createEdgeDataset)(USER_OBJECT_ numPoints, USER_OBJECT_ sname, USER_OBJECT_ ggobiId);
USER_OBJECT_ RS_GGOBI(getConnectedEdges)(USER_OBJECT_ edgesetId, USER_OBJECT_ datasetId);
USER_OBJECT_ RS_GGOBI(getSymbolicEdges)(USER_OBJECT_ edgesetId);
USER_OBJECT_ RS_GGOBI(getMainMenubar)(USER_OBJECT_ gobi);
USER_OBJECT_ RS_GGOBI(getDisplayMenubar)(USER_OBJECT_ display, USER_OBJECT_ gobiId);
USER_OBJECT_ RS_GGOBI(setDisplayEdges)(USER_OBJECT_ dpys, USER_OBJECT_ edgeData, USER_OBJECT_ directed, USER_OBJECT_ On, USER_OBJECT_ ggobiId);

USER_OBJECT_ RS_GGOBI(setBrushSize)(USER_OBJECT_ dims, USER_OBJECT_ datasetId);
USER_OBJECT_ RS_GGOBI(setBrushLocation)(USER_OBJECT_ loc, USER_OBJECT_ datasetId);

USER_OBJECT_ RS_GGOBI(getColorSchemes)(USER_OBJECT_ gobiId);

USER_OBJECT_ RS_GGOBI(init)(USER_OBJECT_ args, USER_OBJECT_ createInstance);


SEXP RS_GGOBI(createPlots)(SEXP plotDescList, SEXP dims, SEXP cells, SEXP gobiId, SEXP rdisplay, SEXP dataset);
USER_OBJECT_ RS_GGOBI(setDisplayVariables)(USER_OBJECT_ vars, USER_OBJECT_ varPrev, USER_OBJECT_ dpy);

void RS_GGOBI(getNumGGobiInstances)(glong *ans);
void RS_GGOBI(flush)();

USER_OBJECT_ RS_GGOBI(setVariableTypes)(USER_OBJECT_ vars, USER_OBJECT_ values, USER_OBJECT_ datasetId);
USER_OBJECT_ RS_GGOBI(setAsTimeVariables)(USER_OBJECT_ vars, USER_OBJECT_ values, USER_OBJECT_ datasetId);

USER_OBJECT_ RS_GGOBI(getIModeNames)(USER_OBJECT_ s_type_name);
USER_OBJECT_ RS_GGOBI(getPModeNames)(USER_OBJECT_ s_type_name);

USER_OBJECT_ RS_GGOBI(setPlotScale)(USER_OBJECT_ x, USER_OBJECT_ y, USER_OBJECT_ rdisplay, USER_OBJECT_ plot);
USER_OBJECT_ RS_GGOBI(getPlotScale)(USER_OBJECT_ rdisplay, USER_OBJECT_ plot);

#endif
