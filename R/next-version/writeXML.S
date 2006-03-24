#
# This can be used to write out an XML description of one or more
# data frames into GGobi's data format.
#

toGGobiXML <-
function(..., dom = xmlOutputDOM("ggobidata", attrs = c(count = length(args))))
{
  args <- list(...)

  for(i in 1:length(args)) {
    name <- names(args)[i]
    # if this is "", use the deparse() version
    datasetToGGobiXML(args[[i]], name, dom)
  }

  dom
}

datasetToGGobiXML <-
function(data, name, dom, description = NULL, asElements = TRUE)
{

  dom$addTag("data", attrs=c(name=name), close = FALSE)
  dom$addTag("description", description)

  dataframe = getData.ggobi(data)
  dom$addTag("variables", attrs=c(count = ncol(data)), close=FALSE)
  for(i in names(dataframe)) {
    if(inherits(dataframe[[i]], "factor")) {
      dom$addTag("categoricalvariable", attrs = c(name = i), close = FALSE)
        levs <- levels(dataframe[[i]])
        dom$addTag("levels", attrs = c(count=length(levs)), close = FALSE)
        for(j in 1:length(levs)) {
           dom$addTag("level", paste("\"", levs[j], "\"", sep=""), attrs= c(value=j))
        }
      dom$closeTag("levels")
      dom$closeTag("categoricalvariable")
    } else
      dom$addTag("realvariable", attrs = c(name = i))
  }
  dom$closeTag("variables")

  dom$addTag("records", attrs =c(count = nrow(data)), close = FALSE)

  rownames <- dimnames(data)[[1]]
  edges <- getEdges.ggobi(data)

  for(i in 1:nrow(data)) {

   if (nrow(edges) >= i) {
       recordAttrs <- c(id = i, source = edges[i,1], destination = edges[i,2])
   } else recordAttrs = c(label = rownames[i], id = rownames[i])

      # If we want to put <el>value</el><el>value</el> within the <record>
      # we'll have to do it one at a time!
   dom$addTag("record", attrs=recordAttrs, close = FALSE)
   if (is.matrix(data) || is.data.frame(data)) {
      if(asElements) {
         for(r in data[i,]) {
             tag <- switch(typeof(r), double="real", integer="int")
             dom$addTag(tag, r)
         }
      } else dom$addNode(xmlTextNode(paste(data[i,], collapse=" ")))
   }
   dom$closeTag("record")
  }
  dom$closeTag("records")

  dom$closeTag("data")
}

writeDatasetsXML <- function(filename, .data = 1, .gobi = getDefaultGGobi()) {
	refs <- lapply(.data, dataset, .gobi)
	.GGobiCall("writeDatasetsXML", as.character(filename), refs, .gobi = .gobi)
}
