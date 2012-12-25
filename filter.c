/******************************************************************************
 *                        Low pas filter file
 *
 * Description: This file contains the data for the low pass filter we will use
 *              for convolving with our input signal. There are 161 samples of 
 *              the low pass filter.
 *              
 *		This file is a part of Electronic stethoscope project.
 *		You can find out more about the project from the README
 *			
 *
 * Author:      Vishwanath & Pavan
 * Repo:	github.com/vishwa91/e_steth
 * Date:        06-23-12
 ******************************************************************************/
/*
* For the sake of readability, the whole of 161 integers is divided into rows of
* 10 numbers each.
*
* The filter design has been done experimentally in python.
* buttord and butter from scipy.signal.filter_design has be used
* to get the transfer function of the low pass filter.
* Then, a delta(array of 512 numbers with all zeros and the first samle 1) is 
* given as input to scipy.signal.lfilter to get the time domain response. All
* samples after 161 were zeros and hence discarded.
*
* The parameters used for the filter design are:
* Wp: Pass band frequency(Normalized to 0-1): 0.2
* Ws: Stop band frequency: 0.3
* gpass: pass band maximum loss: 1dB
* gstop: stop band minimum attenuation: 10dB
*
* This returned an order of 4 and natural frequency Wn = 0.235
*/
const int filter_data[161] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 1, 1, 1, 2, 2, 3, 4, 4,
                        5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
                        15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
                        24, 25, 25, 26, 26, 27, 27, 27, 27, 27,
                        27, 27, 27, 26, 26, 26, 25, 24, 24, 23,
                        22, 22, 21, 20, 19, 18, 17, 16, 15, 14,
                        13, 11, 10, 9, 8, 7, 6, 5, 4, 3,
                        2, 1, 1, 0, 0, -1, -1, -2, -3, -3,
                        -4, -4, -4, -5, -5, -5, -5, -6, -6, -6,
                        -6, -6, -6, -6, -6, -6, -5, -5, -5, -5,
                        -4, -4, -4, -4, -3, -3, -3, -2, -2, -2,
                        -1, -1, -1, -1, 0, 0, 0, 0, 0, 0,
                        0, 0, 1, 1, 1, 1, 1, 1, 1, 2,
                        2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                        2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};