/******************************************************************************
 *                        Signal processing file
 *
 * Description: This file contains routines for signal processing of the input
 *              signal. This includes convolution and peak detection.
 *              
 *		This file is a part of Electronic stethoscope project.
 *		You can find out more about the project from the README
 *			
 *
 * Author:      Vishwanath & Pavan
 * Repo:        github.com/vishwa91/e_steth
 * Date:        06-23-12
 ******************************************************************************/

#include <stdlib.h>
#define NDATA 1024
#define NFILTER 161
#define MIN_PEAK_DIST 60

int min(int a, int b){
  return (a<b?a:b);
}

int max(int a, int b){
  return (a>b?a:b);
}

int max_element_in_range(int *d, int l_idx, int r_idx, int cur_element){
    int temp = 0;
    int i;
    int cur_value = d[cur_element];
    d[cur_element] = -1;
    
    // Finding max element
    for (i = l_idx; i < r_idx; i++)
        if (temp < d[i])
            temp = d[i];
    
    d[cur_element] = cur_value;
    
    return temp;
}

// This routine convolves two signals using the 'standard' method. There is no
// optimisation involved in this.
void convolve(int *output, int *signal1, int siglen1)
{
  int n, k;  
  int temp;
  int *temp_addr;
  temp_addr = signal1;
  // Discrete convolution: y[n] = sum(x[k]*h[n-k]) k: -inf, +inf
  // However, x[k] goes to zero when k<0 and h[n-k] goes to zero when n-k < 0
  // Hence, k will only vary from 0 to n
  
  temp_addr = signal1;          // Just to ensure that we don't tamper main
                                // address, make a copy of it.
  for(n = 0; n < siglen1; n++)
  {
    temp = 0;                   // Temporary storage variable.
    // If 64 outputs have been calculated, write it to flash first.
    if((n%64 == 0) && (n !=0 ))
    {
      flash_write(output + n-64, WRITE_TO_DUMP);
    }
     // Else, continue calculating.
    else
    {
      for(k = max(0, n-NFILTER); k < n; k++)
      { 
        if((n-k) >= NFILTER)
          continue;
        else
          temp+= (filter_data[n-k]*temp_addr[k])/100;
      }
      // While dumping, we need to change index to a number between 0-64.
      data_dump[n%64] = temp;
    }    
  }
}

int detect_peaks(int *d_P){
    int cur_idx = 0;                // Current id in the while loop    
    int data_len = NDATA;
    int m = 0;                      // Number of peaks in the data
    int end_idx = data_len - 1;     // Index of the last element
    int pd = MIN_PEAK_DIST;         // The minimum distance between the peaks
                                    // required
    
    int prev_peak_bound_r = 0;      // Boundary of MIN_PEAK_DIST to the right of
                                    // previous peak    
    int end_l = 0, end_r = 0;    
    int max_element = 0;            // Maximum element excluding the current
                                    // element in the given range of the data     
    while (cur_idx < data_len) {
        if (m > 0)
            if (cur_idx < prev_peak_bound_r)
                cur_idx = prev_peak_bound_r;        
    
        // Left Boundary for the current element
        end_l = max(cur_idx - pd, 0);
        
        // Right Boundary for the current element
        end_r = min(cur_idx + pd, end_idx+1);
        
        max_element = max_element_in_range(d_P, end_l, end_r, cur_idx-end_l);
        
        if (d_P[cur_idx] >= max_element){
            m++;
            data_dump[m-1] = cur_idx;
            prev_peak_bound_r = min(cur_idx + pd, end_idx);
        }        
        cur_idx++;
    }    
    return m;
}