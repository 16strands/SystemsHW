## HW0 - Sarah Strand

# Part 1
a. I chose the number of iterations each time based on whether running that many objects with that many iterations 3 times would stabilize, down to 0.01us. 
  * For 2^8-2^11 I chose 10000 iterations 
  * For 2^12-2^13 I chose 1000
  * For 2^14-2^16 I chose 100
  * For 2^17-2^19 I chose 10
  * For 2^20-2^24 I chose 5
  
 b. Since I ran each of them three times to test stability, I selected the middle of the three numbers as my final answer.

 c. See graph images in the repo.
 
 # Problem 2
* I used floats as my default type for the C++ version
* I've included the bash script I used to call the program repeatedly and pipe the output to a log file in comma separated value format. I then copied and pasted that info into a google sheet and created the graphs. There are 2-3 data points per num_objects. I removed outliers as I saw fit. 
* I decided on the num_iterations for each num_objects based on time constraints. I figured that including all three measured times on the scatterplot and creating a trend line would help give a good approximation of actual time. 
* I noticed a roughly 45 speedup from python to C++. See the graphs included in the repo. 

# Problem 3
* I was running low on time to spend on this assignment so I prioritized testing float, double, and double. I tried testing int8_t, but I couldn't figure out how to get around data loss / type conversion issues in the time I had left. 
* I saw that double and float were roughly the same speed (fastest), int64_t was in the middle, and python was the slowest. 

# Problem 4
* Didn't have time to work on this one. 