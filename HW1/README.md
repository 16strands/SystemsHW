# HW 1 - Sarah Strand

### Part 1

I used two random lists for this assignment. Both lists were of length size_of_buffer, and one of them had random values in range (0, 255), and the other had random values in range (0, size_of_buffer). The first list was the buffer itself (with values from 0-255 because I used type uint8_t to ensure the objects in the list were one byte), and the second list was a list of random indices within the range of the buffer. In retrospect, I should have just populated the list from 0-size_of_buffer and then shuffled it, since my random function is unlikely to hit all the values, but running the bash script for this assignment takes a very long time and I don't know how much it will mess with my results. 

I used 
`g++ -O3 -std=c++17 -g -Werror -Wall -Wextra -pedantic -o prog main.cpp
`
to compile.

### Part 2

Regular scale graph:

![alt text](https://github.com/16strands/SystemsHW/blob/master/HW1/graph.png "graph")

Log scale graph:

![alt text](https://github.com/16strands/SystemsHW/blob/master/HW1/logscale.png "log scale graph")


### Part 3


1. _My general method for guessing here was to look for changes in slope in the log scale graph, and then subtract a little to account for the data that would have been hit in the earlier caches. I looked up how many caches my machine has before analyzing this because my graph looked suspiciously fast at the end_.

    L1: It looks like my L1 cache size is around 262 KB, based on the fact that the first notable jump in times is around then. I think that if I had made the change (shuffling vs randomly filling the list, mentioned in Part 1), the difference may have been more noticeable. But, looking at the log scale version of the graph, there is a visible change in the slope of the graph at around that point. 

    L2: My L2 cache is probably around 1000 KB, given that there is another visible change in slope at that point in the log scale graph.

    L3: It would appear that my L3 cache is probably around 10000 KB.
    
2. My L1 and L2 cache times were approximately correct, but since I have a 3rd cache on my machine, my later reads were faster than the values we're supposed to compare to.

3. My cache sizes are: ![alt text](https://github.com/16strands/SystemsHW/blob/master/HW1/cachesizes.png "cache sizes") This aligns mostly with my guesses. I wasn't sure what the `4 x 32 KB` notation meant, but I assumed that I should just multiply and add accordingly. 

