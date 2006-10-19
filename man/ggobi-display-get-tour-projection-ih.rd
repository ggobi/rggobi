\name{ggobi_display_get_tour_projection}
\alias{ggobi_display_get_tour_projection}
\title{Get tour projection}
\author{Hadley Wickham <h.wickham@gmail.com>}

\description{
Get the tour projection from a GGobi tour.
}
\usage{ggobi_display_get_tour_projection(gd)}
\arguments{
\item{gd}{}
}

\details{This function retrieves the current projection matrix
from a paused tour.  (The tour must be paused so that R
can run commands).

This can be used to record interesting projections of your
data for later analysis.}

\examples{g <- ggobi(mtcars)
d <- displays(g)[[1]]
\dontrun{
pmode(d) <- "2D Tour"
ggobi_display_get_tour_projection(d)
variables(d) <- list(X=names(mtcars))
ggobi_display_get_tour_projection(d)
MASS::eqscplot(as.matrix(mtcars) \%*\% ggobi_display_get_tour_projection(d))
}}
\keyword{dynamic}
