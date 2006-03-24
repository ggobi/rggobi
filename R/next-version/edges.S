setDisplayEdges.ggobi <- function(displays, edgeData, directed = FALSE, on = TRUE, .gobi = getDefaultGGobi()) {
 if(missing(displays))
    displays <- getDisplays.ggobi(.gobi = .gobi)

 if(inherits(displays, "ggobiDisplay"))
   displays <- list(displays)

 if(on) {
  if(missing(edgeData)) {
   datasets <- dataset(1:getNumDatasets.ggobi(.gobi=.gobi), .gobi = .gobi)
   e <- sapply(datasets, function(x) inherits(x, "ggobiEdgeDataset"))
   if(!any(e))
     stop("No edge data")

   k <- ((1:length(e))[e])[1] 
   edgeData <- datasets[[ k ]]
  } else {
   if(is.numeric(edgeData)) {
     edgeData <- .gobi[[edgeData]]
   }
  }

  if(!inherits(edgeData, "ggobiEdgeDataset"))
     stop("Not an edge dataset")  
 } else
  edgeData = NULL

 .GGobiCall("setDisplayEdges", displays, edgeData, as.logical(directed), as.logical(on), .gobi = .gobi)
}

