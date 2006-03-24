getPlugins.ggobi <-
function(.gobi = getDefaultGGobi())
{
 .GGobiCall("getPlugins", .gobi = .gobi)
}

getPluginInfo.ggobi <-
function(.gobi = NULL)
{
 .GGobiCall("getPluginInfo", .gobi = NULL)
}

