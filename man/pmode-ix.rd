\name{pmode}
\alias{pmode}
\alias{pmode<-}
\title{Projection mode}
\author{Hadley Wickham <h.wickham@gmail.com>}

\description{
Functions to get and set projection mode
}
\usage{pmode(x)}
\arguments{
\item{x}{GGobiDisplay object}
}

\details{In GGobi, the projection mode determines the type
of plot that is displayed.  For example, a 1D ASH plot,
or a 2D tour.

You can see the list of available projection modes
using the \code{\link{pmodes}} function.  This accepts
either a GGobiDisplay, or the name of the display type.}

\examples{g <- ggobi(mtcars)
d <- displays(g)[[1]]
pmode(d)
pmodes(d)
pmode(d) <- "1D Plot"}
\keyword{dynamic}
