\name{ggobi_display_set_tour_projection}
\alias{ggobi_display_set_tour_projection}
\title{Set tour projection}
\author{Hadley Wickham <h.wickham@gmail.com>}

\description{
Set the tour projection from a GGobi tour.
}
\usage{ggobi_display_set_tour_projection(gd, value)}
\arguments{
\item{gd}{GGobiDisplay object running tour}
\item{value}{tour projection}
}

\details{If you know the projection you would like to see
in the tour, you can use this function to set it.  The
example illustrates setting the projection to show
the first two principle components.}

\examples{
if (interactive()) {
g <- ggobi(mtcars)
d <- displays(g)[[1]]
}
\dontrun{
pmode(d) <- "2D Tour"
variables(d) <- list(X=names(mtcars))
ggobi_display_get_tour_projection(d)
pc <- princomp(as.matrix(mtcars))$loadings[,1:2]
ggobi_display_set_tour_projection(d, pc)
pc <- princomp(as.matrix(mtcars), cor=T)$loadings
ggobi_display_set_tour_projection(d, pc)[,1:2]
}}
\keyword{dynamic}
