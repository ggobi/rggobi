\name{"colorscheme<-"}
\alias{colorscheme<-}
\title{Set active colour scheme.}
\author{Hadley Wickham <h.wickham@gmail.com>}

\description{
Specify the active color scheme in a GGobi instance or the  session options.
}
\usage{"colorscheme<-"(x, value)}
\arguments{
\item{x}{GGobi object}
\item{value}{colour scheme to make active}
}
\value{The name of the previously active color scheme.}
\details{This makes a particular color scheme active within a GGobi instance.}

\examples{g <- ggobi(mtcars)
colorscheme(g) <- "Set1 8"
colorscheme(g) <- 1}
\keyword{color}
