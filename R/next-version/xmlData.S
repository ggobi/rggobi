dataDescriptionHandlers <-
function()
{
  Data <- list()
  class(Data) <- "XMLDataCollectionInfo"

  count <- 0

  data <- function(node, ...) {
    d <- list()
    
    d[["description"]] <- xmlValue(node[["description"]])
    d[["variables"]] <- node[["variables"]]
    

    d[["count"]] <- count
    count <<- 0

    class(d) <- "DatasetInfo"
    name <- xmlAttrs(node)[["name"]]    
    Data[[name]] <<- d    
    
    NULL
  }


  realvariable <-
    function(node, ...) {
       var <- realvariableInfo(node)

       var
    }
  
  variables <- function(node, ...) {
    els <- xmlChildren(node)
    names(els) <- sapply(els, function(x) x$name)
    els
  }

  
  variable <- function(node, ..) {
     xmlAttrs(node)[["name"]]     
  }

 catvariable <- function(node, ...) {
   l <- xmlApply(node[["levels"]],
                 function(x) {
                   idx <- as.integer(xmlAttrs(x)[["value"]])
                   label <- xmlValue(x)
                   ans <- c(label)
                   names(ans) <- idx
                   ans
                 })
   names(l) <- rep(node[["name"]], length(l))
   l
 }

 
  
  list(data = data,
       variables = variables,
       realvariable = realVariableInfo.XMLNode,
       categoricalvariable = categoricalVariableInfo.XMLNode,
       records = function(x, ...) NULL,
       record = function(x, ...) { count <<- count + 1; NULL},
       Data = function() Data)
}


realVariableInfo <-
function(x, ...)
{
 UseMethod("realVariableInfo")
}

categoricalVariableInfo <-
function(x, ...)
{
 UseMethod("realVariableInfo")
}

variableInfo <-
function(x, ...)
{
 UseMethod("variableInfo")
}  

realVariableInfo.XMLNode <-
function(x, ..., numericFields = c("min", "max"))
{
       at <- xmlAttrs(x)
       var <- variableInfo(x, ..., className = "RealVariableInfo")
       for(i in numericFields) {
         var[[i]] <-  ifelse(i %in% names(at), as.numeric(at[[i]]), NA)         
       }

       var
}


categoricalVariableInfo.XMLNode <-
function(x, ..., numericFields = c("min", "max"))
{
       var <- variableInfo(x, ..., className = "CategoricalVariableInfo")

       var$levels <- xmlSApply(x[["levels"]], function(x) as.integer(xmlAttrs(x)[["value"]]))
       
       names(var$levels) <- xmlApply(x[["levels"]], function(x) xmlValue(x))

       var
}


variableInfo.XMLNode <-
function(x, ..., className = NULL)
{  
       var <- list()

       at <- xmlAttrs(x)       

       var$name <- at[["name"]]
       for(i in c("nickname")) {
         var[[i]] <-  ifelse(i %in% names(at), at[[i]], "")         
       }
      
       var$description <- x[["description"]]
       var$quickHelp <- x[["quickHelp"]]       

       class(var) <- c(className, "VariableInfo")

       var
}
