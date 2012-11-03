#include<msp430g2553.h>

void mydelay(int delay_mul)
{
	unsigned int i = 0, j = 0, k = 0;
	for(j = 0; j < delay_mul; j++)
		for(i = 0; i < 0xffff; i++)
			for(k = 0; k < 0xffff; k++);
}

void main()
{
	// Configure the clock for 1 MHz.
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;
	
	P1DIR |= BIT0 + BIT6;	// Output LEDs
	P1DIR &= ~BIT3;			// Switch input
	while(1)
	{
		if(P1IN&BIT3)
		{
			P1OUT = BIT0;
			__delay_cycles(1000);
		}
		else
		{
			P1OUT = BIT6;		
			__delay_cycles(1000);
		}
	}
}
