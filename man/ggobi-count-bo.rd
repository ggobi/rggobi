\name{ggobi_count}
\alias{ggobi_count}
\title{Get number of GGobis}
\author{Hadley Wickham <h.wickham@gmail.com>}

\description{
Retrieves the number of ggobi instances within this session
}
\usage{ggobi_count()}
\arguments{

}

\details{One or more ggobi instances can be created within an R session so that one
can simultaneously look at different datasets or have different views
of the same dataset.  This function returns the number of ggobis in existence.

The different ggobi instances are maintained as C  level structures.
This function accesses a variable that stores how many are in existence
when the function is invoked.}

\examples{ggobi_count()}
\keyword{dynamic}
