# Scrolling along long time series
# ------------------------------------

df <- data.frame(x=1:10000, y=sin(1:10000 * pi/20) + runif(10000, max=0.5))
g <- ggobi_longitudinal(df[1:100, ])

df_g <- g[1]
for(i in 1:2000) {
	df_g[, 2] <- df[i:(i + 99), 2]
}


# Edges example
# ------------------------------------

library(graph)
library(SNAData)

data(business)
data(marital)
data(florentineAttrs)

florentine <- data.frame(florentineAttrs, business.links = degree(business), marital.links = degree(marital))

g <- ggobi(florentine)
edges(g) <- business
edges(g) <- marital


# Maximum likelihood
# ------------------------------------

# At each iteration, want to show values of objective function
# + values of each of the parameters
# 
# scrolling might be tricky

fr <- function(x) {   ## Rosenbrock Banana function
    x1 <- x[1]
    x2 <- x[2]
    100 * (x2 - x1 * x1)^2 + (1 - x1)^2
}
grr <- function(x) { ## Gradient of 'fr'
    x1 <- x[1]
    x2 <- x[2]
    c(-400 * x1 * (x2 - x1 * x1) - 2 * (1 - x1),
       200 *      (x2 - x1 * x1))
}

mldf <- data.frame(iter=1:100, p1=0, p2=0, val=0)
par <- c(-1.2,1)
op <- optim(par, fr, grr, method = "BFGS", control=list(maxit=1))
mldf[1, 2] <- op$par[1]
mldf[1, 3] <- op$par[2]
mldf[1, 4] <- op$value

g <- ggobi_longitudinal(mldf)
mldf_g <- g[1]


for(i in 2:100) {
	op <- optim(op$par, fr, grr, method = "BFGS", control=list(maxit=1))
	mldf_g[i, 2] <- op$par[1]
	mldf_g[i, 3] <- op$par[2]
	mldf_g[i, 4] <- op$value	
}


# keamns
# ------------------------------------

x <- rbind(matrix(rnorm(100, sd = 0.3), ncol = 2),
            matrix(rnorm(100, mean = 1, sd = 0.3), ncol = 2))
colnames(x) <- c("x", "y")
c1 <- kmeans(x, 2, iter.max=1)
c2 <- kmeans(x, 2, iter.max=1, centers=c1$centers)