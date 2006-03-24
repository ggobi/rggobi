gui2 =
function(dataset, viewMode = "Brush")
{
  library(Rggobi)
  if(missing(dataset)) {
    data(mtcars)
    dataset = mtcars
  }

   g = ggobi(dataset) # , args = c("--sync", "--g-fatal-warnings"))

  library(RGtk)

  tbl = gtkTable(3,2, TRUE)

  p1 = scatterplot.ggobi(1,2)
  class(p1$ref) = gtkObjectGetClasses(p1$ref, FALSE)
  k = ncol(g[[1]])
  p2 = scatterplot.ggobi(k-1,k)
  class(p2$ref) = gtkObjectGetClasses(p2$ref, FALSE)


  getDisplayWindow.ggobi(p1)$Hide()
  getDisplayWindow.ggobi(p2)$Hide()


if(TRUE) {


 box = gtkHBox()
 tbl$Attach(box, 0, 1, 0, 1)

  # Just want the plot, no rulers, menu, etc.
 tmp = p1$ref$GetChildren()[[2]]$GetChildren()[[3]]
 
 gtkWidgetReparent(tmp, box)
 tbl$Attach(gtkButton("B"), 1, 2, 1, 2)
  box = gtkHBox()
 tbl$Attach(box, 0, 1, 2, 3)
   # Here we put the display into the table, menus, rulers and all.
  gtkWidgetReparent(p2$ref, box)

 win = gtkWindow()
  # If we don't create the outer display
  # then we had better make certain there is enough space
  # in the window before we add the table to the window.
  # Otherwise we will get a warning about the rulers.
 win$SetUsize(400,400)
   win$Add(tbl)
}


# getDisplayWindow.ggobi(p1)$Destroy()
# getDisplayWindow.ggobi(p2)$Destroy()

# Done at end so that we don't get errors/warnings
# about size issues.
#p1$ShowAll(); p2$ShowAll()
win$ShowAll()

# Linking and brushing work fine from regular displays, i.e.
# actions in the regular display are linked to the other embedded
# displays.
#
# Making one of the embedded displays active
#
# but there is no ViewMode menu items or accelerator keys
# for the modes.
#
# We can use setMode.ggobi() to change the mode.
#  Identify works fine for the embedded plots/displays.
#  Brush however does not. It displays the brush, but as one
#  moves it, the points are not colored.
#  Interestingly, the number of points under the brush is displayed
#  and updated.
#
# There is no connection with the control panel.
#

setActivePlot.ggobi(2)
  
setMode.ggobi(viewMode)


 unclass(g)$ref$AddCallback("destroy", function(...) cat("Closing GGobi\n"))
  

 list(ggobi= g, table =tbl)
}
