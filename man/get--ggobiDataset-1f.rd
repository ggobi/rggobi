\name{"[.ggobiDataset"}
\alias{[.ggobiDataset}
\alias{[[.ggobiDataset}
\alias{$.ggobiDataset}
\title{Subsettting}
\author{Hadley Wickham <h.wickham@gmail.com>}

\description{
Subsetting for ggobi datasets
}
\usage{"[.ggobiDataset"(x, i, j, drop=FALSE)}
\arguments{
\item{x}{arguments for generic data.frame subset function}
\item{i}{drop dimensions?}
\item{j}{}
\item{drop}{}
}
\value{desired subset from data.frame}
\details{This functions allow one to treat a ggobi dataset as if it were a local
data.frame.  One can extract and assign elements within the dataset.

This method works by retrieving the entire dataset into
R, and then subsetting with R.}

\examples{g <- ggobi(mtcars)
x <- g$mtcars
x[1:5, 1:5]
x[[1]]
x$cyl}
\keyword{manip}
