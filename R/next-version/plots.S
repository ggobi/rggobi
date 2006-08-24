getActivePlot.ggobi <-
function(.gobi = ggobi_get())
{
 as.integer(.GGobiCall("getActivePlot", display) + 1)
}

setActivePlot.ggobi <-
function(display, plot=1)
{
 .GGobiCall("setActivePlot", display, as.integer(plot)-1))
}


setIMode.ggobi <- function(name, .gobi = ggobi_get()) {
 .GGobiCall("setIMode",  as.character(name), .gobi=.gobi)
}
setPMode.ggobi <- function(name, .gobi = ggobi_get()) {
 .GGobiCall("setPMode",  as.character(name), .gobi=.gobi)
}

getPMode.ggobi <- function(.gobi = ggobi_get())  .GGobiCall("getPModeName", .gobi=.gobi)
getIMode.ggobi <- function(.gobi = ggobi_get())  .GGobiCall("getIModeName", .gobi=.gobi)

getPModeNames.ggobi <- function() .GGobiCall("getPModeNames")
getIModeNames.ggobi <- function() .GGobiCall("getIModeNames")
