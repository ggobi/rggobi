\name{"[.GGobi"}
\alias{[.GGobi}
\alias{[[.GGobi}
\alias{$.GGobi}
\title{Get ggobi data.}
\author{Hadley Wickham <h.wickham@gmail.com>}

\description{
Conveniently retrieve ggobi dataset.
}
\usage{"[.GGobi"(x, i, ..., drop=TRUE)}
\arguments{
\item{x}{GGobi object}
\item{i}{name of dataset to retrive}
\item{...}{}
\item{drop}{}
}

\details{It is convenient to be able to refer to and operate on a ggobi
dataset as if it were a regular R dataset.  This function allows one to
get an \code{GGobiData} object that represents a particular
dataset.}

\examples{g <- ggobi(ChickWeight)
g["cars"] <- mtcars
g[1:2]
g["ChickWeight"]
g["cars"]
g$cars}
\keyword{manip}
