"""
Adaptive Filters Example
========================

In this example, we will run adaptive filters for system identification.
"""
from __future__ import division, print_function

import time
import os
import numpy as np 
from scipy.signal import fftconvolve
import math
import matplotlib.pyplot as plt
import pyroomacoustics as pra
import pandas as pd
import seaborn as sns



# parameters
length = 16  # the unknown filter length
n_samples = 2048  # the number of samples to run
SNR = 15  # signal to noise ratio

# the unknown filter (unit norm)
w = np.random.randn(length)

w /= np.linalg.norm(w)

# create a known driving signal
x = np.random.randn(n_samples)


# convolve with the unknown filter
d_clean = fftconvolve(x, w)[:n_samples]

# add some noise to the reference signal
d = d_clean + np.random.randn(n_samples) * 10 ** (-SNR / 20.0)

batches = [1, 2, 4, 8, 32, 64, 128]#, 256, 512, 1024, 2049]
nlms_exec_times = []
nlms_error = []
rls_exec_times = []
rls_error = []

for b in batches:
    filter=pra.adaptive.BlockLMS(length, mu=0.001, L=b, nlms=True)
    error=np.zeros(n_samples)

    start_time = time.time()

    for i in range(n_samples):
        filter.update(x[i], d[i])
        error[i] = np.linalg.norm(filter.w - w)
    nlms_exec_times.append(time.time() - start_time)
    nlms_error.append(error[-1])

    filter=pra.adaptive.BlockRLS(length, lmbd=1.0, delta=2.0, L=b)
    error=np.zeros(n_samples)

    for i in range(n_samples):
        filter.update(x[i], d[i])
        error[i] = np.linalg.norm(filter.w - w)
    rls_exec_times.append(time.time() - start_time)
    rls_error.append(error[-1])

plots_folder = './graphs/block_form_analysis'
df = pd.DataFrame({'batch_size': batches, 
                    'exec_time': nlms_exec_times,
                    'nlms_error': nlms_error})
ax = sns.lineplot(data=df, x="batch_size", y="exec_time", palette='pastel')
ax.set_title(f'Batch size vs Execution time (block nlms)')
ax.set(xlabel='batch_size', ylabel='execution_time')

plt.tight_layout()
plt.savefig(os.path.join(plots_folder, 'block_nlms_execution_time_vs_batch_size(zoom).jpg'))
plt.clf()

ax = sns.lineplot(data=df, x="batch_size", y="nlms_error", palette='pastel')
ax.set_title(f'Batch size vs Error (block nlms)')
ax.set(xlabel='batch_size', ylabel='error')

plt.tight_layout()
plt.savefig(os.path.join(plots_folder, 'block_nlms_error_vs_batch_size(zoom).jpg'))
plt.clf()

df = pd.DataFrame({'batch_size': batches, 
                    'exec_time': rls_exec_times,
                    'rls_error': rls_error})
ax = sns.lineplot(data=df, x="batch_size", y="exec_time", palette='pastel')
ax.set_title(f'Batch size vs Execution time (block rls)')
ax.set(xlabel='batch_size', ylabel='execution_time')

plt.tight_layout()
plt.savefig(os.path.join(plots_folder, 'block_rls_execution_time_vs_batch_size(zoom).jpg'))
plt.clf()

ax = sns.lineplot(data=df, x="batch_size", y="rls_error", palette='pastel')
ax.set_title(f'Batch size vs Error (block rls)')
ax.set(xlabel='batch_size', ylabel='error')

plt.tight_layout()
plt.savefig(os.path.join(plots_folder, 'block_rls_error_vs_batch_size(zoom).jpg'))
plt.clf()

