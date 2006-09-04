\name{dataset.GGobiDisplay}
\alias{dataset.GGobiDisplay}
\title{Get display dataset}
\author{Hadley Wickham <h.wickham@gmail.com>}

\description{
Returns a link to the GGobiData (dataset) object associated with this display.
}
\usage{dataset.GGobiDisplay(gd)}
\arguments{
\item{gd}{GGobiDisplay object}
}

\details{See \code{\link{[.GGobi}} for more information on}

\examples{g <- ggobi(mtcars)
d <- displays(g)[1]
dataset(d)}
\keyword{manip}