library(rggobi); library(butler)

source.with.err <- function(path) {
	tryCatch(source(path), error = function(x) {print(path); print(x)})
}

lapply(dir("~/ggobi/rggobi/R/", "\\.[Sr]$", full.name=T), source.with.err)