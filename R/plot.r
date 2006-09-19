
# Set plot range
# Set the range of a plot.
#
# Only applies to scatterplots.
#
# @keyword internal
# @arguments GGobiDisplay object
# @arguments plot number
# @arguments x range
# @arguments y range
ggobi_display_set_plot_range <- function(gd, plot=1, x, y) {
  x <- rep(x, length=2)
  y <- rep(y, length=2)

  .GGobiCall("setPlotRange", as.numeric(x), as.numeric(y), as.integer(plot),  gd)
}

# Get plot range
# Get plot range as list of X and Y ranges
# 
# @keyword internal
# @arguments GGobiDisplay object
# @arguments plot number
ggobi_display_get_plot_range <- function(gd, plot=1) {
  .GGobiCall("getPlotRange", gd, as.integer(plot))
}
