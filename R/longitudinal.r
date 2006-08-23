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
ggobi_longitudinal <- function(data, time, obsUnit) {
	tmp <- data[order(data[[obsUnit]], data[[time]]), ]
	g <- ggobi(tmp)
	
	edges <- cbind(rownames(tmp[-nrow(tmp), ]), rownames(tmp[-1, ]))
	matching <- tmp[-1, obsUnit] == tmp[-nrow(tmp), obsUnit]
	edges[!matching, ] <- NA
	
	edges(g[1]) <- edges 

	invisible(g)
}
