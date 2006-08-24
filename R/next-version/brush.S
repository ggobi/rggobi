# GGobi brushing (size, location, glyph, etc)

getBrushSize.ggobi <- function(.data = 1, .gobi = ggobi_get(), units=0) {
	# Passing negative dimensions means that we just get the current value back 
	setBrushSize.ggobi(as.integer(-1), as.integer(-1), .gobi=.gobi)
}

brush_location.ggobi <- function(.data=1, .gobi = ggobi_get(), units=0) {
 setBrushLocation.ggobi(as.integer(-1), as.integer(-1), .data, .gobi=.gobi)
}

setBrushLocation.ggobi <- function(x, y, .data = 1, .gobi = ggobi_get(), update = TRUE, units = 0) {
 if(length(x) > 1 && missing(y)) { 
   x <- as.integer(x)
   y <- as.integer(x[2]) 
 }

 if(mode(.data) == "numeric")
   .data <- as.integer(.data - 1)

 tmp <- .GGobiCall("setBrushLocation", as.integer(c(x[1],y[1])), .data, .gobi = .gobi)
 if(!is.null(tmp))
   names(tmp) <- c("x", "y")

 return(tmp)
}


setBrushSize.ggobi <- function(w, h, .data = 1, .gobi = ggobi_get(), update = TRUE, units = 0) {
 if(length(w) > 1 && missing(h)) { 
   w <- as.integer(w)
   h <- as.integer(w[2]) 
 }

 if(mode(.data) == "numeric")
   .data <- as.integer(.data - 1)

 tmp <- .GGobiCall("setBrushSize", as.integer(c(w[1],h[1])), .data, .gobi = .gobi) 
 if(!is.null(tmp))
   names(tmp) <- c("width", "height")

 return(tmp)
}



setBrushColor.ggobi <- function(id, .gobi = ggobi_get()) {
 if(is.character(id)) {
   which <- match(id,  rownames(getColorMap.ggobi(.gobi)))
   if(any(is.na(which))) { 
     stop(paste("Unrecognized color(s)", id[is.na(which)]))
   }
   id <- which
 }
  .GGobiCall("setBrushColor", as.integer(id-1), .gobi=.gobi)
}

getBrushColor.ggobi <- function(.gobi = ggobi_get()) {
  .GGobiCall("getBrushColor", .gobi=.gobi)
}

setBrushGlyph.ggobi <- function(type = -1, size = -1, .gobi = ggobi_get()) {

 if(missing(type) & missing(size))
  stop("Must specify a glyph size or type")

 if(is.character(type)) {
   type <- mapGlyphType(type)
 }

 .GGobiCall("setBrushGlyph", as.integer(c(type, size)), .gobi=.gobi)
 return(TRUE)
}

getBrushGlyph.ggobi <- function(.gobi = ggobi_get()) {
 x <- .GGobiCall("getBrushGlyph", .gobi = .gobi)
 if(is.null(x))
   return(x)

 n <- getGlyphTypes.ggobi()

 names(x) <- c( names(n)[x[1] == n], "size")
 x
}

getGlyphTypes.ggobi <- function() .GGobiCall("getGlyphTypes")
getGlyphSizes.ggobi <- function() .GGobiCall("getGlyphSizes")


