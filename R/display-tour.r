# Get tour projection
# Get the tour projection from a GGobi tour.
#
# @argument GGobiDisplay object running tour
#X g <- ggobi(mtcars)
#X d <- displays(g)[[1]]
#X pmode(d) <- "2D Tour"
#X ggobi_display_get_tour_projection(d)
#X variables(d) <- list(X=names(mtcars))
#X ggobi_display_get_tour_projection(d)
ggobi_display_get_tour_projection <- function(gd) {
  mat <- .GGobiCall("getTourProjection", gd, pmode(gd))
  
  vals <- mat[,1:2]
  attr(vals, "scale") <- mat[,3]
  vals
}


# Set tour projection
# Set the tour projection from a GGobi tour.
#
# @argument GGobiDisplay object running tour
ggobi_display_set_tour_projection <- function(gd, value) {
  invisible(.GGobiCall("setTourProjection", gd, pmode(gd), value))
}