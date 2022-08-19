# RLS Optimization

Optimization hereafter reported are applied considering graphs in the *AdaptiveFiltersPy* folder.

## gain vector (gemv)

The problem with this portion is its naive implementation of gemv without loop unrolling, causing stalls.

Loop unroll x2 (on outer loop):

> - total cycles = 953
> - instructions = 805
> - active cycles = 952
> - external loads (L2+synch) = 0
> - TCDM cont = 0
> - LD stalls = 0
> - I$ misses = 143

Loop unroll x2 (on inner loop):

> - total cycles = 1189
> - instructions = 1052
> - active cycles = 1188
> - external loads (L2+synch) = 0
> - TCDM cont = 0
> - LD stalls = 0
> - I$ misses = 132

Loop unroll x2 x2 (on outer and inner loops):

> - total cycles = 1045
> - instructions = 853
> - active cycles = 1044
> - external loads (L2+synch) = 0
> - TCDM cont = 0
> - LD stalls = 0
> - I$ misses = 187

Loop unroll x4 (on inner loop):

> - total cycles = 1307
> - instructions = 1148
> - active cycles = 1306
> - external loads (L2+synch) = 0
> - TCDM cont = 0
> - LD stalls = 0
> - I$ misses = 154

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

Loop unroll x4 (on outer loop):

> - total cycles = 853
> - instructions = 721
> - active cycles = 863
> - external loads (L2+synch) = 0
> - TCDM cont = 0
> - LD stalls = 0
> - I$ misses = 132

## P update (outer)

Same problem as gain vector (gemv).

Loop unroll x4 (on outer loop):

> - total cycles = 1096
> - instructions = 926
> - active cycles = 1095
> - external loads (L2+synch) = 0
> - TCDM cont = 0
> - LD stalls = 0
> - I$ misses = 165

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

> - total cycles = 7748964
> - instructions = 7446562
> - active cycles = 7748997
> - external loads (L2+synch) = 2048
> - TCDM cont = 0
> - LD stalls = 36864
> - I$ misses = 1375