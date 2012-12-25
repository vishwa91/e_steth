/*
 *****************************************************************************
 *                        ADC routines
 *
 * Description: This file contains the ADC specific routines. The ADC
 *              is configured to take input from channel 5
 *              
 *		This file is a part of Electronic stethoscope project.
 *		You can find out more about the project from the README
 *			
 *
 * Author:  Vishwanath & Pavan
 * Repo:	github.com/vishwa91/e_steth
 * Date:    06-23-12
 *****************************************************************************
   */
 
#include <msp430g2553.h>

void init_adc(void)
{
	ADC10CTL0 &= 0xfffd;			// Disable the ADC for configuring
	ADC10CTL1 |= INCH_5;			// Select the A5 channel
	ADC10CTL1 |= ADC10DIV_3;		// No division for ADC clock
	
	ADC10CTL0 |= SREF_0;			// Vcc, Vss as references
	ADC10CTL0 |= ADC10ON;			// Switch on the ADC	
	ADC10AE0 |= BIT5;			// P1.5 is input
	__delay_cycles(100000);			// Wait for some time to let ADC ref settle down
}

unsigned int get_sample(void)
{
	// Start conversion
	ADC10CTL0 |= ADC10SC + ENC;
	while(ADC10CTL1 & 0x01);		// Halt till ADC conversion is busy.
	return ADC10MEM;			// Return the ADC value
}
