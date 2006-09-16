ggobi_display_get_tour_projection <- function(gd) {
  .GGobiCall("getTourProjections", gd, pmode(gd))
}
ggobi_display_set_tour_projection <- function(gd, value) {
  .GGobiCall("setTourProjections", gd, pmode(gd), value)
}