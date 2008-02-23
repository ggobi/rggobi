\name{.ggobi.symbol}
\alias{.ggobi.symbol}
\title{GGobi symbol creation}
\author{Hadley Wickham <h.wickham@gmail.com>}

\description{
Maps the given name to the name of the corresponding C routine
}
\usage{.ggobi.symbol(name)}
\arguments{
\item{name}{}
}
\value{the name of the C routine corresponding to its argument}
\details{A simple way of generating the prefix for a symbol
used in this package/library so that we can hide
it from other packages and avoid conflicts.}

\examples{}
\keyword{dynamic}
\keyword{internal}
