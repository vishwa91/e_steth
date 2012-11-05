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
	while(ADC10CTL1 & ADC10BUSY);	// Halt till ADC conversion is busy.
	return ADC10MEM;				// Return the ADC value
}

void pwm_init(void)
{
	TACTL = TASSEL_2 + ID_0 + MC_1;		// SMCLK, no divide, up mode.
	TACCR0 = 0x00ff;					// Counter top value
	TACCR1 = 0x003f;					// Period of count. 0 initially
	TACCTL1 = OUTMOD_7;					// TACCR1 set/reset
	P1DIR |= BIT2;						// P1.2 is output
	P1SEL |= BIT2;
}

int main(void)
{
	CPUinit();
	ADCinit();
	pwm_init();
	int sample = 0;
	while(1)
	{
		sample = get_sample();
		TACCR1 = sample;
	}
	return 0;
}
