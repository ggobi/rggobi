\name{imode}
\alias{imode}
\alias{imode<-}
\title{Interaction mode}
\author{Hadley Wickham <h.wickham@gmail.com>}

\description{
Functions to get and set interaction mode
}
\usage{imode(x)}
\arguments{
\item{x}{GGobiDisplay object}
}

\details{In GGobi, the interaction mode determines the how
you interact with a plot: brushing, identify etc.
Each projection mode also has a default interaction
mode that allows you to select variables and control
other parameters of the display

You can see the list of available interaction modes
using the \code{\link{imodes}} function.  This accepts
either a GGobiDisplay, or the name of the display type.}

\examples{
if (interactive()) {
g <- ggobi(mtcars)
d <- displays(g)[[1]]
imode(d)
imodes(d)
imode(d) <- "Brush"}}
\keyword{dynamic}
