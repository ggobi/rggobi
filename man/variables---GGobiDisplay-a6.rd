\name{"variables<-.GGobiDisplay"}
\alias{variables<-.GGobiDisplay}
\title{Set display variables}
\author{Hadley Wickham <h.wickham@gmail.com>}

\description{
Set display variables with a list of x, y, and z component variable indices.
}
\usage{"variables<-.GGobiDisplay"(x, value)}
\arguments{
\item{x}{GGobiDisplay object}
\item{value}{list with x, y and z components}
}

\details{There are three types of variables in GGobi displays:
x, y, z, which correspond to the labels on the toggle buttons
in GGobi.  Most plots have a constrained set of possible options.
For example, in tours you can only set x variables, and you must
have at least three.  Or in the rotation plot, you need exactly
one x, y, and z variable.

Currently, there is no checking done to ensure that you are
sending a sensible set of variables for the given display type.
Generally, any invalid choices will be silently ignored.}

\examples{g <- ggobi(mtcars)
d <- display(g[1], "Parallel Coordinates Display")
variables(d)
variables(d) <- list(X=1:8)
variables(d)}
\keyword{dynamic}
