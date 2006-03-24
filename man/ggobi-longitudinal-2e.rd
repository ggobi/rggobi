\name{ggobi_longitudinal}
\alias{ggobi_longitudinal}
\title{Create longitudinal dataset.}
\author{Hadley Wickham <h.wickham@gmail.com>}

\description{
Instantiate new ggobi with a longitudinal data set.
}
\usage{ggobi_longitudinal(data, time = "Time", obsUnit = "ID")}
\arguments{
\item{data}{data frame}
\item{time}{time variable}
\item{obsUnit}{id variable}
}

\details{@arguments data frame
@arguments time variable
@arguments id variable
@keyword dynamic}

\examples{data(Oxboys, package="nlme")
ggobi_longitudinal(Oxboys, "Occasion", "Subject")}
\keyword{dynamic}
