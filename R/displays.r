# Get GGobi displays
# Get's list of displays in the specified GGobi instance
# 
# @alias displays.GGobi
# @arguments GGobi object
# TODO: add displays method for datasets
displays <- function(x) UseMethod("displays", x)
displays.GGobi <- function(x) {
 .GGobiCall("getDisplays", .gobi = x)
}

dataset.GGobiDisplay <- function(gd) {
 .GGobiCall("getDisplayDataset", gd)
}

# FIXME: how do we do this now from R?
ggobi.GGobiDisplay <- function(gd) {
	.GGobiCall("getGGobiForDisplay", gd)
}

print.GGobiDisplay <- function(gd) {
	print(class(gd)[1])
}

# Close display window
# 
# @arguments GGobiDisplay object to close
close.GGobiDisplay <- function(con, ...) {
	.GGobiCall("closeDisplay", con$ref, .gobi = con$ggobi)
}

summary.GGobiDisplay <- function(x) {
	list(type = class(x), dataset = dataset(x), ggobi = ggobi(x))
}

# This tells the number of plots within  a given display 
# window within a ggobi instance.
length.GGobiDisplay <- function(gd) {
	.GGobiCall("getNumPlotsInDisplay",  gd)
}



#X g <- ggobi(mtcars)
#X save_display(get_RGtk2_display(g, 1), "test.png")
plot.GGobiDisplay <- function(display, path="ggobi_display.png", filetype="png", plot.only = FALSE) {
	display <-
	if (plot.only) {
		disp <- display$getChildren()[[2]]$getChildren()[[3]][["widget"]][["window"]]
	} else {
		disp <- display[["window"]]
	}
	
	disp_size <- disp$getSize()
	disp_pixbuf <- gdkPixbufGetFromDrawable(disp, NULL, 0, 0, 0, 0, disp_size$width, disp_size$height)
	
	disp_pixbuf$save(path, filetype)
}

display <- function(x, ...) UseMethod("display", x)

display.GGobiData <- function(x, type="Scatterplot Display", vars=1:nrow(x)) {
	type <- ggobi_display_get_type(type)

	if(is.character(vars)) {
		vars <- match(vars, names(x))
		if(any(is.na(vars)))
		 stop("Incorrect variables specified")
	}

	.GGobiCall("createDisplay", type, as.integer(vars), x)
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
setDisplayOptions.GGobi <- function(points,
         axes, axesLabels, axesValues,
         directed, undirected, arrowheads,
         whiskers,
         current = NULL,
          display = 1, .gobi = ggobi_get()) {
  old <- getDisplayOptions.GGobi(display, .gobi=.gobi)
  
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

getDisplayOptions.GGobi <- function(which = 1, .gobi = ggobi_get()) {
  ans = .GGobiCall("getDisplayOptions", as.integer(which-1), .gobi = .gobi)
  class(ans) = "GGobiDisplayOptions"

  ans
}

# =========================================================
# Class methods
# =========================================================

ggobi_display_types <- function(.gobi = ggobi_get()) {
 .GGobiCall("getDisplayTypes", .gobi = .gobi)
}

ggobi_display_get_type <- function(type) {
	if(!inherits(type, "GType")) {
    types <- ggobi_display_types()
    id <- match(type, names(types))
    if(is.na(id)) {
      id <- match(type, sapply(types, names))
    }

    if(is.na(id))
      stop("Unrecognized plot type")

    type <- types[[id]]
  }
	type
}
