"""
Adaptive Filters Example
========================

In this example, we will run adaptive filters for system identification.
"""
from __future__ import division, print_function
from re import L
import time

import numpy as np 
from scipy.signal import fftconvolve
import math
import matplotlib.pyplot as plt
import pyroomacoustics as pra

# parameters
length = 16  # the unknown filter length
n_samples = 2048  # the number of samples to run
SNR = 15  # signal to noise ratio

# the unknown filter (unit norm)
w = []

w /= np.linalg.norm(w)

# create a known driving signal
x = []

# convolve with the unknown filter
#d_clean = fftconvolve(x, w)[:n_samples]

# add some noise to the reference signal
#d = d_clean + np.random.randn(n_samples) * 10 ** (-SNR / 20.0)

d = []

# create a bunch adaptive filters
#filter=pra.adaptive.RLS(length, lmbd=1.0, delta=2.0)
filter=pra.adaptive.NLMS(length, mu=0.5)
#filter=pra.adaptive.BlockLMS(length, mu=1.0 / 15.0 / 2.0, L=8, nlms=True)
#filter=pra.adaptive.BlockRLS(length, lmbd=1, delta=2.0, L=8)

error=np.zeros(n_samples)

start_time = time.time()

for i in range(n_samples):
    filter.update(x[i], d[i])
    error[i] = np.linalg.norm(filter.w - w)
    
print(f'Final error: {error[-1]}')
print(f'Finale filter weights: {filter.w}')
print(f'Execution time: {time.time() - start_time}')

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
