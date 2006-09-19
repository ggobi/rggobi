\name{ggobi_longitudinal}
\alias{ggobi_longitudinal}
\title{Create longitudinal dataset.}
\author{Hadley Wickham <h.wickham@gmail.com>}

\description{
Instantiate new ggobi with a longitudinal data set.
}
\usage{ggobi_longitudinal(data, time=1:rows, id=rep(1, rows))}
\arguments{
\item{data}{data frame}
\item{time}{time variable}
\item{id}{id variable}
}

\details{This function allows you to load longitudinal data in
to GGobi and display it as a line plot.  This is achieved
by creating edges between adjacent time points, for a given
id variable.

For best viewing, we recommend that you turn the show points off
in the options menu.  When brushing, you may also want to use
categorical brushing on the id variable, so that the entire
series is selected for an observation.}

\examples{data(Oxboys, package="nlme")
ggobi_longitudinal(Oxboys, Occasion, Subject)
ggobi_longitudinal(stormtracks, seasday, id)
ggobi_longitudinal(data.frame(x=1:100, y=sin(1:100)))}
\keyword{dynamic}
