\name{"[<-.GGobi"}
\alias{[<-.GGobi}
\alias{$<-.GGobi}
\alias{[[<-.GGobi}
\title{[<-.GGobi}
\author{Hadley Wickham <h.wickham@gmail.com>}

\description{
Add data to ggobi instance.
}
\usage{\S3method{[}{GGobi}(x, i) <- value}
\arguments{
\item{x}{ggobi instance}
\item{i}{name of data frame}
\item{value}{data.frame, or string to path of file to load}
}

\details{This function allows you to add (and eventually) replace
GGobiData objects in a GGobi instance.}

\examples{g <- ggobi()
g["a"] <- mtcars
g$b <- mtcars}
\keyword{manip}
