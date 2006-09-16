
ggobi_display_set_plot_scale <- function(gd, plot=1, scale=c(0.7,0.7)) {
  scale <- rep(scale, length=2)
  .GGobiCall("setPlotScale",  scale[1], scale[2], gd, as.integer(plot))
}
ggobi_display_get_plot_scale <- function(gd, plot=1) {
  .GGobiCall("getPlotScale",  gd, as.integer(plot))
	
}

ggobi_display_set_plot_range <- function(gd, plot=1, x, y) {
  x <- rep(x, length=2)
  y <- rep(y, length=2)

  .GGobiCall("setPlotRange", as.numeric(x), as.numeric(y), as.integer(plot),  gd)
}

ggobi_display_get_plot_range <- function(gd, plot=1) {
  .GGobiCall("getPlotRange", as.integer(plot), gd)
}
