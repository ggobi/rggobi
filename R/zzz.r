# Check structs
# Validates GGobi and Rggobi views of internal data structures
# 
# This function is called when the Rggobi library is loaded and it verifies
# that the sizes of the different internal data structures for GGobi are the
# same for both the GGobi shared library/DLL and the Rggobi package. This is
# important as the two shared libraries are compiled separately and may have
# different compilation flags, etc. that make them incompatible. This
# function simply compares the sizes of the two views of the structures and
# raises a warning if they do not agree.
# 
# Essentially, you should never notice this function. A warning implies that
# you need to re-install Rggobi against the version of GGobi you are using.
# 
# @value TRUE if the sizes in the two libraries are the same, otherwise a named logical vector indicating which structures are different
# 
# @keyword programming
# @keyword internal 
ggobi_check_structs <- function() {
	ours   <- .Call(.ggobi.symbol("getStructSizes"), TRUE,  PACKAGE = "rggobi")
	theirs <- .Call(.ggobi.symbol("getStructSizes"), FALSE, PACKAGE = "rggobi")

	which <- match(names(ours), names(theirs))
	if(any(is.na(which)))
		stop(paste("No information about some struct(s):", paste("`", names(ours)[is.na(which)],"'", collapse=", ", sep="")))

	ok <- ours == theirs[which]
	if(!all(ok)) {
		warning("Some structs have different size: ", 
      paste(paste(names(ours)[!ok], "(", ours[!ok], "!=", theirs[which][!ok], ")"), collapse=", "), 
      ". You may have an incompatible version of GGobi installed.", .call=FALSE)
		return(ok)
	}

	TRUE
}


.onLoad <- function(libname, pkgname) {
	library(methods)
	result <- try(library.dynam("rggobi", pkgname, libname))
  if (inherits(result, "try-error"))
    stop("Could not load the rggobi library - please ensure GGobi is on the library path")

	ggobi_check_structs()

	TRUE
}