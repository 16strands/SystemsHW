// main.cpp
// Sarah Strand

#include <iostream>
#include <chrono>
#include <random>

// Populate a uint32_t list with random data
void randomize(uint32_t* data, int length) {
    std::mt19937 generator(length); // Set up random number generator (with length as seed)
    std::uniform_real_distribution<> distribution(0, length);
    for (int n = 0; n < length; n++) {
        data[n] = distribution(generator); // Replace default vals with random nums
    };
}

// Populate a uint8_t list with random data
void randomize_buffer(uint8_t* buffer, int length) {
    std::mt19937 generator(length); // Set up random number generator (with length as seed)
    std::uniform_real_distribution<> distribution(0, 254);
    for (int n = 0; n < length; n++) {
        buffer[n] = (uint8_t) (distribution(generator)); // Replace default vals with random nums
    };
}

int main(int argc, char *argv[]){
    // Check for correct number of args, convert to usable types
    if (argc < 3)
    {
        std::cout << "Required arguments : buffer_length(N) and iterations_num(M)" << std::endl;
        return 1;
    };
    int length = std::stoi(argv[1]);
    int iterations_num = std::stoi(argv[2]);

    // Create buffer and random access_points list, randomize them
    uint8_t* buffer = new uint8_t[length];
    randomize_buffer(buffer, length);
    uint32_t* access_points = new uint32_t[length];
    randomize(access_points, length);

    // Set up var to use in finding minimum time
    double inf = std::numeric_limits<double>::infinity();
    double minTime = inf;

    // Initialize variable to sum vals with to fool the optimizer
    double curr = 0;

    // Iterate iterations_num times
    for (int i=0; i<iterations_num; i++) {
        // Set up timer
        auto t1 = std::chrono::high_resolution_clock::now();

        // Read entire buffer in random order according to access_points list
        for (int i = 0; i < length; i++) {
            // Sum to fool the optimizer
            curr += buffer[access_points[i]];
        };
        // Final time measurement, calculate time for each read
        auto t2 = std::chrono::high_resolution_clock::now();
        double time = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
        double adjTime = ((double) time / (double) (length)) * 1000;
        // Replace minTime if smaller time is found
        if (adjTime < minTime) {
            minTime = adjTime;
        }
    }

    // For pretty printing result to terminal
    // std::cout << "Mean time to read single byte : " << time2 << "ns" << std::endl;

    // For csv printing to terminal (designed to be piped to log file using >> in the bash script)
    std::cout << length << ", " << minTime << std::endl;

    //
    delete[] buffer;
    delete[] access_points;
    return curr;
}

/* Questions:
 * Should the buffer be populated or does it not matter?
 */

/* Notes:
 * Randomize accesses to confuse prefetcher - but make it fast so that it doesn't mess up time measurements
 * Repeat measurements and take the lowest
 * For each iteration, read the entire buffer
 */
