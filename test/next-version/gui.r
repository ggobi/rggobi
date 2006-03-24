library(Rggobi)
data(mtcars)
g = ggobi(mtcars) # , args = c("--sync", "--g-fatal-warnings"))

library(RGtk)

if(FALSE) {
 dpy = .GGobiCall("createDisplay", TRUE, as.integer(0), .gobi  = g)
 class(dpy) = gtkObjectGetClasses(dpy, FALSE)
 dpy$Show()
} else
 dpy = NULL


tbl = gtkTable(3,2, TRUE)

#XXX These embedded displays don't have a name/label in the display tree.

p1 = .GGobiCall("createScatterPlot", as.integer(100, 100), dpy, .gobi = g)
class(p1) = gtkObjectGetClasses(p1, FALSE)

p2 = .GGobiCall("createScatterPlot", as.integer(100, 100), dpy, .gobi = g)
class(p2) = gtkObjectGetClasses(p2, FALSE)


if(TRUE) {

 tbl$Attach(p1, 0, 1, 0, 1)
 tbl$Attach(gtkButton("B"), 1, 2, 1, 2)
 tbl$Attach(p2, 0, 1, 2, 3)

 win = gtkWindow()
  # If we don't create the outer display
  # then we had better make certain there is enough space
  # in the window before we add the table to the window.
  # Otherwise we will get a warning about the rulers.
 win$SetUsize(400,400)
 if(is.null(dpy)) {
   win$Add(tbl)
 } else {
   dpy$Add(tbl)
   win$Add(dpy)
 }
}

# Done at end so that we don't get errors/warnings
# about size issues.
p1$ShowAll(); p2$ShowAll()

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

setMode.ggobi("Identify")
