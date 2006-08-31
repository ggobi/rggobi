# View displays
# See methods for details
# 
# @keyword internal 
displays <- function(x) UseMethod("displays", x)

# Get GGobi displays
# Gets list of displays in the specified GGobi instance
# 
# A display basically corresponds to a window in GGobi.  A display
# may contain mutliple plots within it.  For example, the scatterplot
# matrix contains $p \times p$ plots.
# 
# Use this function to obtain a reference to a display (they are 
# numbered in the order they are created) so you can change
# display mode, set variables (\code{\link{variables.ggobi<-}}),
# or save a static image to disk.
# 
# @seealso \code{\link{display}} to create displays 
# @alias displays.GGobi
# @arguments GGobi object
#X g <- ggobi(mtcars)
#X displays(g)
#X display(g[1])
#X displays(g)
displays.GGobi <- function(x) {
 .GGobiCall("getDisplays", .gobi = x)
}

# Get display dataset
# Returns a link to the GGobiData (dataset) object associated with this display.
# 
# See \code{\link{[.GGobi}} for more information on 
# 
# @arguments GGobiDisplay object
# @keyword manip 
#X g <- ggobi(mtcars)
#X d <- displays(g)[1]
#X dataset(d)
dataset.GGobiDisplay <- function(gd) {
 .GGobiCall("getDisplayDataset", gd, .gobi=ggobi(gd))
}

# Get display GGobi
# Returns a link to the GGobi object associated with this display
# 
# @arguments GGobiDisplay object
# @keyword internal
ggobi.GGobiDisplay <- function(gd) {
	.GGobiCall("getGGobiForDisplay", gd)
}

# Print method for GGobiDisplay
# Shows display type (first element in class vector)
# 
# @arguments GGobiDisplay object
# @keyword internal 
print.GGobiDisplay <- function(gd) {
	print(class(gd)[1])
}

# Close display
# Closes the referenced display.  The R variable will be invalid after this call.
# 
# @arguments GGobiDisplay object to close
# @keyword internal 
#X g <- ggobi(mtcars)
#X displays(g)
#X close(displays(g)[[1]])
#X displays(g)
close.GGobiDisplay <- function(con, ...) {
	.GGobiCall("closeDisplay", con)
}

# Length method for GGobiDisplay
# Returns the number of plots within a given display
# 
# @arguments GGobiDisplay object to close
# @keyword internal 
length.GGobiDisplay <- function(gd) {
	.GGobiCall("getNumPlotsInDisplay",  gd)
}


# Save picture of plot (and window) to disk
# This allows you to make a static copy of a GGobiDisplay.
# 
# If you want to make publicaiton quality graphics, you should
# probably use the DescribeDisplay plugin and package.   This
# will recreate a GGobiDisplay in R, and so can produce high-quality
# vector (eg. pdf) output.  See \url{http://www.ggobi.org/describe-display}
# for more information
# 
# @arguments GGobiDisplay to save
# @arguments path to save to
# @arguments type of file to save
# @arguments if TRUE, save only plot, otherwise save surrounding GUI elements as well
# @keyword plot 
#X g <- ggobi(mtcars)
#X save_display(displays(g)[[1]], "test.png")
ggobi_display_save_picture <- function(display, path="ggobi_display.png", filetype="png", plot.only = FALSE) {
	if (!require("RGtk2")) stop("RGtk2 required to save windows images to disk", .call=FALSE)

	display <- as.RGtkObject(display)	
	if (plot.only) {
		disp <- display$getChildren()[[2]]$getChildren()[[3]][["widget"]][["window"]]
	} else {
		disp <- display[["window"]]
	}
	
	disp_size <- disp$getSize()
	disp_pixbuf <- gdkPixbufGetFromDrawable(disp, NULL, 0, 0, 0, 0, disp_size$width, disp_size$height)
	
	disp_pixbuf$save(path, filetype)
}

# Create a new display
# 
# @keyword internal 
display <- function(x, ...) UseMethod("display", x)

