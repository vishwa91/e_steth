#include <msp430g2553.h>

void ADCinit(void)
{
	ADC10CTL1 |= INCH_5;		// Select the A5 channel
	ADC10CTL1 |= ADC10DIV_0;	// No division for ADC clock
	
	ADC10CTL0 |= SREF_0;		// Vcc, Vss as references
	ADC10CTL0 |= ADC10SHT_3;	// 16 clocks for sample and hold
	ADC10CTL0 |= ADC10ON;		// Switch on the ADC	
	
	ADC10AE0 |= BIT5;			// P1.5 is input
}

void CPUinit(void)
{
	WDTCTL = WDTPW + WDTHOLD;	// Power down watchdog timer
	BCSCTL1 = CALBC1_1MHZ;      // Calibrate system to 1MHz
  	DCOCTL = CALDCO_1MHZ;
  	BCSCTL2 &= ~(DIVS_3);		// SMCLK = MCLK
}

unsigned int get_sample(void)
{
	// Start conversion
	ADC10CTL0 |= ADC10SC + ENC;
	while(ADC10CTL1 & ADC10BUSY);
	return ADC10MEM;
}

int main(void)
{
	CPUinit();
	ADCinit();
	return 0;
}
