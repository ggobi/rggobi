\name{dim.GGobiData}
\alias{dim.GGobiData}
\title{GGobiData dimensions}
\author{Hadley Wickham <h.wickham@gmail.com>}

\description{
Retrieve the dimension of a GGobiData
}
\usage{\S3method{dim}{GGobiData}(x)}
\arguments{
\item{x}{dataset}
}



\examples{
if (interactive()) {
g <- ggobi(mtcars)
dim(g[1])}}
\keyword{attribute}
\keyword{internal}
