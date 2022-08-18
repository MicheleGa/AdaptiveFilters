# RLS Optimization

Optimization hereafter reported are applied considering graphs in the *AdaptiveFiltersPy* folder.

## gain vector (gemv)

Problem with this portion is its naive implementation of gemv without loop unrolling, causing stalls.
Naive version performance:

> - total cycles = 1345
> - instructions = 985
> - active cycles = 1344
> - external loads (L2+synch) = 0
> - TCDM cont = 0
> - LD stalls = 256
> - I$ misses = 99

Loop unroll x2 (on outer loop):

> - total cycles = 953
> - instructions = 805
> - active cycles = 952
> - external loads (L2+synch) = 0
> - TCDM cont = 0
> - LD stalls = 0
> - I$ misses = 143

Loop unroll x2 x2 (on outer and inner loops):

> - total cycles = 1045
> - instructions = 853
> - active cycles = 1044
> - external loads (L2+synch) = 0
> - TCDM cont = 0
> - LD stalls = 0
> - I$ misses = 187

Loop unroll x4 (on outer loop):

> - total cycles = 919
> - instructions = 721
> - active cycles = 918
> - external loads (L2+synch) = 0
> - TCDM cont = 0
> - LD stalls = 0
> - I$ misses = 187

Loop unroll x8 (on outer loop):

> - total cycles = 1009
> - instructions = 714
> - active cycles = 1008
> - external loads (L2+synch) = 0
> - TCDM cont = 0
> - LD stalls = 2
> - I$ misses = 286


## P update (gemv)

Same as just made for gain vector (gemv).
Without unrolling:

> - total cycles = 1290
> - instructions = 985
> - active cycles = 1300
> - external loads (L2+synch) = 0
> - TCDM cont = 0
> - LD stalls = 256
> - I$ misses = 55

Loop unroll x4 (on outer loop):

> - total cycles = 820
> - instructions = 721
> - active cycles = 830
> - external loads (L2+synch) = 0
> - TCDM cont = 0
> - LD stalls = 0
> - I$ misses = 99

## P update (outer)

Same problem as gain vector (gemv).
Without unrolling:

> - total cycles = 1555
> - instructions = 1195
> - active cycles = 1554
> - external loads (L2+synch) = 0
> - TCDM cont = 0
> - LD stalls = 256
> - I$ misses = 99

Loop unroll x4 (on outer loop):

> - total cycles = 748
> - instructions = 536
> - active cycles = 747
> - external loads (L2+synch) = 0
> - TCDM cont = 0
> - LD stalls = 64
> - I$ misses = 143

## P update (element-wise sub and mult)

TO-DO:

> - total cycles = 1230
> - instructions = 1051
> - active cycles = 1229
> - external loads (L2+synch) = 0
> - TCDM cont = 0
> - LD stalls = 0
> - I$ misses = 187

Overall speed-up with the all 2048 samples:

> - total cycles = 9559363
> - instructions = 9256994
> - active cycles = 9559396
> - external loads (L2+synch) = 2048
> - TCDM cont = 0
> - LD stalls = 36864
> - I$ misses = 1342