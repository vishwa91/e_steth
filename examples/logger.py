#!/bin/python

'''
    This module is used for testing data sent by the UART of msp430g2553. This
    will be used for the analysis of the data acquired by the ADC.
'''
from scipy import *
import scipy.signal as sg
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
            data_dump[n%64] = temp/120;

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
f = open('data_log.txt', 'w')
while(port.read() != '#'):
    pass
print 'Starting data aquisition.'
while(1):
    t = port.read()
    if t== '@':
        print 'Done.'
        break
    elif t == '^':
        print 'Done data aquisition.'
    else:
        f.write(t)
        
f.close()
port.close()
'''
int_resp = []
resp_file = open('impulse_response_integer.dat', 'r')
for i in resp_file.readlines():
    int_resp.append(int(i))
f = open('data_log.txt', 'r')
data = []
output = []
read = []
verify = []
locs = []
for line in f.readlines():
    if ('Data' in line):
        i = line.index('|')
        data.append(int(line[i-5:i]))
    elif ('read' in line):
        i = line.index('|')
        read.append(int(line[i-5:i]))
    elif ('time' in line):
        i = line.index('|')
        time_period = int(line[i-5:i])
    elif ('loc' in line):
        i = line.index('|')
        locs.append(int(line[i-5:i]))
    elif('Output' in line):
        i = line.index('|')
        if line[i-6] == '-':
            output.append(-1*int(line[i-5:i]))
        else:
            output.append(int(line[i-5:i]))
    elif('verify' in line):
        i = line.index('|')
        if line[i-6] == '-':
            verify.append(-1*int(line[i-5:i]))
        else:
            verify.append(int(line[i-5:i]))
clf()
data = read
op2 = zeros_like(data)
conv(op2)
plot(verify, 'b')
#plot(output, 'b')
plot(data, 'r')
#plot(op2, 'g')
#plot(read, 'y')
locs = array(locs)
verify = array(verify)
plot(locs, verify[locs], 'rx')
show()
avg_duration = 0
for i in range(len(locs)-1):
    t = (locs[i+1] - locs[i]) * time_period * 8 * 10e-6
    print t
    avg_duration = ((i)*avg_duration + t)/(i+1.0)

print 60.0/avg_duration
'''
