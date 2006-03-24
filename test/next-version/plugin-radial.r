# Run from $GGOBI_HOME for the moment.

library(Rggobi)

# init.ggobi(c("Rggobi", "--plugin=plugins/GraphLayout/plugin.xml"))
g = ggobi("~/Projects/ggobi/ggobi/data/snetwork.xml", args = "--plugin=plugins/GraphLayout/plugin.xml",
             "--plugin=plugins/GraphAction/plugin.xml")

info = getPluginInfo.ggobi()$GraphLayout

dll = dyn.load(paste(info@dll, .Platform$dynlib.ext, sep=""))



#g = ggobi("~/Projects/ggobi/ggobi/data/snetwork.xml")
inst = g$getPlugins()$GraphLayout

# .Call("S_radial_cb", inst, as.integer(-1), unclass(g[[1]])$data, unclass(g[[2]])$data, getDisplays.ggobi()[[1]], PACKAGE = dll)

fun = getNativeSymbolInfo("do_radial_plugin", PACKAGE="GraphLayout")$address

.Call("S_radial_cb", inst, as.integer(-1), unclass(g[[1]])$data, unclass(g[[2]])$data, getDisplays.ggobi()[[1]],
                      fun, PACKAGE = "Rggobi")

