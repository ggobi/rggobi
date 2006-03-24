# zooming operations on plots

# I think setting the plot range belongs here, but I'm not sure

setPlotRange.ggobi <-
function(x, y, plot=1, display = 1, .gobi = getDefaultGGobi())
{
 stop("This is not implemented yet")
 length(x) <- 2
 length(y) <- 2

 plot <- as.integer(plot)
 if(mode(display) == "numeric")
    display <- as.integer(display - 1)
 .GGobiCall("setPlotRange", as.numeric(x), as.numeric(y), as.integer(plot), 
            display, .gobi=.gobi)
}

getPlotRange.ggobi <-
function(plot=integer(1), display = 1, .gobi = getDefaultGGobi())
{
 stop("This is not implemented yet")
 if(mode(display) == "numeric")
    display <- as.integer(display - 1)
 .GGobiCall("getPlotRange", as.integer(plot), display, .gobi=.gobi)
}
