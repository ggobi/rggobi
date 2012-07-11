\name{"edges<-.GGobiData"}
\alias{edges<-.GGobiData}
\title{Set edges}
\author{Hadley Wickham <h.wickham@gmail.com>}

\description{
Set edges for a dataset.
}
\usage{\S3method{edges}{GGobiData}(x) <- value}
\arguments{
\item{x}{GGobiData}
\item{value}{matrix, data frame, or graph containing of edges.  First column should be from edge, second column to edge.}
}

\details{In GGobi, and edge dataset is a special type of dataset that has
two additional (hidden) columns which specification source and
destination row names.  These rownames are compared to the row
names of the dataset in the current plot, and if any match, it
is possible to specify this dataset as an edge set to the plotted
datset.  When this is done, edges will be drawn between points
specified by the edge dataset.

To remove edges, set edges to NULL.}
\seealso{\code{\link{ggobi_longitudinal}} for creating edges which simulate time series plots}
\examples{cc<-cor(t(swiss),use="p", method="s") 
ccd<-sqrt(2*(1-cc)) 
a <- which(lower.tri(ccd), arr.ind=TRUE)
src <- row.names(swiss)[a[,2]]
dest <- row.names(swiss)[a[,1]] 
weight <- as.vector(as.dist(ccd))
gg <- ggobi(swiss)
gg$cor <- data.frame(weight)
edges(gg$cor) <- cbind(src, dest)
edges(gg$cor)
edges(gg$cor) <- NULL}
\keyword{manip}
