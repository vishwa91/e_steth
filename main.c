/******************************************************************************
 *                        Main
 *
 * Description: This file is a collaboration of all modules. 
 *              
 *		This file is a part of Electronic stethoscope project.
 *		You can find out more about the project from the README
 *			
 *
 * Author:      Vishwanath & Pavan
 * Repo:	github.com/vishwa91/e_steth
 * Date:        06-23-12
 ******************************************************************************/

// Standard declarations.
#include <msp430g2553.h>
#include <stdio.h>

// Data buffers.
int data_buffer[64];                    // Use this for holding 64 samples.
int data_dump[64];                      // Use this for dumping data read.

// Local includes.
#include "uart.c"
//#include "display.c"
#include "flash.c"
#include "adc.c"
#include "filter.c"
#include "signal.c"

// CPU initialisation routine. The CPU is calibrated to work at 1 MHz.
void init_cpu(void)
{    
  WDTCTL = WDTPW + WDTHOLD;	        // Power down watchdog timer
  BCSCTL1 = CALBC1_1MHZ;                // Calibrate system to 1MHz
  DCOCTL = CALDCO_1MHZ;
  BCSCTL2 &= ~(DIVS_3);		        // SMCLK = MCLK  
  FCTL2  = FWKEY + FSSEL_1 + FN1;
}
int main(void)
{
  // Initialise peripherals first
  P1DIR = BIT0 + BIT6;                  // Just for indication purpose.
  P1OUT = BIT6;
  init_cpu();                           // Calibrate CPU to 1 MHz
  init_adc();                           // ADC for sampling
  init_uart();                          // UART for logging service
  
  // Create the sample data array. This will hold 1024 samples.
  log("Starting system.\n");
  
  // Flash memory addresses.
  int *data = (int *)0x0E000;           // Address of data storage in flash
  int *output = (int *)0x0E800;         // Address of output in flash
  
  // Variable declarations.
  int *data_incr;
  data_incr = data;                     // To ensure we keep original address
                                        // intact.
  unsigned int i, j, time_period, peaks;
  long duration, avg_duration;
  
  // Initialise Timer_A. We will use it to find the time duration between two
  // samples.
  TACCTL1 = OUTMOD_7;                   // TACCR1 reset/set
  TACTL	= TASSEL_2 + MC_0 + ID_3;       // SMCLK, upmode
  P1OUT = BIT0;
  while(1)
  {
    // Erase flash segments first.
    for(i = 0; i < NDATA/256; i++)
    {
      flash_erase(data + i*256);
      flash_erase(output + i*256);
    }
    P1OUT = BIT0;
    
    // Next line is for debugging.
    // send_byte('#');
    
    for(i = 0; i < NDATA/64; i++)
    {
      data_incr = data + 64*i;
      for(j = 0; j < 64; j++)
      {
        TACTL = TACLR;                      // Clear the timer.
        TACTL = TASSEL_2 + MC_2 + ID_3;     // Start timer.
        
        data_buffer[j] = get_sample();
        __delay_cycles(200);
        time_period = TAR;                // The TAR value is our duration.
      }
      flash_write(data_incr, WRITE_TO_BUFFER);
    }
    log_num(time_period);
    // Next line is for debugging.
    // send_byte('^');
    
    // Apply the low pass filter and detect the peaks.
    P1OUT = BIT6;
    convolve(output, data, NDATA);
    
    peaks = detect_peaks(output);
    
    // Next line is for debugging.
    // send_byte('@');
    avg_duration = 0;                 // To ensure outliers don't distort
                                        // results, average out the data.
    for(i = 0; i < peaks-1; i++)
    {
      send_byte('\n');
      log("locs:");
      log_num(data_dump[i]);
      duration = (data_dump[i+1]-data_dump[i]);
      avg_duration += duration;
    }
    duration = avg_duration * time_period * 8 / 1000;
    duration = 60000 / duration;
    log_num((int)duration);
    // Replace the previous line with the next line.
    // print((int)duration);
  }
}