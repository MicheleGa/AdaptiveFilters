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

def get_string_to_record(desc, data, data_name, length) -> str:
    string = f'{desc}:\n'
    string += f'static float {data_name}[' + str(length) + '] = {'
    for el in range(len(data) - 1):
        string += str(round(data[el], 4)) + ', '
    string += str(round(data[length - 1], 4)) + '}\n'

    return string

# recording file
with open('results_nlms.txt', 'w') as rec_file:
    rec_file.write('')

with open('results_nlms.txt', 'a') as rec_file:
    rec_file.write('**** Adaptive filter data ****\n')

    # parameters
    length = 15  # the unknown filter length
    n_samples = 256  # the number of samples to run
    SNR = 15  # signal to noise ratio

    # the unknown filter (unit norm)
    w = np.random.randn(length)

    rec_file.write(get_string_to_record(desc='Unknown filter',
                                        data=w,
                                        data_name='w',
                                        length=length))

    w /= np.linalg.norm(w)

    rec_file.write(get_string_to_record(desc='Normalized w',
                                        data=w,
                                        data_name='w_norm',
                                        length=length))

    # create a known driving signal
    x = np.random.randn(n_samples)

    rec_file.write(get_string_to_record(desc='Input driving signal',
                                        data=x,
                                        data_name='x',
                                        length=n_samples))

    # convolve with the unknown filter
    d_clean = fftconvolve(x, w)[:n_samples]

    rec_file.write(get_string_to_record(desc='Convolve with unknown filter',
                                        data=d_clean,
                                        data_name='d_clean',
                                        length=n_samples))

    # add some noise to the reference signal
    d = d_clean + np.random.randn(n_samples) * 10 ** (-SNR / 20.0)

    rec_file.write(get_string_to_record(desc='Convolve with unknown filter and add some noise',
                                        data=d,
                                        data_name='d',
                                        length=n_samples))

    # create a bunch adaptive filters
    # filter=pra.adaptive.RLS(length, lmbd=1.0, delta=2.0)
    filter=pra.adaptive.NLMS(length, mu=0.5)
    error=np.zeros(n_samples)

    for i in range(n_samples):
        filter.update(x[i], d[i])
        error[i] = np.linalg.norm(filter.w - w)

    rec_file.write(get_string_to_record(desc='Final weights weights',
                                        data=w,
                                        data_name='w_final',
                                        length=length))
    
    rec_file.write(get_string_to_record(desc='Final filter weights',
                                        data=filter.w,
                                        data_name='w_filter_final',
                                        length=length))
    
    rec_file.write(get_string_to_record(desc='Final filter error',
                                        data=error,
                                        data_name='error',
                                        length=n_samples))

    """ plt.plot(w)
    for algo in adfilt.values():
        plt.plot(algo["filter"].w)
    plt.title("Original and reconstructed filters")
    plt.legend(["groundtruth"] + list(adfilt))

    plt.figure()
    for algo in adfilt.values():
        plt.semilogy(algo["error"])
    plt.legend(adfilt)
    plt.title("Convergence to unknown filter")
    plt.show() """
