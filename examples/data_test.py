#!/bin/python

'''
    This module is used for testing data sent by the UART of msp430g2553. This
    will be used for the analysis of the data acquired by the ADC.
'''
from scipy import *
import scipy.signal as sg
import scipy.signal.filter_design as fd

from matplotlib.pyplot import *
import serial
data_dump = range(64)
data_buffer = range(64)
def flash_write(n):
    global data_dump
    global op2
    op2[n-64:n] = data_dump

def flash_read(k):
    global data_buffer
    global data
    data_buffer = data[k:k+64]
def conv(signal1):
    '''
        We need to try and mimic what is happening in microcontroller.
    '''
    global int_resp
    global data_dump
    global data_buffer
    
    for n in range(len(signal1)):
        if (n%64 == 0) and (n != 0):
            flash_write(n)
        else:
            temp = 0
            for k in range(n):
                if (k%64 == 0):
                    flash_read(k)
                else:
                    if((n-k)>= len(int_resp)):
                        temp += 0
                    else:
                        temp += data_buffer[k%64]*int_resp[n-k]
            data_dump[n%64] = temp;
                        
    
f = open('sample_vishwa.dat', 'r')
data = []
for i in f.readlines():
    data.append(int(i))
data_test = zeros(len(data))
data_test[0] = 1
order, Wn = fd.buttord(0.2, 0.3, 1, 10)
print order, Wn
b, a = fd.butter(order, Wn)
response = sg.lfilter(b, a, data_test)[:256]
int_resp = []
resp_file = open('impulse_response_integer.dat', 'r')
for i in resp_file.readlines():
    int_resp.append(int(i))
#op1 = conv(data[:1024], int_resp)
f.close()
op2 = range(1024)
conv(data)
op1 = convolve(int_resp, data)
clf()
plot(op1, 'r')
plot(op2)
show()
    
