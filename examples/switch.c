#include<msp430g2553.h>

int main(void)
{
	// Configure the clock for 1 MHz.
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;
	
	P1DIR |= BIT0 + BIT6;	// Output LEDs
	P1DIR &= ~BIT3;			// use switch as input
	/* Since we dont have a pull up resistor, but only a pull down, we can
	 short the P1.3 and P1.4, making P1.4 high. This will connect upper terminal
	 of switch to +5 and lower to gnd through a pull down resistor.
	
		+5
		___  P1.4
		 |
		 |__ P1.3
	  S2\
		 |
		 /
		 \
		 / Pull down resistor
		 \
		 |
		_|_
		 _
		 .
	*/	 
		 
	
	P1DIR |= BIT4;			// Create upper voltage terminal
	P1OUT |= BIT4;
	P1REN = BIT3;			// Enable pull down.
	while(1)
	{
		if( P1IN & BIT3 )
			P1OUT = BIT0 + BIT4;
		else
			P1OUT = BIT6 + BIT4;
	}
	
	return 0;
}
