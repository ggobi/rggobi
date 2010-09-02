\name{ggobi_get}
\alias{ggobi_get}
\title{Get GGobi}
\author{Hadley Wickham <h.wickham@gmail.com>}

\description{
Returns a ggobi reference
}
\usage{ggobi_get(id = ggobi_count(), drop=TRUE)}
\arguments{
\item{id}{numeric vector indicating which ggobi instances to retrieve.  Use default if none specified}
\item{drop}{drop if possible?}
}

\details{This allows one to get a list of all the ggobi instances currently
in existence in the R session.  Also, one can fetch particular instances.}

\examples{ggobi(mtcars)
ggobi(Nile)
ggobi_get(1)
ggobi_get(1:2)}
\keyword{dynamic}
