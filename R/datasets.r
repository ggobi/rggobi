# Basic data set manipulation
# 
# Subsetting, dimensions, names etc.
# Attempts to mimick R data frames as closely as possible
# ============================================================================ 

# Print ggobiDataset
# Print ggobiDataset
# 
# By default printing a ggobiDataset acts like
# printing an R data.frame - ie. show all the data
# 
# @arguments GGobi dataset to retrieve
# @keyword attribute
# @keyword internal 
print.ggobiDataset <- function(x, ...) {
  print(as.data.frame(x), ...)
}

# ggobiDataset dimensions
# Retrieve the dimension of an ggobi dataset
# 
# @arguments dataset
# @keyword attribute
# @keyword internal 
dim.ggobiDataset <- function(x) {
	.GGobiCall("datasetDim", x)
}

# ggobiDataset rows
# Retrieve the number of row in an ggobi dataset
# 
# @arguments dataset
# @keyword attribute
# @keyword internal 
nrow.ggobiDataset <- function(d) dim(d)[2]

# ggobiDataset columns
# Retrieve the number of columns in an ggobi dataset
# 
# @arguments dataset
# @keyword attribute
# @keyword internal 
ncol.ggobiDataset <- function(d) dim(d)[1]

# ggobiDataset column names
# Get column names for a ggobiDataset
# 
# @arguments dataset
# @keyword attribute
# @keyword internal 
names.ggobiDataset <- function(x, ...) {
  .GGobiCall("getVariableNames", FALSE, x)
}

# Set column names
# Set column names for a ggobiDataset
# 
# @arguments ggobiDataset
# @arguments new names
# @keyword attribute
# @keyword internal 
"names<-.ggobiDataset" <- function(x, value) {
	.GGobiCall("setVariableNames", as.integer(1:ncol(x) -1 ), as.character(value), x)
	x
}


# Variable index
# Return indices corresponding to variable names
# 
# @arguments ggobiDataset
# @arguments variable names
# @returns numeric vector of (0-based) positions
# @keyword attribute
# @keyword internal 
variable_index <- function(x, names) {
	if(length(names) == 0) return(integer(0))
	if(is.numeric(names)) return(as.integer(names - 1))
	if(is.character(names)) return(as.integer(match(names, names(x)) - 1))
	
	unlist(lapply(names, function(name) variable_index.ggobiDataset(x, name)))
}

# Get row names
# Get row names for a ggobiDataget
# 
# @arguments ggobiDataget
# @arguments new names
# @keyword attribute
# @keyword internal 
rownames.ggobiDataset <- function(x) {
	.GGobiCall("getRowNames", x)
}

# Set row names
# Set row names for a ggobiDataset
# 
# @arguments ggobiDataset
# @arguments new names
# @keyword attribute
# @keyword internal 
"rownames<-.ggobiDataset" <- function(x, value) {
	.GGobiCall("setRowNames", as.character(value), as.integer(1:length(value) - 1), x)
	x
}


# Get dimension names
# Get row and column names for a ggobiDataget
# 
# @arguments ggobiDataget
# @keyword attribute
# @keyword internal 
dimnames.ggobiDataset <- function(x) {
  list(rownames.ggobiDataset(x), names(x))
}

# Summarise ggobiDataset.
# Summarise a ggobiDataset with dimensions, mode and variable names.
# 
# @arguments ggobiDataset
# @keyword attribute
summary.ggobiDataset <- function(object, ...) {
	list(dim = dim(object), mode = mode(object), variables = names(object))
}

# Subsettting
# Subsetting for ggobi datasets
#
# This functions allow one to treat a ggobi dataset as if it were a local
# data.frame.  One can extract and assign elements within the dataset.
# 
# This method works by retrieving the entire dataset into
# R, and then subsetting with R.
# 
# @argument ggobi dataset
# @arguments arguments for generic data.frame subset function
# @arguments drop dimensions?
# @value desired subset from data.frame
# @alias [[.ggobiDataset
# @alias $.ggobiDataset
# @keyword manip 
#X g <- ggobi(mtcars)
#X x <- g$mtcars
#X x[1:5, 1:5]
#X x[[1]]
#X x$cyl
"[.ggobiDataset" <- function(x, i, j, drop=FALSE) {
	as.data.frame(x)[i, j, drop=drop]
}

"$.ggobiDataset" <- "[[.ggobiDataset" <- function(x, i) {
	as.data.frame(x)[[i]]
}

# Conversion methods
# Convert a ggobiDataset to a regular R data.frame or matrix
# 
# @arguments ggobiDataset
# @keyword manip 
# @keyword internal
# @alias as.matrix.ggobiDataset
"as.data.frame.ggobiDataset" <- function(x, ...) {
	as.data.frame(.GGobiCall("getData", x))
}

"as.matrix.ggobiDataset" <- function(x, ...) {
  as.matrix(as.data.frame(x))
}

# Assignments for ggobi datasets
#
# This functions allow one to treat a ggobi dataset as if it were a local
# data.frame.  One can extract and assign elements within the dataset.
# 
# This method works by retrieving the entire dataset into
# R, subsetting that copy, and then returning any changes.
# 
# @argument ggobi dataset
# @arguments row indices
# @arguments column indices
# @arguments new values
# @keyword manip 
# @keyword internal 
#
#X g <- ggobi(mtcars)
#X x <- g["mtcars"]
#X x[1:5, 1:5]
#X x[1:5, 1] <- 1:5
#X x[1:5, 1:5]
"[<-.ggobiDataset" <- function(x, i, j, value) {
	data <- as.data.frame(x)

	data[i, j] <- value
	for(i in unique(j)) {
		ggobi_data_set_variables(x, data[,i], i)
	}
	x
}

# Set variable values
# Set the variable values for a column in a ggobiDataset
# 
# @arguments ggobiDataset
# @arguments values of new variable
# @arguments variable name
# @arguments update?
# @keyword manip 
# @keyword internal 
ggobi_data_set_variables <- function(x, values, var, update = TRUE) {
	varId <- variable_index(x, var)
	if(any(is.na(varId))) stop("Invalid variable")

	.GGobiCall("setVariableValues", as.numeric(values), as.integer(1:length(values) - 1), varId, as.logical(update), x)
	x
}

ids <- function(x) UseMethod("ids", x)
"ids<-" <- function(x, value) UseMethod("ids<-", x)

# Row ids
# Retrive row ids from a ggobiDataset
# 
# @alias ids
# @arguments ggobiDataset
# @keyword manip 
# @seealso \code{\link{ids<-}}
ids.ggobiDataset <- function(x) {
  .GGobiCall("getCaseIds", x)
}  

# Set row ids
# Set row ids from a ggobiDataset
# 
# @alias ids<-
# @arguments ggobiDataset
# @arguments new values
# @keyword manip 
# @seealso \code{\link{ids}}
"ids<-.ggobiDataset" <- function(x, value) {
	.GGobiCall("setIDs", as.character(value), .data=x)
	x
}
