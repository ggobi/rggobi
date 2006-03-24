setOldClass("ggobiDataset")

# Set data
# ===================================================================

# [<-.ggobi
# Add data to ggobi instance.
# 
# This function allows you to add (and eventually) replace
# ggobi to a ggobi instance.  
# 
# @arguments ggobi instance
# @arguments name of data frame
# @arguments data.frame, or string to path of file to load
# @alias $<-.ggobi
# @returns ggobiDataset
# @keyword manip 
#X g <- ggobi()
#X g["a"] <- mtcars
#X g$b <- mtcars
"[<-.ggobi" <- function(x, i, value) {
	if (inherits(value, "ggobiDataset")) return(x)
	if (length(i) != 1) stop("You may only add or replace one dataset at a time")
	replace <- (i %in% names(x))
	if (replace) {
	  warning("Replacement does not work")
	  return(x)
	}
	
	if(is.character(value)) {
		ggobi_set_data_file(value, .gobi = x)
	} else {
		ggobi_set_data_frame(as.data.frame(value), name = clean.ggobi(i), .gobi = x)
	}
	x
}
"$<-.ggobi" <- function(x, i, value) {
  x[i] <- value
  x
}
# Set data file.
# Open data file and add to ggobi datasets.
# 
# @arguments path to file
# @arguments mode of file
# @arguments add?
# @arguments ggobi instance
# @returns ggobiDataset
# @keyword manip 
# @keyword internal
ggobi_set_data_file <- function(file, mode = "unknown", add = TRUE, .gobi = ggobi_get()) {
	num <- .GGobiCall("setFile", as.character(file), as.character(mode), as.logical(add), .gobi = .gobi)
	if(num > -1) {
		dataset(num, .gobi)
	} else {
		warning(paste("Failed to open", file))
	}
}


# Set data frame.
# Add data.frame to ggobi instance.
# 
# @arguments data frame to add
# @arguments data set name (appears on tabs in ggobi)
# @arguments description of data frame
# @arguments rownames
# @arguments ggobi instance
# @returns ggobiDataset
# @keyword manip 
# @keyword internal
ggobi_set_data_frame <- function(data, name = deparse(sys.call()[[2]]), description = paste("R data frame", name), id = rownames(data), .gobi = ggobi_get()) {
	n <- dimnames(data)
	if(length(id) != nrow(data)) {
		id <- paste(seq(1, nrow(data)))
	}

	.data <- .GGobiCall("addData",
		data,
		n[[1]], n[[2]], dim(data), as.character(description),
		as.character(name),
		as.character(id), .gobi = .gobi)

	.data
}

# Add variable
# Add variable to a ggobiDataset
# 
# @alias addVariable
# @arguments ggobiDataset
# @arguments values to add
# @arguments name of column to add
# @keyword manip 
addVariable.ggobiDataset <- function(x, vals, name, ...) {
	if (!(is.factor(vals) || is.numeric(vals))) stop("Variable must be a factor or numeric")
	if (length(vals) != nrow(x)) stop("Variable must be same length as existing data set")

	levels <- NULL
	values <- NULL

	if(is.factor(vals)) {
		levels <- summary(vals[!is.na(vals)])
		values <- sort(unique(as.integer(vals)))
	}

	.GGobiCall("addVariable", as.numeric(vals), as.character(name), levels, values, x)
}
addVariable <- function(x, ...) UseMethod("addVariable", x)

# Get data
# ===================================================================

# Get ggobi data.
# Conveniently retrieve ggobi dataset.
#
# It is convenient to be able to refer to and operate on a ggobi 
# dataset as if it were a regular R dataset.  This function allows one to
# get an \code{ggobiDataset} object that represents a particular 
# dataset. 
# 
# @arguments GGobi object
# @arguments name of dataset to retrive
# @keyword manip 
# @alias [.ggobi
#X g <- ggobi(ChickWeight)
#X g["cars"] <- mtcars
#X g[1:2]
#X g["ChickWeight"]
#X g["cars"]
#X g$cars
"$.ggobi" <- function(x, i) {
  x[i]
}
"[.ggobi" <- function(x, i, ..., drop=TRUE) {
	d <- dataset(clean.ggobi(i), .gobi = x)
	if (drop && length(d) == 1 ) {
		d[[1]]
	} else {
		d
	}
}

# Get ggobi dataset.
# Get an object representing an internal ggobi dataset
#
# It is convenient to be able to refer to and operate on a ggobi 
# dataset as if it were a regular R dataset.  This function allows one to
# get an \code{ggobiDataset} object that represents a particular 
# dataset. 
# 
# @arguments which dataset to retrieve, an integer for positional matching or a character to match by name
# @arguments GGobi instance
# @value A list of \code{ggobiDataset} objects
# @seealso \code{link{.ggobi}} 
# @keyword manip 
# @keyword internal
dataset <- function(which, .gobi = ggobi_get()) {
	if (is.character(which)) {
		id <- match(which, names(.gobi))
		if (any(is.na(id))) {
			stop(paste("Unrecognized dataset name", which[is.na(id)]))
		}
		which <- id
	}

	refs <- .GGobiCall("getDataset", as.integer(which-1), .gobi=.gobi)

	refs <- lapply(refs, function(x) {
		if(is.null(x)) return() 
		#class(x) <- c(class(x), "data.frame"); 
		attr(x,"ggobi") <- .gobi
		x
	}) 

	refs
}