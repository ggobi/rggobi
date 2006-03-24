#
# Read color schemes from an XML file.
#
# This is cool but is it really necessary?

readXMLColorSchemes <-
function(uri, handlers = xmlColorSchemeHandlers())
{
  require(XML)
  xmlTreeParse(uri, handlers = handlers)
  handlers$getData()
}  

xmlColorSchemeHandlers <-
function()
{
 schemes <- list()


 getColor <-
   function(x) {
     # Handle the max and min here also!
             vals <- list()
             xmlSApply(x, function(z) {
                vals[[xmlName(z)]] <<- as.numeric(xmlValue(z[[1]]))
              })

             mx <- max(c(unlist(vals), 1.0))
             ats <- xmlAttrs(x)
             if("max" %in% names(ats)) {
               vals$max <- mx <- as.numeric(ats[["max"]])
             } else {
               if(mx > 1.0)
                 warning(paste("RGB value", mx, "greater than 1."))
             }


             
             vals$value <- rgb(vals$red/mx, vals$green/mx, vals$blue/mx)
             vals
           }

 colormap <- function(node, ...) {
  at <- xmlAttrs(node)

  scheme <- list()
  desc <- node[["description"]]
  if(!is.null(desc))
    scheme$description <- xmlValue(desc)


  scheme$colors <- xmlApply(node[["foreground"]],  getColor)
  names(scheme$colors) <- xmlSApply(node[["foreground"]],
                                    function(x) {
                                       at <- xmlAttrs(x)
                                       ifelse("name" %in% names(at), at[["name"]], "")
                                       })

  for(i in c("background", "annotations")) {
    scheme[[i]] <- getColor(node[[i]][[1]])  
  }

  type <- paste(at[["type"]], "ColorScheme", sep="")

  class(scheme) <- c(type, "ColorScheme")
  schemes[[at[["name"]]]] <<- scheme
  NULL
 }

 list(colormap = colormap, getData = function() schemes)
}  
