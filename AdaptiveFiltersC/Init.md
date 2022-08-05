# Performances of *init*

## Standard sequential version (only FC)

> - total cycles = 9559

## Parallel version (cluster on, 8 cores)

Parallelize just the data transfers from L2 to L1:

> - total cycles = 2247

Parallelize norm 2 and zeros:

> - total cycles = 2197