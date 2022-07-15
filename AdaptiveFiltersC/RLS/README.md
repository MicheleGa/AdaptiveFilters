# RLS 

## Sequential Version

### Log

> - Final error: 0.036078
> - Ground truth error: 0.036015
> - Final filter w: [0.249569, 0.076749, -0.102774, -0.252551, -0.161999, -0.196209, -0.264801, 0.347877, -0.349686, 0.282876, 0.224842, -0.256641, -0.229464, -0.273502, -0.426800, 0.001739]
> - Ground truth filter w: [0.249793, 0.076620, -0.102520, -0.252501, -0.162050, -0.196180, -0.264864, 0.347911, -0.349829, 0.282966, 0.224732, -0.256671, -0.229391, -0.273377, -0.426682, 0.000000]

### Performances

With error reporting during adaptive filter loop (debug on, Struct and L1 allocation):

> - total cycles = 1194539
> - instructions = 1082408
> - active cycles = 1194561
> - external loads (L2+synch) = 512
> - TCDM cont = 0
> - LD stalls = 13568
> - I$ misses = 20174

No debug:

> - total cycles = 930415
> - instructions = 874811
> - active cycles = 930448
> - external loads (L2+synch) = 256
> - TCDM cont = 0
> - LD stalls = 26112
> - I$ misses = 1617