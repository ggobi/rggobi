#
#
#
#
plotLayout <-
function(..., mfrow = NULL, cells = NULL, .data = 1, .gobi=getDefaultGGobi(), display = -1)
{
 if(mode(display) == "numeric") {
   display <- as.integer(display)
 } else if(inherits(display, "ggobiDisplay"))
      display <- display[["ref"]]

 plots <- list(...)
 n <- length(plots)

 if(n == 0)
    plots <- NULL
 else {
    plots <- lapply(plots, resolvePlotDescription, .data = .data, .gobi = .gobi)
 }

  # We can compute this if it is missing and cells is not.
 if(length(mfrow) == 0) {
   nrows <- n # ceiling(sqrt(n))
   ncols <- 1 # sqrt(n)
   mfrow <- c(nrows, ncols)
 }

 if(missing(cells))
   cells <- t(gtkCells(mfrow[1], mfrow[2]))
 else {
      # We'll transpose gtkCells for you.
    if(ncol(cells) == 4)
      cells <- t(cells)
 }

 if(mode(.data) == "numeric") {
   .data <- as.integer(.data -1)
 }

 if(mode(.gobi) == "numeric") {
   .gobi <- as.integer(.gobi -1)
 }

 val <- .Call(.ggobi.symbol("createPlots"), plots, as.integer(mfrow), as.integer(cells-1),
               .gobi, display, .data, PACKAGE = "rggobi")

 gdk.flush()

 val
}

gtkCells <-
function(r, c)
{
 x <- rep(1:c, r)
 y <- rep(1:r, rep(c, r))
 l <- cbind(left = x, right = x+1, top = y, bottom = y+1)
 l
}
