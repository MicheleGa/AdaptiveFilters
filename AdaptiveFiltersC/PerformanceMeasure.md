# Performance Recording

In this file, performances of the following algorithms are reported: Normalized Least Mean Sqaures, Recursive Least Squares, and the respective block forms.
Firstly, the performance of each single algorithm that runs on the whole 2048 samples with a 16-bit filter are reported (no compiler inlinig).
Secondly, a fine-grained recording of each portion of code that composes the algotihms are registered with the following methodology:

> - produce assembly with the command *make dis > foo.txt*
> - change the number of iterations to the smallest needed to record an update execution, i.e. 1 for the NLMS and RLS algorithms and 8 for the block versions
> - remove inlining of functions to get more readable traces and logs (that are reported in the folder *logs*)
> - initialize the performance counters
> - run the code: reset and start the perfomance counters before every target protion, while stop and print the statistics after the end of the target porotion

Notice that when a portion of *update* is surround with performance counters, execution flow will change and some compiler optimizations are no more effective.

## Normalized Least Mean Squares

|code portion|lines|total cycles|instructions|active cycles|external loads(L2+synch)|TCDM cont|LS stalls|I$ misses|
|---|---|---|---|---|---|---|---|---|
|whole code|all|623114|358434|623147|2048|0|36864|517|
|1 iteration|all|768|196|801|1|0|18|473|
|shift|57-60|153|50|186|0|0|0|132|
|filter_x dot product filter_w|63-65|213|56|212|0|0|0|154|
|filter_x dot product filter_x|68-70|153|40|152|0|0|0|110|
|filter_w update|75-77|444|87|443|0|0|16|242|

## Recursive Least Squares

|code portion|lines|total cycles|instructions|active cycles|external loads(L2+synch)|TCDM cont|LS stalls|I$ misses|
|---|---|---|---|---|---|---|---|---|
|whole code|all|623114|358434|623147|2048|0|36864|517|
||||||||||
||||||||||
||||||||||
||||||||||
||||||||||
