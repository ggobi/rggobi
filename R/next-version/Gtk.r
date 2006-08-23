# Contains functions for accessing pieces of the GGobi user interface. 
# These are useful for embedding in other applications or for listening
# to their signals via ie RGtk.

getMainWindow.ggobi <-
function(.gobi = getDefaultGGobi())
{
  .GGobiCall("getMainWindow", .gobi = .gobi)
}
getMenuBar.ggobi <-
function(.gobi = getDefaultGGobi())
{
  .GGobiCall("getMenubar", .gobi = .gobi)
}



as.RGtkObject <- 
function(x)
{
 if(require(RGtk2))
	 class(x) <- c(class(x), "RGtkObject")
 x
}
