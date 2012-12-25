#!/bin/python

'''
    This module is used for testing data sent by the UART of msp430g2553. This
    will be used for the analysis of the data acquired by the ADC.
'''
from scipy import *
import scipy.signal as sg
from matplotlib.pyplot import *
import serial

def conv(signal1, signal2):
    output = range(len(signal1))    # We hardly care about last values. Discard
    for n in range(len(output)):
        output[n] = 0
        for k in range(n):
            if (n-k)>= len(signal2):
                continue
            else:
                # Conventional standard algorithm. Nothing great.
                output[n] += signal1[k]*signal2[n-k]
    return output

port = serial.Serial()  # Create an instance of the serial port
for i in range(100):    # Figure out which port is open.
    port.port = i
    # Try setting different ports. Lock the port which works
    try:
        port.open()
        print 'Locking port',i
        break
    except:
        pass

count = 0
NCOUNT = 1024           # 1024 sample of data.
sample = 0
sample_array = []
clf()
while count < NCOUNT:
    get_sample = port.read()
    # Our data format is |xxx|xxx and so on.
    # We need to convert this into integer data.
    if get_sample == '|':
        sample_array.append(sample)
        sample = 0
        count += 1
        continue
    else:
        sample = 10*sample + int(get_sample)
port.close()            # Dont forget to close the port. Else, it won't be
                        # detected from next time.
# Low pass filter file.
resp_file = open('impulse_response_integer.dat', 'r')
int_resp = []
for i in resp_file.readlines():
    int_resp.append(int(i))
# convolve twice to make a good signal.
f = open('sample.dat', 'w')
for i in sample_array:
    f.write(str(i)+'\n')
f.close()
op1 = conv(sample_array, int_resp)
op2 = conv(op1, int_resp)
plot(op1)
show()
