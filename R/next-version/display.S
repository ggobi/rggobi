getDisplays.ggobi <-
function(describe = FALSE, .gobi = getDefaultGGobi())
{
  dpys = .GGobiCall("getDisplays", as.logical(describe), .gobi = .gobi)

  lapply(dpys, function(d)  { class(d$ref) = gtkObjectGetClasses(d$ref, FALSE)
                              d
                            })
  
}

# contains functions for manipulating GGobi displays

createDisplay.ggobi <- function(type, vars, .data = 1, .gobi = getDefaultGGobi()) {
  if(!inherits(type, "GtkType")) {
    types <- getDisplayTypes.ggobi()
    id <- match(type, names(types))
    if(is.na(id)) {
      id <- match(type, sapply(types, names))
    }

    if(is.na(id))
      stop("Unrecognized plot type")

    type <- types[[id]]
  }

  if(mode(.data) == "numeric" || mode(.data) == "character")
    .data <- dataset(.data, .gobi)

  if(is.character(vars)) {
     vars = match(vars, names(.data))
     if(any(is.na(vars)))
       stop("Incorrect variables specified")
  }

  .GGobiCall("createPlot", type, as.integer(vars), .data, .gobi = .gobi)
}  

setDisplayWidth.ggobi <- function(sz,  display = 1, .gobi = getDefaultGGobi() ) {
 .GGobiCall("setDisplayWidth", dims = as.integer(sz), as.integer(display - 1), .gobi = .gobi)
}

getDisplayWidth.ggobi <- function(display = 1, .gobi = getDefaultGGobi() ) {
  setDisplayWidth.ggobi(NULL, display, .gobi)
}

# Want a way to be able to create a display without
# creating the window.
#

getDisplayWindow.ggobi <- function(display = 1, .gobi = getDefaultGGobi(), expandClasses = TRUE) {
#
# This returns an reference to the GtkWindow object
# associated with the given display.
#
# The value of display should be either an integer
# identifying by index the display of interest within the GGobi
# instance (.gobi), or an object of class ggobiDisplay
#  
# I don't think that we should allow specifying the index of the display anymore

#  if(is.numeric(display)) {
#    display <- as.integer(display)
#    display <- getDisplays.ggobi(FALSE, .gobi=.gobi)[[display]]
#    
#  }

  if(!inherits(display, "ggobiDisplay"))
    stop("Need an object of class ggobiDisplay")
  
  dpy <- .Call(.ggobi.symbol("getDisplayWindow"), display, PACKAGE = "rggobi")
  dpy
}  

getPlotWidgets.ggobiDisplay <- function(dpy, expandClasses = TRUE)   {
#
# Get references to the GtkDrawingArea widgets
# of the splotd objects in the given display.
# The value of dpy should be an object of class
# ggobiDisplay and can be obtained using a call of the
# form
#    getDisplays.ggobi(FALSE)[[which]]
#  
 if(!inherits(dpy, "ggobiDisplay"))
    stop("Object must be of class ggobiDisplay")

 els <- .Call(.ggobi.symbol("getDisplayPlotWidgets"), dpy, PACKAGE = "rggobi")

 els
}

#
# This tells the type of plot(s) in a given display 
# window within a ggobi instance.
# 
getPlotType.ggobi <- function(display = 1, .gobi = getDefaultGGobi()) {
	.GGobiCall("getDisplayType",  as.integer(display-1), .gobi = .gobi)
}


# This tells the number of plots within  a given display 
# window within a ggobi instance.
getPlotCount.ggobi <- function(display = 1, .gobi = getDefaultGGobi()) {
	.GGobiCall("getNumPlotsInDisplay",  as.integer(display-1), .gobi = .gobi)
}


#
# Changes the settings of the display options for a
# collection of plots or the template options for future
# plots.
#
#
# The argument `which' identifies the GGobi display (this is an index
# into the ordered list of displays viewable via the hierarchical dislay
# tree). If this is negative, the Default options used when creating new
# plots will be modified.
#
# The return value is the current settings for the specified options.
#
# Need to sort current if specified.
#
setDisplayOptions.ggobi <- function(points,
         axes, axesLabels, axesValues,
         directed, undirected, arrowheads,
         whiskers,
         current = NULL,
          display = 1, .gobi = getDefaultGGobi()) {
  old <- getDisplayOptions.ggobi(display, .gobi=.gobi)
  
  if(is.null(current)) {
    current = old
  } else {
    if(length(names(current)) == 0) {
      stop("Need named components")
    } else {
      which = pmatch(names(current), names(old))
      if(any(is.na(which)))
        stop("Unknown element(s)", paste(names(current)[is.na(which)], sep=", "))
       # Now reorder current so that the names are
      tmp = old
      tmp[which] = current
      current = tmp
    }
  }

  if(!missing(points))
    current["Show Points"] <- as.logical(points)

  if(!missing(axes))
    current["Show axes"] <- as.logical(axes)

  if(!missing(axesValues))
    current["Show tour axes"] <- as.logical(axes)

  if(!missing(axesLabels))
    current["Show axes labels"] <- as.logical(axes)        
    
  if(!missing(directed))
    current["Directed edges"] <- as.logical(directed)

  if(!missing(undirected))
    current["Undirected edges"] <- as.logical(undirected)

    
  if(!missing(arrowheads))
    current["Arrowheads"] <- as.logical(arrowheads)

  if(!missing(whiskers)) 
    current["Show whiskers"] <- as.logical(whiskers)


  
  .GGobiCall("setDisplayOptions", as.integer(display - 1), current, .gobi = .gobi)

 return(old)
}

#
# Retrieve the display options for the 
# specified display. The default is to get
# the template default options which are used when
# creating a new display/plot.
# Alternatively, one can specify an integer which identifies
# the display (window) and its sub-plots.
# These are stored in ordered and can be examined using
# the hierarchical display tree accessed from the Plots
# menu item.
#
# which - the display number within the specified ggobi instance.
#

getDisplayOptions.ggobi <- function(which = 1, .gobi = getDefaultGGobi()) {
  ans = .GGobiCall("getDisplayOptions", as.integer(which-1), .gobi = .gobi)
  class(ans) = "GGobiDisplayOptions"

  ans
}

getDisplayDataset.ggobi <- function(dpy, .gobi = getDefaultGGobi()) {
 .GGobiCall("getDisplayDataset", dpy, .gobi = .gobi)
}

summary.ggobiDisplay <- function(x) {
	list(type = class(x), plots = lapply(plots(x), summary), dataset = dataset(x), ggobi = ggobi(x))
}

close.ggobiDisplay <- function(con, ...) {
	.GGobiCall("closeDisplay", con[["ref"]], .gobi = con[["ggobi"]])
}
close.ggobiDisplayDescription <- function(con, ...) {
	close(con[["display"]])
}
