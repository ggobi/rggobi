\name{.GGobiCall}
\alias{.GGobiCall}
\alias{.GGobiC}
\title{Calling native routines}
\author{Hadley Wickham <h.wickham@gmail.com>}

\description{
Wrappers for calling C routines in the R-ggobi library.
}
\usage{.GGobiCall(.name, ..., .gobi = ggobi_get(), .test=TRUE)}
\arguments{
\item{.name}{the simple name of the C routine to be resolved}
\item{...}{the arguments that to be passed to the \code{\link{.C}} or \code{\link{.Call}}}
\item{.gobi}{the ggobi instance identifier that is to be passed to the C routine as its last argument}
\item{.test}{}
}
\value{the same result as the corresponding \code{.C} and \code{.Call}}
\details{\code{.GGobiC} and \code{.GGobiCall} convert the name and then call
their C invocation counterparts.

These functions map the simple name of a C routine into the
package-specific version of that name.  These allow use to hide the
use a name \textit{mangling} scheme of our choosing for the C level
routines in the shared library/DLL that provides the glue between R
and ggobi.  This is useful for avoiding name conflicts with other C
code in R or other packages.  These are only of relevance to the
developers of this package and those working with its C code.

The mapping of the name to its corresponding C routine name
is done in conjunction with the pre-processor macro
\code{RS_GGOBI}. These  must be synchronized.}
\seealso{\code{\link{.C}}, \code{\link{.Call}}}
\examples{}
\keyword{dynamic}
\keyword{internal}
