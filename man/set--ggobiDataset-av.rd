\name{"[<-.ggobiDataset"}
\alias{[<-.ggobiDataset}
\title{Assignments for ggobi datasets}
\author{Hadley Wickham <h.wickham@gmail.com>}

\description{

}
\usage{"[<-.ggobiDataset"(x, i, j, value)}
\arguments{
\item{x}{row indices}
\item{i}{column indices}
\item{j}{new values}
\item{value}{}
}

\details{This functions allow one to treat a ggobi dataset as if it were a local
data.frame.  One can extract and assign elements within the dataset.

This method works by retrieving the entire dataset into
R, subsetting that copy, and then returning any changes.

@argument ggobi dataset
@arguments row indices
@arguments column indices
@arguments new values
@keyword manip
@keyword internal}

\examples{g <- ggobi(mtcars)
x <- g["mtcars"]
x[1:5, 1:5]
x[1:5, 1] <- 1:5
x[1:5, 1:5]}
\keyword{manip}
\keyword{internal}
