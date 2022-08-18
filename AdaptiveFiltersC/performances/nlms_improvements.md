# NLMS Optimization

Optimization hereafter reported are applied considering graphs in the *AdaptiveFiltersPy* folder.

## filter_w update

High number of cycles in the filter_w update is due to the mult and then div by floats.
But both mult and div by floats can be both pre-computed one time instead of at each iteration in the filter_w update.

Situation before optimization:

> - 1 mult by scalar, 1 div by scalar, 1 accumulate for each if the 16 coefficient = 32 scalar ops and 16 accumulate

After optimization

> - 1 mult before iterations, 1 mult inside loop, 1 accumulate = 17 scalar ops and 16 accumulate

Performances:

> - total cycles = 280
> - instructions = 72
> - active cycles = 279
> - external loads (L2+synch) = 0
> - TCDM cont = 0
> - LD stalls = 1
> - I$ misses = 198

Overall speed-up with the all 2048 samples:

> - total cycles = 375240
> - instructions = 327714
> - active cycles = 375273
> - external loads (L2+synch) = 2048
> - TCDM cont = 0
> - LD stalls = 4096
> - I$ misses = 451