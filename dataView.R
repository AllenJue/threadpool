library(dplyr)
library("viridis")         

avg.seq.time = 9.671273700
avg.seq.fail = 5 / (42 * 10)

df.c.time = read.csv("./Downloads/Cs378-Concurrency/threadpool/timing/c_timing.csv")
df.my.time = read.csv("./Downloads/Cs378-Concurrency/threadpool/timing/my_timing.csv")

c.speedup = avg.seq.time / df.c.time$execTime 
my.speedup = avg.seq.time / df.my.time$execTime

df.c.time$execTime = c.speedup
df.my.time$execTime = my.speedup

df.c.time
df.my.time
### Methods to help me plot graphs and overlay them
plotGraph2 <- function(results, results2, title, legend, colors) {
  plot(log(results$threads),
       results$execTime, 
       type = "o",
       main = title,
       xlab = "Log(Number of Threads in Threadpool)",
       ylim=range(0, max(results$execTime) * 1.1),
       ylab = "Speedup",
       col = colors[1],
       pch = 19,
       lwd = 3)
  lines(
    log(results2$threads),
    results2$execTime,
    type = "o",
    col = colors[2],
    lwd = 3
  )
  legend("topleft", legend = legend, 
         fill = colors, cex = 0.8)
}

plotGraph <- function(results, title, legend, colors) {
  plot(log(results$threads),
       results$execTime, 
       type = "o",
       main = title,
       xlab = "Log(Number of Threads in Threadpool)",
       ylim=range(0, max(results$execTime) * 1.1),
       ylab = "Speedup",
       col = colors[1],
       pch = 19,
       lwd = 3)
  legend("topleft", legend = legend, 
         fill = colors, cex = 0.8)
}

palette_colors <- viridis(8)

plotGraph(df.c.time, "Boost ASIO Threadpool Speedup", c("Boost ASIO"), c("peach"))
plotGraph(df.my.time, "Personal Threadpool Implementation Speedup", c("Personal Threadpool"), c("steelblue"))

plotGraph2(df.c.time, 
           df.my.time, 
           "Boost ASIO and Personal Implementation Speedup Comparison",
           c("Boost ASIO", "Personal Threadpool"),
           c("pink", "steelblue"))


