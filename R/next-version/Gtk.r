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



asGObject <-
function(obj)
{
 w <- unclass(obj)$ref
 if(require(RGtk2)) {
	 class(w) <- c(class(w), "RGtkObject")
 }
 w
}
