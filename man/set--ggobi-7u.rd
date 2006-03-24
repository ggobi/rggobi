\name{"[<-.ggobi"}
\alias{[<-.ggobi}
\alias{$<-.ggobi}
\title{[<-.ggobi}
\author{Hadley Wickham <h.wickham@gmail.com>}

\description{
Add data to ggobi instance.
}
\usage{"[<-.ggobi"(x, i, value)}
\arguments{
\item{x}{ggobi instance}
\item{i}{name of data frame}
\item{value}{data.frame, or string to path of file to load}
}

\details{This function allows you to add (and eventually) replace
ggobi to a ggobi instance.}

\examples{g <- ggobi()
g["a"] <- mtcars
g$b <- mtcars}
\keyword{manip}
