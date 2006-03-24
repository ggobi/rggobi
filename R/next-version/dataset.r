# Stuff yet to be cleaned up
# ============================================================================ 


VariableTypeEnum <- c(real = 0, categorical = 1, integer = 2, counter = 3, uniform = 4)


#
#  variable name = value, ...
# or
#   .which = c(variable name = value, ....)
#
setVariableType.ggobi <- function(..., .which,  .data = 1, .gobi = getDefaultGGobi())
{
  if(missing(.which)) {
     tmp = list(...)
     .which = as.integer(unlist(list(...)))
     names(.which) = names(tmp)
  } else {
      mode(.which) = "integer"
  }

  if(mode(.data) == "numeric" || mode(.data) == "character")
    .data <- getDatasetReference.ggobi(.data, .gobi)
  
  i = match(names(.which), names(.data))
  if(any(is.na(i))) {
     stop("variable names ", paste(names(.which)[is.na(i)], collapse = ", "), " not in the specified data set")
  }

  .GGobiCall("setVariableTypes", as.integer(i-1), .which, .data, .gobi = .gobi)
}

"[[<-.ggobiDataset" <- function(x, i, j, value) {
  if(is.character(j))
    vars <- getVariableIndex.ggobi(j, .data=x)
  else
    vars <- j

  val <- rep(value, length = length(i))
  for(v in vars) {
    setVariableValues.ggobi(val, v, i, .data = x)
  } 

  x
}




removeVariable.ggobi <- function(..., .data = 1, .gobi = getDefaultGGobi()) {
 which <- getVariableIndex.ggobi(..., .gobi=.gobi)

 .GGobiC("removeVariables", which, .data, .gobi = .gobi)[[1]]
}

getVariable.ggobi <- function(which, .data = 1, .gobi = getDefaultGGobi(), asDataFrame = FALSE) {
  idx <- getVariableIndex.ggobi(which, .data = .data, .gobi = .gobi)
  if(any(is.na(idx))) {
    stop(paste("Unmatched variable name", which[is.na(idx)],"in ggobi"))
  }

  varNames <- getVariableNames.ggobi(.data = .data, .gobi = .gobi)[idx]
  if(asDataFrame)
    rowNames <- getRowNames.ggobi(.data = .data, .gobi = .gobi)

  if(mode(.data) == "numeric")
    .data <- as.integer(.data - 1)

  vals <- .GGobiCall("getVariables", as.integer(idx), .data, .gobi=.gobi)
  names(vals)  <- varNames

  if(asDataFrame)
    vals <- data.frame(vals, row.names = rowNames, check.names = FALSE)

  vals
}
