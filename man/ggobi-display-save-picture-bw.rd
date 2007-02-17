\name{ggobi_display_save_picture}
\alias{ggobi_display_save_picture}
\title{Save picture of plot (and window) to disk}
\author{Hadley Wickham <h.wickham@gmail.com>}

\description{
This allows you to make a static copy of a GGobiDisplay.
}
\usage{ggobi_display_save_picture(display=displays(ggobi_get())[[1]], path="ggobi_display.png", filetype="png", plot.only = FALSE)}
\arguments{
\item{display}{GGobiDisplay to save}
\item{path}{path to save to}
\item{filetype}{type of file to save}
\item{plot.only}{if TRUE, save only plot, otherwise save surrounding GUI elements as well}
}

\details{If you want to make publicaiton quality graphics, you should
probably use the DescribeDisplay plugin and package.   This
will recreate a GGobiDisplay in R, and so can produce high-quality
vector (eg. pdf) output.  See \url{http://www.ggobi.org/describe-display}
for more information}

\examples{g <- ggobi(mtcars)
ggobi_display_save_picture(displays(g)[[1]], "test.png")}
\keyword{hplot}
