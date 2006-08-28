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

# FIXME: inccorect
dataset.GGobiDisplay <- function(gd) {
 .GGobiCall("getDisplayDataset", gd, .gobi=ggobi(gd))
}

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
	.GGobiCall("closeDisplay", con)
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


goptions <- function(x) UseMethod("goptions", x)

# Set defaults for display options
#
#
"goptions<-.GGobiGGobi" <- function(x, value) {
	goptions.GGobiDisplay(NULL) <- value
}


# Changes the settings of the display options for a
# collection of plots
# 
# Possible options:
# 
#  * "Show Points"
#  * "Show axes"
#  * "Show tour axes"
#  * "Show axes labels"
#  * "Directed edges"
#  * "Undirected edges"
#  * "Arrowheads"
#  * "Show whiskers" 
"goptions<-.GGobiDisplay" <- function(x, value) {
	old <- goptions(x)
	cur <- update(old, value)
	.GGobiCall("setDisplayOptions", cur, gd)

	cur
}

goptions.GGobiDisplay <- function(x) {
  .GGobiCall("getDisplayOptions", x)
}
goptions.GGobiGGobi <- function(x) {
  goptions.GGobiDisplay(NULL)
}

getDisplayVariables <-
function(display, .gobi = ggobi_get())
{
  vars <- .GGobiCall("getDisplayVariables", display, .gobi = .gobi)
  # convert to names?
  #vars[[1]] <- colnames(dataset(display))[vars[[1]] + 1]
  split(vars[[1]]+1, factor(vars[[2]], levels = c("X", "Y", "Z")))
}

setDisplayVariables.GGobiDisplay <-
# shouldn't .gobi default to ggobi(display)?
function(display, x = NULL, y = NULL, z = NULL, .gobi = ggobi_get())
{
  # get the old vars
  prev_vars <- getDisplayVariables(display, .gobi)
  
  # Get the indices of the variables the user has specified.
  d <- dataset(display)
  
  # build a list of variable indices for each of x, y, and z
  x_vars <- variable_index(d, x)
  y_vars <- variable_index(d, y)
  z_vars <- variable_index(d, z)
  vars <- list(x_vars, y_vars, z_vars)
  
  # find the vars we don't want anymore
  
  prev_vars <- lapply(1:length(vars), function(i) {
    pv <- variable_index(d, prev_vars[[i]]) 
    pv[!(pv %in% vars[[i]])]
  })
 
  .GGobiCall("setDisplayVariables", vars, prev_vars, display, .gobi= .gobi)
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
