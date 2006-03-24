setIdentifyHandler.ggobi <-
function(f, .gobi = getDefaultGGobi())
{
 gobj = unclass(.gobi)$ref
 gobj$AddCallback("identify-point", function(gg, sp, which, d)  f(which, d))
}


#
# This file contains functions to control the handlers
# for numbered key press events. It allows one to 
# register and un-register functions for specific keys.

NumberedKeyHandler.ggobi <-
function(...)
{
 handlers <- list()
 defaultHandler <- NULL

  addHandlers <- 
  function(...) {
     args <- list(...)
     if(any(is.na(match(names(args), as.character(0:9))))) {
        stop("Handlers must be identified by a name corresponding to one of the keys 0,..,9")
     }
     handlers[names(args)] <<- args
  }

  addDefaultHandler <-
  function(h) {
    defaultHandler <<- h
  }

  removeHandlers <-
  function(...) {
    which <- as.character(unlist(...))    
    idx <- match(which, names(handlers),nomatch=0)
    handlers <<- handlers[-idx]
  }

  internalHandler <-
  function(key, plot = NULL, ggobi = NULL, ev = NULL) {

   if(length(handlers) > 0 && !is.na(match(as.character(key), names(handlers)))) {
      f <- handlers[[as.character(key)]]
      f(key, plot, ggobi, ev)
   } else if(!is.null(defaultHandler))
       defaultHandler(key, plot, ggobi, ev)
    else 
       return(FALSE)

    TRUE
  }


 # The internalHandler must be last in this list. 
 # Please, please, ... make it the last one.
 return(list(addHandlers=addHandlers, removeHandlers=removeHandlers,
             addDefaultHandler = addDefaultHandler, 
             defaultHandler = function(){ return(defaultHandler) },
             handlers = function(){ return(handlers) },
             internalHandler = internalHandler))
}


addNumberedKeyHandler.ggobi <-
function(..., .gobi=getDefaultGGobi())
{
 handler <- .GetNumberedKeyHandler.ggobi(TRUE, .gobi=.gobi)
 handler$addHandlers(...)

 invisible(handler)
}

.GetNumberedKeyHandler.ggobi <-
#
# This interacts with the C internals to see if there is 
# already an R handler registered for the numbered keys. If
# there is, it returns that object. Otherwise, if create == T,
# it creates one by calling NumberedKeyHandler()
# (or uses the one specified as the argument to create if that 
function(create = FALSE, register=TRUE, .gobi= getDefaultGGobi())
{
  handler <- .GGobiCall("getNumberedKeyHandler", .gobi=.gobi)

  if(is.null(handler) && ( (is.logical(create) && create) || is.list(create))) {
   if(is.logical(create))
    handler <- NumberedKeyHandler.ggobi()
   else 
    handler <- create

    if(register)
      registerNumberedKeyHandler.ggobi(handler, .gobi=.gobi)  
  }

 handler
}

registerNumberedKeyHandler.ggobi <-
#
# Registers the specified handler with the specified ggobi instance
# as the event handler for key presses on the number keys.
#
function(handler = NumberedKeyHandler.ggobi(), .gobi= getDefaultGGobi(), description="R numbered key press event handler")
{
 .GGobiCall("setNumberedKeyHandler", handler, description, .gobi = .gobi)

 return(handler)
}
