\name{create_time_edges}
\alias{create_time_edges}
\title{Create time edges}
\author{Hadley Wickham <h.wickham@gmail.com>}

\description{
Generate the connecting edges for longitudinal data
}
\usage{create_time_edges(data, time = "Time", obsUnit = "ID")}
\arguments{
\item{data}{the dataset from which to compute the edge connections.}
\item{time}{name/index of time variable}
\item{obsUnit}{name/index of ID variable}
}
\value{A matrix with 2 columns and as many rows as in original data.  Each row in this matrix gives a directed line segment connecting one point in time to the next point in time}
\details{This function takes a data frame containing records with repeated
measurements for a given observational unit and generates the
appropriate edge information so that consecutive time
points can be connected in a GGobi display. The output of this
function can be given to setEdges to enable
viewing these time series plots for individuals.

Currently this only works if the data are sorted by ID.
This will be fixed in the future as it requires re-ordering
the records from the original dataset.}

\examples{}
\keyword{dynamic}
\keyword{internal}
