
setPlotVariables.ggobiDisplay <-
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
