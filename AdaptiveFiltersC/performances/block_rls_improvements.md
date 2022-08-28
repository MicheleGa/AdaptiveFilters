# Block RLS Optimization

Optimization hereafter reported are applied considering graphs in the *AdaptiveFiltersPy* folder.

## hankel matrix H

Same problem seen with Block NLMS. Just changing the way of building the H matrix:

> - total cycles = 594
> - instructions = 160
> - active cycles = 604
> - external loads (L2+synch) = 0
> - TCDM cont = 0
> - LD stalls = 0
> - I$ misses = 440

## H gemv filter_w

Just loop unroll as in the other algorithms:

> - total cycles = 582
> - instructions = 386
> - active cycles = 581
> - external loads (L2+synch) = 0
> - TCDM cont = 0
> - LD stalls = 0
> - I$ misses = 187

## transpose H

Changed to a hand-made version to use loop unrolling instead of trasnpose from plp_math.h

> - total cycles = 524
> - instructions = 374
> - active cycles = 523
> - external loads (L2+synch) = 0
> - TCDM cont = 0
> - LD stalls = 0
> - I$ misses = 143

## gain vector (pi = matrix mult P * H_t)

> - total cycles = 6048
> - instructions = 5814
> - active cycles = 6047
> - external loads (L2+synch) = 0
> - TCDM cont = 0
> - LD stalls = 0
> - I$ misses = 165

## gain vector (transpose pi)

Changed to a hand-made version to use loop unrolling instead of trasnpose from plp_math.h

> - total cycles = 435
> - instructions = 351
> - active cycles = 445
> - external loads (L2+synch) = 0
> - TCDM cont = 0
> - LD stalls = 0
> - I$ misses = 88

## gain vector (aux = matrix mult H * pi)

Changed to a hand-made version to use loop unrolling instead of trasnpose from plp_math.h

> - total cycles = 3059
> - instructions = 2934
> - active cycles = 3058
> - external loads (L2+synch) = 0
> - TCDM cont = 0
> - LD stalls = 0
> - I$ misses = 88

## gain vector (aux1 = diag + aux)

Just the average loop unroll x4:

> - total cycles = 426
> - instructions = 297
> - active cycles = 414
> - external loads (L2+synch) = 0
> - TCDM cont = 0
> - LD stalls = 1
> - I$ misses = 110

## gain vector (aux = transpose aux)

Changed to a hand-made version to use loop unrolling instead of trasnpose from plp_math.h

> - total cycles = 256
> - instructions = 205
> - active cycles = 255
> - external loads (L2+synch) = 0
> - TCDM cont = 0
> - LD stalls = 0
> - I$ misses = 44

## gain vector (aux1 = inv aux)

## gain vector (g = matrix mult aux1 * pi_t)

Changed to a hand-made version to use loop unrolling instead of trasnpose from plp_math.h

> - total cycles = 3480
> - instructions = 3268
> - active cycles = 3490
> - external loads (L2+synch) = 0
> - TCDM cont = 0
> - LD stalls = 0
> - I$ misses = 154

## gain vector (aux = transpose pi)

Changed to a hand-made version to use loop unrolling instead of trasnpose from plp_math.h

> - total cycles = 456
> - instructions = 374
> - active cycles = 455
> - external loads (L2+synch) = 0
> - TCDM cont = 0
> - LD stalls = 0

## filter_w update (gemv)

Just loop unroll as in the other algorithms:

> - total cycles = 609
> - instructions = 433
> - active cycles = 619
> - external loads (L2+synch) = 0
> - TCDM cont = 0
> - LD stalls = 0
> - I$ misses = 176

## filter_w update (sum)

## P update (outer = mat mult g * pi_t)

Changed to a hand-made version to use loop unrolling instead of trasnpose from plp_math.h

> - total cycles = 6710
> - instructions = 6469
> - active cycles = 6720
> - external loads (L2+synch) = 0
> - TCDM cont = 0
> - LD stalls = 0
> - I$ misses = 121

## P update (element-wise sub and mult)

## shift (plp_copy)

As in Block NLMS, copy function from plp_math.h had some stalls. Just switched to hand-made one.

> - total cycles = 139
> - instructions = 37
> - active cycles = 127
> - external loads (L2+synch) = 0
> - TCDM cont = 0
> - LD stalls = 0
> - I$ misses = 99

Overall speed up:

> - total cycles = 7508258
> - instructions = 6961691
> - active cycles = 7508302
> - external loads (L2+synch) = 2304
> - TCDM cont = 0
> - LD stalls = 214272
> - I$ misses = 3377