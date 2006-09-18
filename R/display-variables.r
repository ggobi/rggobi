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
	prev_vars <- variables(x)

	# Get the indices of the variables the user has specified.
	d <- dataset(x)

	# build a list of variable indices for each of x, y, and z
	vars <- variable_index(d, value)

	# find the vars we don't want anymore
	prev_vars <- lapply(1:length(vars), function(i) {
	  pv <- variable_index(d, prev_vars[[i]]) 
	  pv[!(pv %in% vars[[i]])]
	})

	.GGobiCall("setDisplayVariables", vars, prev_vars, x, .gobi=ggobi(x))
	x
}