setIMode.ggobi <- function(name, .gobi = getDefaultGGobi()) {
 .GGobiCall("setIMode",  as.character(name), .gobi=.gobi)
}
setPMode.ggobi <- function(name, .gobi = getDefaultGGobi()) {
 .GGobiCall("setPMode",  as.character(name), .gobi=.gobi)
}

getPMode.ggobi <- function(.gobi = getDefaultGGobi())  .GGobiCall("getPModeName", .gobi=.gobi)
getIMode.ggobi <- function(.gobi = getDefaultGGobi())  .GGobiCall("getIModeName", .gobi=.gobi)

getPModeNames.ggobi <- function() .GGobiCall("getPModeNames")
getIModeNames.ggobi <- function() .GGobiCall("getIModeNames")

#
# Returns a description of the type
# of display which is the active or current
# one.
#
getCurrentDisplayType.ggobi <- function(.gobi = getDefaultGGobi()) {
  .GGobiCall("getCurrentDisplayType", .gobi = .gobi)
}

getDisplayTypes.ggobi <- function(.gobi = getDefaultGGobi()) {
 .GGobiCall("getDisplayTypes", .gobi = .gobi)
}

getNumDisplays.ggobi <- function(.gobi=getDefaultGGobi()) {
 .GGobiCall("getNumDisplays", .gobi=.gobi)
}
