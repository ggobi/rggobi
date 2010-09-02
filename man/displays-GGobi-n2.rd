\name{displays.GGobi}
\alias{displays.GGobi}
\alias{displays.GGobi}
\title{Get GGobi displays}
\author{Hadley Wickham <h.wickham@gmail.com>}

\description{
Gets list of displays in the specified GGobi instance
}
\usage{displays.GGobi(x)}
\arguments{
\item{x}{GGobi object}
}

\details{A display basically corresponds to a window in GGobi.  A display
may contain mutliple plots within it.  For example, the scatterplot
matrix contains $p * p$ plots.

Use this function to obtain a reference to a display (they are
numbered in the order they are created) so you can change
display mode, set variables (\code{\link{variables<-.GGobiDisplay}}),
or save a static image to disk.}
\seealso{\code{\link{display}} to create displays}
\examples{g <- ggobi(mtcars)
displays(g)
display(g[1])
displays(g)}
\keyword{dynamic}
