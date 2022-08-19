# Block NLMS Optimization

Optimization hereafter reported are applied considering graphs in the *AdaptiveFiltersPy* folder.

## hankel matrix H

The problem wih the construction of a hanlel matrix, is that the first inner loop is slow due to stall. Quite problematic to remove all stalls since at each iteration of the outer loop, the number of iteration of the first inner loop changes.

## H gemv filter_w

Naive implementation of gemv, just add loop unrolling x4 as done for rls.
Loop unrolling x4 (on outer loop):

> - total cycles = 593
> - instructions = 386
> - active cycles = 581
> - external loads (L2+synch) = 0
> - TCDM cont = 0
> - LD stalls = 0
> - I$ misses = 187

## H norm2 along axis 1

## transpose H

Stall as always, loop unrolling x4 on the outer loop:

> - total cycles = 510
> - instructions = 374
> - active cycles = 521
> - external loads (L2+synch) = 0
> - TCDM cont = 0
> - LD stalls = 0
> - I$ misses = 143

## divide by norm

## filter_w update (gemv)

Naive implementation of gemv, just add loop unrolling x4 as done for rls.
Loop unrolling x4 (on outer loop):

> - total cycles = 585
> - instructions = 431
> - active cycles = 595
> - external loads (L2+synch) = 0
> - TCDM cont = 0
> - LD stalls = 0
> - I$ misses = 154

## filter_w update (vector by scalar)

