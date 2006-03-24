\name{"edges<-"}
\alias{edges<-}
\title{Set edges}
\author{Hadley Wickham <h.wickham@gmail.com>}

\description{
Set edges for a dataset.
}
\usage{"edges<-"(x, value)}
\arguments{
\item{x}{ggobiDataset}
\item{value}{matrix or data frame of edges.  First column should be from edge, second column to edge.}
}

\details{To remove edges, set edges to NULL.

@arguments ggobiDataset
@arguments matrix or data frame of edges.  First column should be from edge, second column to edge.
@keyword manip}

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
