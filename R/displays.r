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

# right now the behavior of this function is exactly like that of the user
# clicking buttons on the variable selection panel. The elements of the
# 'vars' vector correspond to the X, Y, and Z buttons, respectively, if they exist.
# otherwise they spill over to the X button (ie for tours, scatterplot matrices).
# unfortunately this means we are not "setting" the variables but pressing buttons,
# which are very different things, since in some cases we are "adding" them.
# see notes below
setDisplayVariables.GGobiDisplay <-
# shouldn't .gobi default to ggobi(display)?
function(display, vars, .gobi = ggobi_get())
{
  # Get the indices of the variables the user has specified.
  varIds <- variable_index(dataset(display, .gobi = .gobi), vars)
  
  # I could make it so that the currently selected variables remain selected
  # if they are specified in the 'vars' vector - ie, they are not toggled
  # But the main problem is deselecting the ones that we no longer want selected
  
  # In this case,
  # we could try to "select" them first, so that if they are toggles (ie a tour) 
  # they are toggled off but this doesn't work otherwise
  
  # there's no nice way to get the variables in the display either - since
  # there's no model for the varpanel, we'd have to query each button
  
  # maybe if this was renamed to toggleDisplayVariables the semantics would be
  # such that we wouldn't have to worry about this
  
  #prev <- getDisplayVariables(display)
  #varIds <- c(prev[!(prev %in% varIds)], varIds)
  
  .GGobiCall("setDisplayVariables", varIds, display, .gobi= .gobi)
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
