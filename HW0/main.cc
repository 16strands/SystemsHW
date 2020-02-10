#include <array>
#include <random>
#include <iostream>
#include <chrono>
#include <numeric>
#include <string>

float *generate_random_list(int size, float bound)
{

    float *ret = new float[size];
    std::mt19937 generator(size);
    std::uniform_real_distribution<> distribution(-bound, bound);
    for (int n = 0; n < size; n++)
    {
        ret[n] = distribution(generator);
    };
    return ret;
}

void update_coords(float *xs, float *ys, float *zs, float *vx, float *vy, float *vz, int size)
{
    for (int i=0; i<size; i++)
    {
        xs[i] = xs[i] + vx[i];
        ys[i] = ys[i] + vy[i];
        zs[i] = zs[i] + vz[i];
    }
}

int main(int argc, char *argv[])
{
    if (argc < 3){
        std::cout << "Required arguments : vector_length(N) and iterations_num(M)" << std::endl;
        return 1;
    };
    int size = std::stoi(argv[1]);
    int iterations_num = std::stoi(argv[2]);

    float *xs = generate_random_list(size, 1000.0);
    float *ys = generate_random_list(size, 1000.0);
    float *zs = generate_random_list(size, 1000.0);
    float *vx = generate_random_list(size, 1.0);
    float *vy = generate_random_list(size, 1.0);
    float *vz = generate_random_list(size, 1.0);

    auto t1 = std::chrono::high_resolution_clock::now();
    for (int i=0; i<iterations_num; i++)
    {
        update_coords(xs, ys, zs, vx, vy, vz, size);
    };
    auto t2 = std::chrono::high_resolution_clock::now();
    int chksum = 0;
    chksum = std::accumulate(xs, xs+size, chksum);
    chksum = std::accumulate(ys, ys+size, chksum);
    chksum = std::accumulate(zs, zs+size, chksum);
    auto time = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    double time2 = (double) time / (double) (size * iterations_num);
    // std::cout << "Mean time per coordinate : " << time2 << "us" << std::endl;
    // std::cout << "Final checksum: " << std::to_string(chksum) << std::endl;
    std::cout << size << ", " << time2 << ", " << std::to_string(chksum)<< std::endl;

    return 0;
};

