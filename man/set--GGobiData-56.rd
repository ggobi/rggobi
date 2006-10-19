\name{"[<-.GGobiData"}
\alias{[<-.GGobiData}
\alias{$<-.GGobiData}
\alias{[[<-.GGobiData}
\title{Assignments for ggobi datasets}
\author{Hadley Wickham <h.wickham@gmail.com>}

\description{
This functions allow one to treat a ggobi dataset as if it were a local data.frame.  One can extract and assign elements within the dataset.
}
\usage{"[<-.GGobiData"(x, i, j, value)}
\arguments{
\item{x}{ggobi dataset}
\item{i}{row indices}
\item{j}{column indices}
\item{value}{new values}
}

\details{This method works by retrieving the entire dataset into
R, subsetting that copy, and then returning any changes.}

\examples{g <- ggobi(mtcars)
x <- g["mtcars"]
x[1:5, 1:5]
x[1:5, 1] <- 1:5
x[1:5, 1:5]}
\keyword{manip}
\keyword{internal}
