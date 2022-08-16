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

In this folder, you can find the Python implementation of the adaptive filter algorithms: in the *results.txt* file data for checksum in the C code is reported.

## AdaptiveFiltersC

In this folder instead, you can find the C of the adaptive filter algorithms: for each single algorithm a .c file is present while in *PerformanceMeasure.md* for each single portion of code of the particular algotihm, the number of cycles and instrctions are reported in order to apply optimizations as parallelization.
