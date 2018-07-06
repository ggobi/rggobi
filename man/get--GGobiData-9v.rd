\name{"[.GGobiData"}
\alias{[.GGobiData}
\alias{[[.GGobiData}
\alias{$.GGobiData}
\title{Subsettting}
\author{Hadley Wickham <h.wickham@gmail.com>}

\description{
Subsetting for ggobi datasets
}
\usage{\S3method{[}{GGobiData}(x, i, j, drop=FALSE)}
\arguments{
\item{x}{ggobi dataset}
\item{i}{rows}
\item{j}{cols}
\item{drop}{drop dimensions?}
}
\value{desired subset from data.frame}
\details{This functions allow one to treat a ggobi dataset as if it were a local
data.frame.  One can extract and assign elements within the dataset.

This method works by retrieving the entire dataset into
R, and then subsetting with R.}

\examples{
if (interactive()) {

g <- ggobi(mtcars)
x <- g$mtcars
x[1:5, 1:5]
x[[1]]
x$cyl}}
\keyword{manip}
