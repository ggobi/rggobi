\name{valid_ggobi}
\alias{valid_ggobi}
\title{Validity checking}
\author{Hadley Wickham <h.wickham@gmail.com>}

\description{
Determines whether a reference to an internal ggobi object is valid
}
\usage{valid_ggobi(.gobi)}
\arguments{
\item{.gobi}{an object of class \code{ggobi} which refers to an internal ggobi instance.}
}
\value{\code{TRUE} if real object still exist, \code{FALSE} otherwise}
\details{One can create multiple, independent ggobi instances within a single
R session and one can also remove them either programmatically or
via the GUI.  To be able to refer to these objects which are
actually C-level internal objects, one has a reference or handle
from an S object. Since the C level object can be destroyed while the S
object still refers to them, this function allows one to check whether the
internal object to which R refers is still in existence.}

\examples{
if (interactive()) {
g <- ggobi(mtcars)
valid_ggobi(g)
close(g)
valid_ggobi(g) }}
\keyword{dynamic}
\keyword{internal}
