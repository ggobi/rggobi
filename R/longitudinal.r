# Create longitudinal dataset.
# Instantiate new ggobi with a longitudinal data set.
# 
# @arguments data frame
# @arguments time variable
# @arguments id variable
# @keyword dynamic 
# 
#X data(Oxboys, package="nlme")
#X ggobi_longitudinal(Oxboys, Occasion, Subject)
#X example adding an id, if not present
ggobi_longitudinal <- function(data, time=1:nrow(data), obsUnit=1) {
	time <- eval(time, data)
	obsUnit <- eval(obsUnit, data)

	or <- order(obsUnit, time)
	tmp <- data[or, ]
	g <- ggobi(tmp)
	
	edges <- cbind(rownames(tmp[-nrow(tmp), ]), rownames(tmp[-1, ]))
	matching <- obsUnit[or][-1] == obsUnit[or][-nrow(tmp)]
	edges[!matching, ] <- NA
	
	edges(g[1]) <- edges 

	invisible(g)
}
