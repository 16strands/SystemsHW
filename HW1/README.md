# HW 1 - Sarah Strand

### Part 1

I used two random lists for this assignment. Both lists were of length size_of_buffer, and one of them had random values in range (0, 255), and the other had random values in range (0, size_of_buffer). The first list was the buffer itself (with values from 0-255 because I used type uint8_t to ensure the objects in the list were one byte), and the second list was a list of random indices within the range of the buffer. In retrospect, I should have just populated the list from 0-size_of_buffer and then shuffled it, since my random function is unlikely to hit all the values, but running the bash script for this assignment takes a very long time and I don't know how much it will mess with my results. 

I used 
`g++ -O3 -std=c++17 -g -Werror -Wall -Wextra -pedantic -o prog main.cpp
`
to compile.

### Part 2

![alt text](https://github.com/16strands/SystemsHW/blob/master/HW1/graph.png "graph")

### Part 3