# Create a new display
# Create a new display for the GGobiData object.  
#
# This function allows you to create a new display from
# a GGobiData object.  You will need to specify the type of display you 
# want ("Scatterplot Display", "Scatterplot Matrix" and  "Parallel Coordinates
# Display" are the most common), and which variables the plot
# should be intialised with.  Specifying more than two variables only makes
# sense for scatterplot matrices and pcps.
# 
# Many of the plots used in GGobi (eg. the tours and densities plots) are
# special modes of the scatterplot display.  You will need to create a 
# new scatterplot display, change the projection mode to what you want, 
# and then set the variables.  Hopefully this will be improved in a future
# version of rggobi.
#  
# @seealso \code{\link{ggobi_display_types}} for a list of display types
# @keyword dynam 
#X g <- ggobi(mtcars)
#X display(g[1])
#X display(g[1], vars=c(4,5))
#X display(g[1], vars=c("drat","hp"))
#X display(g[1], "Parallel Coordinates Display")
#X display(g[1], "Scatterplot Matrix")
display.GGobiData <- function(x, type="Scatterplot Display", vars=1:ncol(x)) {
	type <- ggobi_display_get_type(type)
	vars <- variable_index(x, vars)

	.GGobiCall("createDisplay", type, vars, x)
}


# Get variables
# 
# @keyword internal 
variables <- function(x, ...) UseMethod("variables", x)

# Set variables
# 
# @keyword internal 
"variables<-" <- function(x, value) UseMethod("variables<-", x)

# Get display variables
# List the variables used in a given display
# 
# There are three types of variables in GGobi displays: 
# x, y, z, which correspond to the labels on the toggle buttons
# in GGobi.  Most plots have a constrained set of possible options.
# For example, in tours you can only set x variables, and you must 
# have at least three.  Or in the rotation plot, you need exactly
# one x, y, and z variable. 
# 
# @arguments GGobiDisplay object
# @keyword dynamic 
# @seealso \code{\link{variable<-.GGobiDataset}} for examples
variables.GGobiDisplay <- function(x) {
  vars <- .GGobiCall("getDisplayVariables", x, .gobi=ggobi(x))
  # convert to names?
  #vars[[1]] <- colnames(dataset(display))[vars[[1]] + 1]
  split(vars[[1]]+1, factor(vars[[2]], levels = c("X", "Y", "Z")))
}

# Set display variables 
# Set display variables with a list of x, y, and z component variable indices.
# 
# There are three types of variables in GGobi displays: 
# x, y, z, which correspond to the labels on the toggle buttons
# in GGobi.  Most plots have a constrained set of possible options.
# For example, in tours you can only set x variables, and you must 
# have at least three.  Or in the rotation plot, you need exactly
# one x, y, and z variable. 
#
# Currently, there is no checking done to ensure that you are
# sending a sensible set of variables for the given display type.
# Generally, any invalid choices will be silently ignored.
# 
# @arguments GGobiDisplay object
# @arguments list with x, y and z components
# @keyword dynamic 
#X g <- ggobi(mtcars)
#X d <- display(g[1], "Parallel Coordinates Display")
#X variables(d)
#X variables(d) <- list(x=1:8)
#X variables(d)
"variables<-.GGobiDisplay" <- function(x, value) {
	# get the old vars
	prev_vars <- getDisplayVariables(x)

	# Get the indices of the variables the user has specified.
	d <- dataset(x)

	# build a list of variable indices for each of x, y, and z
	vars <- lapply(value, function(vars) variable_index(d, vars))

	# find the vars we don't want anymore
	prev_vars <- lapply(1:length(vars), function(i) {
	  pv <- variable_index(d, prev_vars[[i]]) 
	  pv[!(pv %in% vars[[i]])]
	})

	.GGobiCall("setDisplayVariables", vars, prev_vars, x, .gobi=ggobi(x))
	x
}

# =========================================================
# Class methods
# =========================================================

# GGobiDisplay types
# Get list of GGobiDisplay types.  An instance of GGobi must be open
# 
# @arguments GGobi reference
# @seealso \code{\link{ggobi_display_get_type}}
# @keyword internal
ggobi_display_types <- function() {
 .GGobiCall("getDisplayTypes", .gobi = ggobi_get())
}

# Convert plot name to GGobi plot type
# Used to convert between friendly plot name and internal GGobi name.
# 
# @keyword internal 
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
