\name{"shadowed<-.GGobiData"}
\alias{shadowed<-.GGobiData}
\alias{shadowed<-}
\title{Set shadowed status}
\author{Hadley Wickham <h.wickham@gmail.com>}

\description{
Set the exclusion status of points.
}
\usage{"shadowed<-.GGobiData"(x, value)}
\arguments{
\item{x}{GGobiData}
\item{value}{logical vector}
}

\details{If a point is shadowed it is drawn in a dark gray colour,
behind all non-shadowed points.  It cannot be selected.}
\seealso{\code{\link{shadowed}}}
\examples{}
\keyword{dynamic}
