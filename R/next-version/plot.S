# Manipulation of individual plots within a display

scatterplot.ggobi <-
function(x, y, .data = 1, .gobi = getDefaultGGobi()) 
{
 if(missing(y) && length(x)  > 1) {
   y <- x[2]
   x <- x[1]
 }
    
  
  x <- getVariableIndex.ggobi(x, y, .data = .data, .gobi = .gobi) - 1

  if(mode(.data) == "numeric")
    .data <- as.integer(.data - 1)

 .GGobiCall("newScatterplot", as.integer(x), .data, .gobi = .gobi)
}

parcoords.ggobi <-
function(..., .data = 1, .gobi = getDefaultGGobi()) 
{
  x <- getVariableIndex.ggobi(..., .data=.data, .gobi = .gobi)

  if(length(x) == 0)
    stop("No variable specified in parcoords.ggobi")

  if(mode(.data) == "numeric")
    .data <- as.integer(.data - 1)

 .GGobiCall("newParcoords", as.integer(x-1), .data, .gobi = .gobi)
}


scatmat.ggobi <-
#
#
#
#
function(x, y, .data = 1, .gobi = getDefaultGGobi()) 
{
  if(!inherits(.data, "ggobiDataset")) {
    .data <- dataset(.data, .gobi=.gobi)
  }

 if(missing(x)) {
   x <- as.integer(seq(1, length=ncol(.data)))
 }

 if(mode(x) != "numeric")
   x <- getVariableIndex.ggobi(x, .data = .data, .gobi = .gobi) 

 if(missing(y)) {
   y <- x
 } else {
  if(mode(y) != "numeric")	
    y <- getVariableIndex.ggobi(y, .data = .data, .gobi = .gobi)
 }

 if(any(is.na(x)) | any(is.na(y)))
   stop("No missing values allowed")


 .GGobiCall("newScatmat", as.integer(x-1), as.integer(y-1), .data, .gobi = .gobi)
}

setPlotVariables.ggobi <-
#
# In this version, one cannot alter the variables
# in a programmatically generated display that is
# embedded within another display's window.
#
function(..., display = 1, .gobi = getDefaultGGobi(), plots = NULL)
{
 if(is.numeric(display))
   display <- as.integer(display)

   # find out what type of plot we are dealing with.
   # won't work for hybrid types.
 type <- getPlotType.ggobi(display, .gobi = as.integer(.gobi))

   # Determine the number of splots within this display.
 if(missing(plots)) {
   numPlots <- getPlotCount.ggobi(display, .gobi)
   plots <- 1:numPlots
 } else {
   plots <- as.integer(plots)
   numPlots <- length(plots)
 }

   # We need the dataset associated with the display.
  data <- dataset(display, .gobi = .gobi)
  varNames <- names(data)

   # Now, get the indices of the variables the user has specified.
 varIds <- as.integer(getVariableIndex.ggobi(..., .data = data, .gobi = .gobi) -1)

   # Since we are setting these for the individual sub-plots,
   # determine how we skip over the variables as we iterate
   # over the plots.
 offset <- switch(type,"scatterplot"=2, "parallel coordinates plot"=1,
                        "scatterplot matrix"=2) 


 
 olds <- vector("list", numPlots)
 for(i in 1:numPlots) {
   if(length(varIds) >= offset) {
     tmp <- as.integer(varIds[1:offset])
     olds[[i]] <- .GGobiCall("setPlotVariables", tmp, ifelse(is.integer(display),as.integer(display-1),display), as.integer(i-1), .gobi= .gobi)
  
       # Put names on these indices
     names(olds[[i]]) <- varNames[olds[[i]]]     
     varIds <- varIds[-c(1:offset)]
   }
  }

 .GGobiCall("updateDisplay", as.integer(display-1), .gobi=.gobi)
 return(olds)
}

summary.ggobiPlot <-
function(x)
{
	list(type = class(x), variables = variables(x), dataset = dataset(x), ggobi = ggobi(x))
}
