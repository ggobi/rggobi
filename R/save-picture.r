# Get GGobi displays
# Get's list of displays in the specified GGobi instance
# 
# @alias displays.ggobi
# @arguments GGobi object
displays <- function(x) UseMethod("displays", x)
displays.ggobi <- function(x) {
 rggobi:::.GGobiCall("getDisplays", .gobi = x)
}

as.RGtkObject <- function(x) {
 if(require(RGtk2)) class(x) <- c(class(x), "RGtkObject")
 x
}
get_RGtk2_display <- function(g, i) {
	disp <- as.RGtkObject(displays(g)[[i]]$ref)
}

save_display <- function(display, path="ggobi_display.png", filetype="png", plot.only = FALSE) {
	if (plot.only) {
		disp <- display$getChildren()[[2]]$getChildren()[[3]][["widget"]][["window"]]
	} else {
		disp <- display[["window"]]
	}
	
	disp_size <- disp$getSize()
	disp_pixbuf <- gdkPixbufGetFromDrawable(disp, NULL, 0, 0, 0, 0, disp_size$width, disp_size$height)
	
	disp_pixbuf$save(path, filetype)
}

#X g <- ggobi(mtcars)
#X save_display(get_RGtk2_display(g, 1), "test.png")
