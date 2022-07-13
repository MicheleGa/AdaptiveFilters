# NLMS 

## Sequential Version

### Log
> - Final error: 0.130376
> - Ground truth error: 0.130042
> - Final filter w: [-0.085491, 0.327151, -0.001610, 0.012100, 0.148630, 0.272413, -0.062435, 0.124774, 0.039172, -0.144918, -0.338466, 0.374552, -0.469244, -0.071587, 0.559438, -0.002135]
> - Ground truth filter w: [-0.080043, 0.329554, -0.001368, 0.013135, 0.148226, 0.269431, -0.062278, 0.123105, 0.038721, -0.144997, -0.339531, 0.370853, -0.467417, -0.070823, 0.560770, 0.000000]
> - Error at position 0, got -0.085491 instead of -0.080043. You got 1 errors on final filter w array, with tollerance 100.

### Performances

With error reporting during adaptive filter loop (debug on):

> - total cycles = 658725
> - instructions = 256084
> - active cycles = 658758
> - external loads (L2+synch) = 16896
> - TCDM cont = 0
> - LD stalls = 5120
> - I$ misses = 1078

No debug:

> - total cycles = 80817
> - instructions = 38287
> - active cycles = 80861
> - external loads (L2+synch) = 799
> - TCDM cont = 0
> - LD stalls = 7680
> - I$ misses = 660

Added L1 allocation:

> - total cycles = 72916
> - instructions = 37256
> - active cycles = 72949
> - external loads (L2+synch) = 256
> - TCDM cont = 0
> - LD stalls = 7168
> - I$ misses = 616

## Parallel Version

init cycles:

sequential:9853

