\name{variables.GGobiDisplay}
\alias{variables.GGobiDisplay}
\title{Get display variables}
\author{Hadley Wickham <h.wickham@gmail.com>}

\description{
List the variables used in a given display
}
\usage{variables.GGobiDisplay(x)}
\arguments{
\item{x}{GGobiDisplay object}
}

\details{There are three types of variables in GGobi displays:
x, y, z, which correspond to the labels on the toggle buttons
in GGobi.  Most plots have a constrained set of possible options.
For example, in tours you can only set x variables, and you must
have at least three.  Or in the rotation plot, you need exactly
one x, y, and z variable.}
\seealso{\code{\link{variable<-.GGobiDataset}} for examples}
\examples{}
\keyword{dynamic}
