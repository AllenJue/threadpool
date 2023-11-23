library(dplyr)
library("viridis")         

avg.seq.time = 3.461231200
avg.seq.fail = 0
avg.seq.fail

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
  plot(log2(results$threads),
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
    log2(results2$threads),
    results2$execTime,
    type = "o",
    col = colors[2],
    lwd = 3
  )
  legend("topright", legend = legend, 
         fill = colors, cex = 0.8)
  # Add a red horizontal line at y = 1
  abline(h = 1, col = "red", lty = 2)
}

plotGraph <- function(results, title, legend, colors) {
  plot(log2(results$threads),
       results$execTime, 
       type = "o",
       main = title,
       xlab = "Log(Number of Threads in Threadpool)",
       ylim=range(0, max(results$execTime) * 1.1),
       ylab = "Speedup",
       col = colors[1],
       pch = 19,
       lwd = 3)
  legend("topright", legend = legend, 
         fill = colors, cex = 0.8)
}

### Methods to handle graphing fails
plotGraphFail <- function(results, title, legend, colors) {
  plot(log2(results$threads),
       results$totalFails, 
       type = "o",
       main = title,
       xlab = "Log(Number of Threads in Threadpool)",
       ylim=range(0, max(results$totalFails) * 1.1),
       ylab = "Fail Increase",
       col = colors[1],
       pch = 19,
       lwd = 3)
  legend("topleft", legend = legend, 
         fill = colors, cex = 0.8)
}

plotGraphFails2 <- function(results, results2, title, legend, colors) {
  plot(log2(results$threads),
       results$totalFails, 
       type = "o",
       main = title,
       xlab = "Log(Number of Threads in Threadpool)",
       ylim=range(0, max(max(results$totalFails), max(results2$totalFails)) * 1.1),
       ylab = "Fail Increase",
       col = colors[1],
       pch = 19,
       lwd = 3)
  lines(
    log2(results2$threads),
    results2$totalFails,
    type = "o",
    col = colors[2],
    lwd = 3
  )
  legend("topleft", legend = legend, 
         fill = colors, cex = 0.8)
  # Add a red horizontal line at y = 1
  abline(h = 1, col = "red", lty = 2)
}

palette_colors <- viridis(8)

plotGraph(df.c.time, "Boost ASIO Threadpool Speedup", 
          c("Boost ASIO"), c(palette_colors[1]))

plotGraph(df.my.time, "Personal Threadpool Implementation Speedup", 
          c("Personal Threadpool"), c(palette_colors[6]))

plotGraph2(df.c.time, 
           df.my.time, 
           "Boost ASIO and Personal Implementation Speedup Comparison",
           c("Boost ASIO", "Personal Threadpool"),
           c(palette_colors[1], palette_colors[6]))

df.c.fails = read.csv("./Downloads/Cs378-Concurrency/threadpool/timing/c_fail.csv")
df.my.fails = read.csv("./Downloads/Cs378-Concurrency/threadpool/timing/my_fail.csv")

# calculate the change in avg fail rate for the threadpools
df.c.total.fails <- aggregate(totalFails ~ threads, data = df.c.fails, sum)
df.c.total.fails$totalFails = (df.c.total.fails$totalFails / (42 * 10)) / avg_seq_fail 
df.c.total.fails

df.my.total.fails <- aggregate(totalFails ~ threads, data = df.my.fails, sum)
df.my.total.fails$totalFails =  (df.my.total.fails$totalFails / (42 * 10)) / avg_seq_fail 
df.my.total.fails

plotGraphFail(df.c.total.fails, "Boost ASIO Threadpool Fail Increase", 
          c("Boost ASIO"), c(palette_colors[1]))

plotGraphFail(df.my.total.fails, "Personal Threadpool Implementation Fail Increase", 
          c("Personal Threadpool"), c(palette_colors[6]))

plotGraphFails2(df.c.total.fails, 
           df.my.total.fails, 
           "Boost ASIO and Personal Implementation Fails Comparison",
           c("Boost ASIO", "Personal Threadpool"),
           c(palette_colors[1], palette_colors[6]))
