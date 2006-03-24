\name{ggobi_version}
\alias{ggobi_version}
\title{Get version}
\author{Hadley Wickham <h.wickham@gmail.com>}

\description{
GGobi version information
}
\usage{ggobi_version()}
\arguments{

}
\value{
 \item{the release date of the ggobi library}
 \item{a vector of 3 integers containing the major, minor and patch-level numbers.}
 \item{a string version of the major, minor and patch-level numbers,}
}
\details{Return an object that describes the version of the ggobi
library being used. This allows code to execute
conditionally on certain version numbers, etc.

@value the release date of the ggobi library
@value a vector of 3 integers containing the major, minor and patch-level numbers.
@value a string version of the major, minor and patch-level numbers,
@keyword dynamic}

\examples{ggobi_version()}
\keyword{dynamic}
