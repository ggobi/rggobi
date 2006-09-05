\name{display.GGobiData}
\alias{display.GGobiData}
\title{Create a new display}
\author{Hadley Wickham <h.wickham@gmail.com>}

\description{
Create a new display for the GGobiData object.
}
\usage{display.GGobiData(x, pmode="Scatterplot Display", vars=list(X=names(x)))}
\arguments{
\item{x}{}
\item{pmode}{}
\item{vars}{}
}

\details{This function allows you to create a new display from
a GGobiData object.  You will need to specify the type of display you
want ("Scatterplot Display", "Scatterplot Matrix" and  "Parallel Coordinates
Display" are the most common), and which variables the plot
should be intialised with.  Specifying more than two variables only makes
sense for scatterplot matrices and pcps.

Many of the plots used in GGobi (eg. the tours and densities plots) are
special modes of the scatterplot display.  You will need to create a
new scatterplot display, change the projection mode to what you want,
and then set the variables.  Hopefully this will be improved in a future
version of rggobi.}
\seealso{\code{\link{ggobi_display_types}} for a list of display types}
\examples{g <- ggobi(mtcars)
display(g[1])
display(g[1], vars=list(X=c(4,5)))
display(g[1], vars=list(X=c"drat","hp")))
display(g[1], "Parallel Coordinates Display")
display(g[1], "2D Tour")
display(g[1], "2x1D Tour", list(X=c(1,2,3), Y=c(4,5,6)))
display(g[1], "Scatterplot Matrix")}
\keyword{dynam}
