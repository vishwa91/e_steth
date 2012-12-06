/******************************************************************************
 *                        UART routines
 *
 * Description: This file contains the UART routines. These routines can be
 *              combined with display routines to print to the computer
 *              
 *				This file is a part of Electronic stethoscope project.
 *				You can find out more about the project from the README
 *			
 *
 * Author:  Vishwanath & Pavan
 * Repo:	github.com/vishwa91/e_steth
 * Date:    06-23-12
 ******************************************************************************/
 
#include <msp430g2553.h>
 
void initUART(void) {
	P1SEL = BIT1 + BIT2 ;				// P1.1 = RXD, P1.2=TXD
	P1SEL2 = BIT1 + BIT2 ;				// P1.1 = RXD, P1.2=TXD

	UCA0CTL1 |= UCSSEL_2;				// CLK = SMCLK
	UCA0BR0 = 0x6D;						// 1048576Hz/9600 = 109.23
	UCA0BR1 = 0x00;
	UCA0MCTL = UCBRS1 + UCBRS0;			// Modulation UCBRSx = 3
	UCA0CTL1 &= ~UCSWRST;				// **Initialize USCI state machine**
}

void send_byte(unsigned char byte )
{
	while (UCA0STAT & 0x01);			// Halt till any operations are in progress
	UCA0TXBUF = byte;					// TX -> RXed character
}

unsigned char receive_byte(void)
{
	while (UCA0STAT & 0x01);			// Halt till any operations are in progress
	return UCA0RXBUF;					// Return received character
}
