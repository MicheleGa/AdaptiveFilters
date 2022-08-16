# Performance Recording

In this file, performances of the following algorithms are reported: Normalized Least Mean Sqaures, Recursive Least Squares, and the respective block forms.
Firstly, the performance of each single algorithm that run on the whole 2048 samples with a 16-bit filter are reported.
Secondly, a fine-grained recording of each portion of code that composes the algotihms are registered with the following methodology:

> - produce assembly with the command *make dis > foo.txt*
> - change the number of iterations to the smallest needed to record an update execution, i.e. 1 for the NLMS and RLS algorithms and 8 for the block versions
> - remove inlining of functions to get more readable traces and logs (that are reported in the folder *logs*)
> - initialize the performance counters
> - run the code: reset and start the perfomance counters before every target protion, while stop and print the statistics after the end of the target porotion

## NLMS


