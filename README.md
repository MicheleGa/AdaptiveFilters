# Adaptive Filters
This is a porting of the following adaptive filter algorithms:

> - Normalized Least Mean Squares
> - Recursive Least Squares
> - Block form of LMS
> - Block form of RLS

Implementation taken from

[Pyroomacoustics](https://github.com/LCAV/pyroomacoustics/blob/master/examples/adaptive_filters.py)

and ported to C, more precisely for Parallel Ultra-Low Power platform

[PULP](https://github.com/pulp-platform)

## AdaptiveFiltersPy

In this folder, you can find the Python implementation of the adaptive filters algorithms.
The golden model results are written to *results.txt*, for checksum with the C code porting.

## AdaptiveFiltersC

In this folder instead, you can find the C of the adaptive filter algorithms: for each single algorithm a .c file is present, while in *PerformanceMeasure.md* for each single algorithm a fine grained perofrmane measure is recorded, including the number of cycles and instrctions (this allows to appreciate and understand gains of optimization and parallelization).

## Run instruction

Running the code could be a bit difficult: first is necessary to get the PULP virtual machine and then run it (I used VMWare, free version). 
Secondly, produce the python golden model my running *adaptive_filters.py* to write on results.txt.
Then the C code can run with the *make clean all run* command on the PULP platform.

## Third Party Code

As said, Adaptive Filters code is taken from PyRoomAcoustics repository, while the invert matrix function in the C code is taken from:

[https://github.com/pulp-platform/pulp-dsp](https://github.com/pulp-platform/pulp-dsp)