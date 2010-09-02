\name{close.GGobiDisplay}
\alias{close.GGobiDisplay}
\title{Close display}
\author{Hadley Wickham <h.wickham@gmail.com>}

\description{
Closes the referenced display.  The R variable will be invalid after this call.
}
\usage{close.GGobiDisplay(con, ...)}
\arguments{
\item{con}{GGobiDisplay object to close}
\item{...}{}
}



\examples{g <- ggobi(mtcars)
displays(g)
close(displays(g)[[1]])
displays(g)}
\keyword{internal}
