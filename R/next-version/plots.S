getActivePlot.ggobi <-
function(.gobi = getDefaultGGobi())
{
 as.integer(.GGobiCall("getActivePlot", .gobi = .gobi)+1)
}

setActivePlot.ggobi <-
function(display, plot=1, .gobi = getDefaultGGobi())
{
 .GGobiCall("setActivePlot", as.integer(c(display, plot)-1), .gobi = .gobi)
}
