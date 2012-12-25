#!/bin/bash

from scipy import *
import scipy.io.wavfile as wave
from matplotlib.pyplot import *

w = wave.read('sample_heart_beat.wav')
data = w[1]
data = 64 * data[:,0] + data[:, 1]
clf()
plot(range(len(data)), data)

NUM_PEAKS = 10
indices = []
index_vals = []
for i in range(NUM_PEAKS):
    index = where(data == data.max())[0][0]
    indices.append(index)
    index_vals.append(data[index])
    data[index-100:index+100] = 0

plot(indices, index_vals, 'ro')
show()
