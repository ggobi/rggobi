\name{dataset.numeric}
\alias{dataset.numeric}
\alias{dataset.character}
\title{Get ggobi dataset.}
\author{Hadley Wickham <h.wickham@gmail.com>}

\description{
Get an object representing an internal ggobi dataset
}
\usage{dataset.numeric(x, .gobi = ggobi_get())}
\arguments{
\item{x}{which dataset to retrieve, an integer for positional matching or a character to match by name}
\item{.gobi}{GGobi instance}
}
\value{A list of \code{GGobiData} objects}
\details{It is convenient to be able to refer to and operate on a ggobi
dataset as if it were a regular R dataset.  This function allows one to
get an \code{GGobiData} object that represents a particular
dataset.}
\seealso{\code{\link{$.GGobiData}} for user level selection of datasets}
\examples{}
\keyword{manip}
\keyword{internal}
