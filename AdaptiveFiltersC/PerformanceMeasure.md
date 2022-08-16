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
|whole code|all|10929288|9078818|10929321|2048|0|1609728|1155|
|1 iteration|all|6328|4454|6361|1|0|786|990|
|shift|66-69|155|50|201|0|0|0|143|
|filter_x dot product filter_w|72-74|227|57|215|0|0|0|154|
|gain vector (element-wise filter_x * LMBDA_INV)|79-81|154|39|153|0|0|0|110|
|gain vector (gemv)|83|1345|985|1344|0|0|256|99|
|gain vector (filter_x dot product gain vector)|85-87|213|56|212|0|0|0|154|
|gain vector (element wise div)|91-93|324|55|323|0|0|16|154|
|filter_w update|96-98|274|73|273|0|0|0|198|
|P update (gemv)|101|1290|985|1300|0|0|256|55|
|P update (outer)|104|1555|1195|1554|0|0|256|99|
|P update (element-wise sub and mult)|107-112|1230|1051|1229|0|0|0|187|

## Block Normalized LMS

|code portion|lines|total cycles|instructions|active cycles|external loads(L2+synch)|TCDM cont|LS stalls|I$ misses|
|---|---|---|---|---|---|---|---|---|
|whole code|all|1247800|836386|1247833|2304|0|144128|1331|
|8 iterations|all|6041|3302|6074|9|0|563|1166|
|buffers update|72-76|51|24|50|1|0|0|0|
|hankel matrix H|87-94|919|574|917|0|0|36|275|
|H gemv filter_w|97|738|504|726|0|0|128|88|
|calculate error (difference)|99-101|154|39|153|0|0|0|110|
|H norm 2 along axis 1|104-111|422|287|421|0|0|0|132|
|transpose H|114|564|331|552|0|0|128|88|
|divide by norm|116-120|1427|418|1426|0|0|128|110|
|filter_w update (gemv)|123|809|599|819|0|0|128|88|
|filter_w update (vector by scalar)|125-127|285|73|284|1|0|0|198|
|shift (plp_copy)|132|115|50|125|0|0|15|55|
|shift (copy some values)|134-136|139|37|149|0|0|0|110|

## Block RLS

|code portion|lines|total cycles|instructions|active cycles|external loads(L2+synch)|TCDM cont|LS stalls|I$ misses|
|---|---|---|---|---|---|---|---|---|
|whole code|all|11324335|9053467|11324379|2304|0|1980416|2750|
|8 iterations|all|46081|35393|46125|9|0|7736|1870|
|buffers update|87-91|51|24|50|1|0|0|0|
|hankel matrix H|102-109|804|481|802|0|0|36|253|
|H gemv filter_w|112|749|504|748|0|0|128|110|
|calculate error (difference)|114-116|154|39|153|0|0|0|110|
|transpose H|119|566|331|565|0|0|128|99|
|gain vector (pi = matrix mult P * H_t)|122|10162|7911|10150|0|0|2048|165|
|gain vector (transpose pi)|125|570|379|569|0|0|128|55|
|gain vector (aux = matrix mult H * pi)|128|5086|3974|5074|0|0|1024|55|
|gain vector (aux1 = diag + aux)|131|442|301|430|0|0|1|121|
|gain vector (aux = transpose aux)|132|318|202|317|0|0|64|44|
|gain vector (aux1 = inv aux)|141|6556|4586|6555|1|0|836|363|
|gain vector (g = matrix mult aux1 * pi_t)|142|5866|4743|5685|0|0|1024|77|
|gain vector (aux = transpose pi)|143|541|330|529|0|0|128|77|
|filter_w update (gemv)|146|832|600|842|0|0|128|110|
|filter_w update (sum)|148-150|287|75|275|0|0|0|198|
|P update (outer = mat mult g * pi_t)|155|11641|9447|11640|0|0|2048|110|
|P update (element-wise sub and mult)|160-165|1541|1307|1540|0|0|0|231|
|shift (plp_copy)|170|148|50|147|0|0|15|77|
|shift (copy some values)|172-174|139|37|138|0|0|0|99|