\name{ggobi_check_structs}
\alias{ggobi_check_structs}
\title{Check structs}
\author{Hadley Wickham <h.wickham@gmail.com>}

\description{
Validates GGobi and Rggobi views of internal data structures
}
\usage{ggobi_check_structs()}

\value{TRUE if the sizes in the two libraries are the same, otherwise a named logical vector indicating which structures are different}
\details{This function is called when the Rggobi library is loaded and it verifies
that the sizes of the different internal data structures for GGobi are the
same for both the GGobi shared library/DLL and the Rggobi package. This is
important as the two shared libraries are compiled separately and may have
different compilation flags, etc. that make them incompatible. This
function simply compares the sizes of the two views of the structures and
raises a warning if they do not agree.

Essentially, you should never notice this function. A warning implies that
you need to re-install Rggobi against the version of GGobi you are using.

@value TRUE if the sizes in the two libraries are the same, otherwise a named logical vector indicating which structures are different}


\keyword{programming}
\keyword{internal}
