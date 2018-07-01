## A good start, but tries to generate some non-existent symbols,
## and getStructSizes() needs to be added manually.
generate_native_routine_registration_skeleton <- function(path = ".") {
    fun_orig <- fun <- tools:::native_routine_registration_db_from_ff_call_db
    b <- body(fun)
    b[[3]] <- quote(ff_call_names <- c(".GGobiCall", ".GGobiC"))
    b[[4]] <- quote(ff_call_args <-
                        lapply(ff_call_names,
                               function(e) args(get(e, getNamespace("rggobi")))))
    b2 <- b[[10L]][[3L]][[3L]][[3L]]
    b2[[11L]] <- quote(s <- e[[".name"]])
    b[[10L]][[3L]][[3L]][[3L]] <- b2
    manip <- quote({
        nrdb[,1L] <- sub("GGobi", "", nrdb[,1L])
        nrdb[,2L] <- rggobi:::.ggobi.symbol(nrdb[,2L])
        nrdb[,3L] <- nrdb[,3L] + 1
    })
    b <- as.call(c(as.list(head(b, 14L)), list(manip), as.list(tail(b, -14L))))
    body(fun) <- b
    assignInNamespace("native_routine_registration_db_from_ff_call_db", fun,
                      getNamespace("tools"))
    
    fun2_orig <- fun2 <- tools:::package_ff_call_db
    b <- body(fun2)
    b[[2]] <- quote(ff_call_names <- c(".GGobiCall", ".GGobiC"))
    body(fun2) <- b
    assignInNamespace("package_ff_call_db", fun2, getNamespace("tools"))

    tools::package_native_routine_registration_skeleton(path, "rggobi_init.c")

    assignInNamespace("native_routine_registration_db_from_ff_call_db", fun_orig,
                      getNamespace("tools"))
    assignInNamespace("package_ff_call_db", fun2_orig, getNamespace("tools"))
}
