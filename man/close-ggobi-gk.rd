\name{close.ggobi}
\alias{close.ggobi}
\title{Close GGobi instance}
\author{Hadley Wickham <h.wickham@gmail.com>}

\description{
Terminates and discards a ggobi instance
}
\usage{close.ggobi(con, ...)}
\arguments{
\item{con}{ggobi object to close}
\item{...}{ignored and for compatability generic function.}
}

\details{This allows the caller to close a ggobi instance and discard the
resources it uses. The function closes the display windows and
variable panel window associated with this ggobi instance.
It also resets the default ggobi instance to be the last
one created.

@arguments ggobi object to close
@arguments ignored and for compatability generic function.
@keyword dynamic}

\examples{g1 <- ggobi(mtcars)
g2 <- ggobi(mtcars)
close(g2)
close(ggobi_get())}
\keyword{dynamic}
