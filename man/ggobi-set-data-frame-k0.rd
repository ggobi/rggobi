\name{ggobi_set_data_frame}
\alias{ggobi_set_data_frame}
\title{Set data frame.}
\author{Hadley Wickham <h.wickham@gmail.com>}

\description{
Add data.frame to ggobi instance.
}
\usage{ggobi_set_data_frame(data, name = deparse(sys.call()[[2]]), description = paste("R data frame", name), id = rownames(data), .gobi = ggobi_get())}
\arguments{
\item{data}{data frame to add}
\item{name}{data set name (appears on tabs in ggobi)}
\item{description}{description of data frame}
\item{id}{rownames}
\item{.gobi}{ggobi instance}
}

\details{}

\examples{}
\keyword{manip}
\keyword{internal}
