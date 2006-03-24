library(Rggobi)
data(mtcars)

ggobi(mtcars)


# This is temporary until we put the class on the
# display reference when we create it.

d = getDisplays.ggobi()[[1]]

gtkAddCallback(d$ref, "tour_step", function(dpy, tour, i, gg, ...){ print(i); print(tour); print(gg)}, object = FALSE)


library(RGtk)
library(Rggobi)
data(mtcars)
gg = ggobi(mtcars)
gg$AddCallback("identify-point",
  function(gg, sp, id, d) {
    if(id > -1)
      print (getRowNames.ggobi(d)[id+1]);
  }
)


library(Rggobi)
library(RGtk)
data(mtcars)
g=ggobi(mtcars)
unclass(g)$ref$AddCallback("brush-motion", function(...) cat("Okay\n"))

