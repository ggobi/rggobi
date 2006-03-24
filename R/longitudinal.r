# Create longitudinal dataset.
# Instantiate new ggobi with a longitudinal data set.
# 
# @arguments data frame
# @arguments time variable
# @arguments id variable
# @keyword dynamic 
# 
#X data(Oxboys, package="nlme")
#X ggobi_longitudinal(Oxboys, "Occasion", "Subject")
ggobi_longitudinal <- function(data, time = "Time", obsUnit = "ID") {
	tmp <- data[order(data[, obsUnit]), ]
	g <- ggobi(tmp)
	edges(g[1]) <-  create_time_edges(tmp, time, obsUnit)

	invisible(g)
}


# Create time edges
# Generate the connecting edges for longitudinal data
#
# This function takes a data frame containing records with repeated 
# measurements for a given observational unit and generates the 
# appropriate edge information so that consecutive time
# points can be connected in a GGobi display. The output of this 
# function can be given to setEdges to enable
# viewing these time series plots for individuals.  
#
# Currently this only works if the data are sorted by ID.
# This will be fixed in the future as it requires re-ordering
# the records from the original dataset.
#
# @arguments the dataset from which to compute the edge connections.
# @arguments name/index of time variable 
# @arguments name/index of ID variable 
# @value A matrix with 2 columns and as many rows as in original data.  Each row in this matrix gives a directed line segment connecting one point in time to the next point in time
# @keyword dynamic 
# @keyword internal 
create_time_edges <- function(data, time = "Time", obsUnit = "ID") {
  i <- by(data, data[, obsUnit], create_time_edges_for_unit, time)

  matrix(unlist(i), , 2, byrow = TRUE, )
}

# Create time edges for a unit
# Create time edges for a unit
# 
# @arguments data frame of observations from one unit
# @arguments time variable
# @keyword dynamic 
# @keyword internal 
create_time_edges_for_unit <- function(d, time) {
	n <- nrow(d)
	if(n == 1) return(matrix(NA, 1, 2))

	o <- order(d[, time])
	ids <- rownames(d)[o]

	t(cbind(c(ids[1:(n-1)], NA), c(ids[2:n], NA))  )
}
