# Manipulation of individual plots within a display

scatterplot.ggobi <-
function(x, y, .data = 1, .gobi = ggobi_get()) 
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
function(..., .data = 1, .gobi = ggobi_get()) 
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
function(x, y, .data = 1, .gobi = ggobi_get()) 
{
  if(!inherits(.data, "GGobiData")) {
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

summary.ggobiPlot <-
function(x)
{
	list(type = class(x), variables = variables(x), dataset = dataset(x), ggobi = ggobi(x))
}
