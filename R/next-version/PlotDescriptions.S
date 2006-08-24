#
# These functions create a ``virtual'' description 
# of a ggobi plot without actually creating the plot.
# They act as templates allowing the user to create
# descriptions that can be resolved or instantiated
# into real ggobi plots.
#
# See plotLayout() which takes a collection of these
# plot descriptions and a layout specification and
# creates a new display with the instantiated plots.

parallelCoordDescription <-
function(..., .data = -1, .gobi=NULL)
{
  pl <- list(vars = c(...), ggobi=.gobi, .data=.data)
  class(pl) <- ifelse(length(pl$vars) > 1, "MultipleParallelCoordinates", "ParallelCoordinates")

 pl
}

ashDescription <-
function(x, .data = -1, .gobi=NULL)
{
  pl <- list(vars = x, ggobi=.gobi, .data=.data)
  class(pl) <- "GGobiAsh"

 pl
}

scatterplotDescription <-
function(x, y, .data = -1, .gobi=NULL)
{
  pl <- list(vars = c(x, y), ggobi=.gobi, .data=.data)
  class(pl) <- "ScatterPlot"

 pl
}

scatmatrixDescription <-
function(..., .data = -1, .gobi=NULL)
{
  pl <- list(vars = c(...), ggobi=.gobi, .data = .data)
  class(pl) <- "ScatmatrixPlot"

  pl
}

timeseriesDescription <-
function(..., .data = -1, .gobi = NULL)
{
 pl <- list(vars=c(...), ggobi=.gobi, .data = .data)
 class(pl) <- "TimeSeriesPlot"

 pl
}


plotList <-
function(..., mfrow = NULL, cells = NULL)
{
  plots <- list(plots = list(...), mfrow = mfrow, cells = cells)
  class(plots) <- "ggobiPlotList"

 return(plots)
}


resolvePlotDescription <- 
# 
# Transforms variable names into indices 
# by resolving them relative to the specified
# dataset within the given ggobi instance
#
function(desc, .data = 1, .gobi = ggobi_get(), isError = TRUE)
{
 if(is.null(desc))
   return(NULL)

 if(is.character(desc$vars)) {
   if(!is.null(desc$ggobi)) {
     .gobi <- desc$ggobi
   } else
     desc$ggobi <- .gobi

   if(desc$.data > -1) {
    .data <- desc$.data
   } else
    desc$.data <- .data

   nms <- getVariableNames.ggobi(.data = .data, .gobi= .gobi)

   idx <- match(desc$vars, nms)
   if(any(is.na(idx)) && isError) 
     stop(paste("Cannot resolve variable name", desc$vars[is.na(idx)],"in ggobi"))

   desc$vars <- idx
 } else {
   desc$vars <- as.integer(desc$vars)
   if(desc$.data < 0)
     desc$.data <- .data
   if(is.null(desc$ggobi)) 
     desc$ggobi <- .gobi
 }

 desc
}
