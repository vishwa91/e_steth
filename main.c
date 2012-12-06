#include <msp430g2553.h>
#define UART_DISPLAY
#include <hardware.h>

void CPUinit(void)
{
	WDTCTL = WDTPW + WDTHOLD;	// Power down watchdog timer
	BCSCTL1 = CALBC1_1MHZ;      // Calibrate system to 1MHz
  	DCOCTL = CALDCO_1MHZ;
  	BCSCTL2 &= ~(DIVS_3);		// SMCLK = MCLK
}

void pwm_init(void)
{
	TACCTL1	= OUTMOD_7;           	// TACCR1 reset/set
	TACTL	= TASSEL_2 + MC_1;    	// SMCLK, upmode
	TACCR0	= 0x02ff;        		// PWM Period
	TACCR1	= 0x0000;            	// TACCR1 PWM Duty Cycle
	P1DIR	|= BIT2;               	// P1.2 = output
	P1SEL	|= BIT2;               	// P1.2 = TA1 output
}

int main(void)
{
	CPUinit();
	P1DIR = 0xef;			// All are outputs except P1.5, which is ADC input
	P2DIR = 0xff;
	init_adc();
	int sample = 0;
	P1OUT &= ~BIT0;
	while(1)
	{
		sample = get_sample();					// Get our ADC sample
		P2OUT = ((sample & 0x03f0) >> 0x04);	// Display the ADC value on P1.1-P1.4 and P2.0-P2.5
		P1OUT = ((sample & 0x000f) << 0x01);
		printf("%d", sample);
	}
	/*while(1)
	{
		for(temp = 0; temp < 10; temp++)
		{
			upper_nib = (0x01 << temp);
			P2OUT = ((upper_nib & 0x03f0) >> 0x04);
			P1OUT = ((upper_nib & 0x000f) << 0x01); 			
			__delay_cycles(100000);
		}
	}*/
		
	return 0;
}
