

ggobi_display_set_plot_range <- function(gd, plot=1, x, y) {
  x <- rep(x, length=2)
  y <- rep(y, length=2)

  .GGobiCall("setPlotRange", as.numeric(x), as.numeric(y), as.integer(plot),  gd)
}

ggobi_display_get_plot_range <- function(gd, plot=1) {
  .GGobiCall("getPlotRange", gd, as.integer(plot))
}
