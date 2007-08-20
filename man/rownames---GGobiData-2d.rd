\name{"rownames<-.GGobiData"}
\alias{rownames<-.GGobiData}
\title{Set row names}
\author{Hadley Wickham <h.wickham@gmail.com>}

\description{
Set row names for a GGobiData
}
\usage{"rownames<-.GGobiData"(x, value)}
\arguments{
\item{x}{GGobiData}
\item{value}{new names}
}

\details{}

\examples{g <- ggobi(mtcars)
df <- g[1]
rownames(df)
rownames(df) <- tolower(rownames(df))
rownames(df)}
\keyword{attribute}
\keyword{internal}
