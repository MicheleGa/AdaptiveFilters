"""
Adaptive Filters Example
========================

In this example, we will run adaptive filters for system identification.
"""
from __future__ import division, print_function

import numpy as np 
from scipy.signal import fftconvolve
import math
import matplotlib.pyplot as plt
import pyroomacoustics as pra

results_file = 'results.txt'

def get_string_to_record(desc, data, data_name, length) -> str:
    string = f'{desc}:\n'
    string += f'static float {data_name}[' + str(length) + '] = {'
    for el in range(len(data) - 1):
        string += str(data[el]) + ', '
    string += str(data[length - 1]) + '}\n'

    return string

# recording file
with open(results_file, 'w') as rec_file:
    rec_file.write('')

with open(results_file, 'a') as rec_file:
    rec_file.write('**** Adaptive filter data ****\n')

    # parameters
    length = 16  # the unknown filter length
    n_samples = 256  # the number of samples to run
    SNR = 15  # signal to noise ratio

    # the unknown filter (unit norm)
    w = list(np.random.randn(length - 1))
    w.append(0.0)
    w = np.array(w)

    rec_file.write(get_string_to_record(desc='Unknown filter',
                                        data=w,
                                        data_name='w',
                                        length=length))

    w /= np.linalg.norm(w)

    # create a known driving signal
    x = np.random.randn(n_samples)

    rec_file.write(get_string_to_record(desc='Input driving signal',
                                        data=x,
                                        data_name='x',
                                        length=n_samples))

    # convolve with the unknown filter
    d_clean = fftconvolve(x, w)[:n_samples]

    # add some noise to the reference signal
    d = d_clean + np.random.randn(n_samples) * 10 ** (-SNR / 20.0)

    rec_file.write(get_string_to_record(desc='Convolve with unknown filter and add some noise',
                                        data=d,
                                        data_name='d',
                                        length=n_samples))

    # nlms
    filter=pra.adaptive.NLMS(length, mu=0.5)
    error=np.zeros(n_samples)

    for i in range(n_samples):
        filter.update(x[i], d[i])
        error[i] = np.linalg.norm(filter.w - w)
    
    rec_file.write(get_string_to_record(desc='Final nlms filter weights',
                                        data=filter.w,
                                        data_name='w_nlms_filter_final',
                                        length=length))
    
    rec_file.write(get_string_to_record(desc='Final nlms filter error',
                                        data=error,
                                        data_name='nlms_error',
                                        length=n_samples))
    
    # rls
    filter=pra.adaptive.RLS(length, lmbd=1.0, delta=2.0)
    error=np.zeros(n_samples)

    for i in range(n_samples):
        filter.update(x[i], d[i])
        error[i] = np.linalg.norm(filter.w - w)
    
    rec_file.write(get_string_to_record(desc='Final rls filter weights',
                                        data=filter.w,
                                        data_name='w_rls_filter_final',
                                        length=length))
    
    rec_file.write(get_string_to_record(desc='Final rls filter error',
                                        data=error,
                                        data_name='rls_error',
                                        length=n_samples))

    # block (n)lms
    filter=pra.adaptive.BlockLMS(length, mu=0.001, L=8, nlms=True)
    error=np.zeros(n_samples)

    for i in range(n_samples):
        filter.update(x[i], d[i])
        error[i] = np.linalg.norm(filter.w - w)
    
    rec_file.write(get_string_to_record(desc='Final block_nlms filter weights',
                                        data=filter.w,
                                        data_name='w_block_nlms_filter_final',
                                        length=length))
    
    rec_file.write(get_string_to_record(desc='Final block_nlms filter error',
                                        data=error,
                                        data_name='block_nlms_error',
                                        length=n_samples))

    # block rls
    filter=pra.adaptive.BlockRLS(length, lmbd=1.0, delta=2.0, L=8)
    error=np.zeros(n_samples)

    for i in range(n_samples):
        filter.update(x[i], d[i])
        error[i] = np.linalg.norm(filter.w - w)
    
    rec_file.write(get_string_to_record(desc='Final block_rls filter weights',
                                        data=filter.w,
                                        data_name='w_block_rls_filter_final',
                                        length=length))
    
    rec_file.write(get_string_to_record(desc='Final block_rls filter error',
                                        data=error,
                                        data_name='block_rls_error',
                                        length=n_samples))
    
