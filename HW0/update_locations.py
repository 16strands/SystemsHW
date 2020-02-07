#!/usr/bin/env python3

import random, sys, timeit

###############
# Create a list of 'size' floating point numbers in the range [-bound, bound]
def generate_random_list(size, bound):
    return [random.uniform(-bound, bound) for i in range(size)]

###############
# Update position by velocity, one time-step
def update_coords(xs, ys, zs, vx, vy, vz):
    for i in range(len(xs)):
        xs[i] = xs[i] + vx[i]
        ys[i] = ys[i] + vy[i]
        zs[i] = zs[i] + vz[i]

############ Main:
if (len(sys.argv) != 3):
    print("Required arguments: vector_length(N) and iterations_num(M)")
    sys.exit(-1)
size = int(sys.argv[1])
iters = int(sys.argv[2])
random.seed(size)
xs = generate_random_list(size, 1000.)
ys = generate_random_list(size, 1000.)
zs = generate_random_list(size, 1000.)
vx = generate_random_list(size, 1.)
vy = generate_random_list(size, 1.)
vz = generate_random_list(size, 1.)
t = timeit.timeit(stmt = "update_coords(xs, ys, zs, vx, vy, vz)",
    setup = "from __main__ import update_coords, xs, ys, zs, vx, vy, vz",
    number = iters)
chksum = sum(xs) + sum(ys) + sum(zs)
print("Mean time per coordinate: " + str(1000000 * t / (size * iters)) + "us")
print("Final checksum is: " + str(chksum))
exit(0)
