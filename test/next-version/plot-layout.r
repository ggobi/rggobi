plot4 <- ashDescription("tars2")
plot5 <- scatmatrixDescription("tars1", "tars2", "head")
plot6 <- parallelCoordDescription("tars1", "tars2", "head")

g <- ggobi("../ggobi/data/flea.xml")

cl <- gtkCells(2,2)[-3,]
cl[3,1] <- 1

plotLayout(plot4, plot5, plot6, mfrow=c(2,2), cells=t(cl))


plot4 <- scatterplotDescription("tars1", "tars2")
plot5 <- scatmatrixDescription("tars1", "tars2", "head")

g <- ggobi("../ggobi/data/flea.xml")
dpy <- plotLayout(mfrow=c(3,2))
plotLayout(plot4, plot5, cells=t(gtkCells(3,2)[c(1,4),]), display = dpy)


plot4 <- scatterplotDescription("tars1", "tars2")
plot5 <- scatmatrixDescription("tars1", "tars2", "head")

g <- ggobi("../ggobi/data/flea.xml")
dpy <- plotLayout(plot4, mfrow=c(3,2))
plotLayout(plot5, cells=t(gtkCells(3,2)[c(1,4),]), display = dpy)
