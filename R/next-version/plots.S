getActivePlot.ggobi <-
function(.gobi = getDefaultGGobi())
{
 as.integer(.GGobiCall("getActivePlot", display) + 1)
}

setActivePlot.ggobi <-
function(display, plot=1)
{
 .GGobiCall("setActivePlot", display, as.integer(plot)-1))
}
