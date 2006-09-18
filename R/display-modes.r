
"imode<-" <- function(x, value) {
	if (!(value %in% imodes(x))) stop("imode must be one of: ", paste(imodes(x), collapse=", "))
	.GGobiCall("setIMode",  as.character(value), x)
	x
}
"pmode<-" <- function(x, value) {
	if (!(value %in% pmodes(x))) stop("pmode must be one of: ", paste(pmodes(x), collapse=", "))
	.GGobiCall("setPMode", as.character(value), x)
	x
}

# Projection mode
# Get the projection mode from the GGobiDisplay
#
# In GGobi, the projection mode determines the type
# of plot that is displayed.  For example, a 1D ASH plot,
# or a 2D tour.  
# 
# You can see the list of available projection modes
# using the \code{\link{pmodes}} function.  This accepts
# either a GGobiDisplay, or the name of the display type.
# 
# @arguments GGobiDisplay object
# @keywords dynamic
#X g <- ggobi(mtcars)
#X d <- displays(g)[[1]]
#X pmode(d)
#X pmodes(d)
#X pmode(d) <- "1D Plot"
pmode <- function(x) .GGobiCall("getPModeName", x)

# Projection mode
# Get the projection mode from the GGobiDisplay
#
# In GGobi, the projection mode determines the type
# of plot that is displayed.  For example, a 1D ASH plot,
# or a 2D tour.  
# 
# You can see the list of available projection modes
# using the \code{\link{pmodes}} function.  This accepts
# either a GGobiDisplay, or the name of the display type.
# 
# @arguments GGobiDisplay object
# @keywords dynamic
#X g <- ggobi(mtcars)
#X d <- displays(g)[[1]]
#X pmode(d)
#X pmodes(d)
#X pmode(d) <- "1D Plot"imode <- function(x) .GGobiCall("getIModeName", x)


# List available projection modes
# This generic method retrieves allowed projection modes.
#
# 
pmodes <- function(x=NULL) UseMethod("pmodes", x)

# List available projection modes
# This function will display the projection modes availabe
pmodes.default <- function(x) {
	types <- ggobi_display_types()
	modes <- lapply(types, pmodes)
	unlist(mapply(
		function(modes, type) {
			all <- append(modes, type)
			lookup <- rep(type, length=length(all))
			names(lookup) <- all
			lookup
		}, 
		modes, names(modes), SIMPLIFY=FALSE
	))
}
pmodes.GGobiDisplay <- function(x) pmodes(class(x)[1])
pmodes.character <- function(x) .GGobiCall("getPModeNames", x)

imodes <- function(x) UseMethod("imodes", x)
imodes.GGobiDisplay <- function(x) imodes(class(x)[1])
imodes.character <- function(x) .GGobiCall("getIModeNames", x)
