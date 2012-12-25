/******************************************************************************
 *                        UART routines
 *
 * Description: This file contains the UART routines. These routines can be
 *              combined with display routines to print to the computer
 *              
 *		This file is a part of Electronic stethoscope project.
 *		You can find out more about the project from the README
 *			
 *
 * Author:      Vishwanath & Pavan
 * Repo:	github.com/vishwa91/e_steth
 * Date:        06-23-12
 ******************************************************************************/
 
#include <msp430g2553.h>
 
void init_uart(void) {
	P1SEL = BIT1 + BIT2 ;				// P1.1 = RXD, P1.2=TXD
	P1SEL2 = BIT1 + BIT2 ;				// P1.1 = RXD, P1.2=TXD

	UCA0CTL1 |= UCSSEL_2;				// CLK = SMCLK
	UCA0BR0 = 0x6D;					// 1048576Hz/9600 = 109.23
	UCA0BR1 = 0x00;
	UCA0MCTL = UCBRS0;				// Modulation UCBRSx = 1
	UCA0CTL1 &= ~UCSWRST;				// **Initialize USCI state machine**
        IE2 |= UCA0RXIE;                                // Enable USCI_A0 RX interrupt
}

void send_byte(unsigned char byte)
{
   while (!(IFG2 & UCA0TXIFG));         // Halt till transmission is busy
        UCA0TXBUF = byte;               // Now throw the byte into the buffer
}
unsigned char receive_byte(void)
{
	while (UCA0STAT & 0x01);        // Halt till any operations are in progress
	return UCA0RXBUF;		// Return received character
}
void log(char *line)
{
  while(*line)
    send_byte(*(line++));
}

// log_num is used for logging numbers.
void log_num(int num)
{
  if(num < 0)
  {
    num *= -1;
    send_byte('-');
  }
    send_byte('0' + num/10000);
    send_byte('0' + (num/1000)%10);
    send_byte('0' + (num/100)%10);
    send_byte('0' + (num/10)%10);
    send_byte('0' + num%10);
    send_byte('|');
}

int putchar(int byte)
{
  send_byte((char)byte);
  return 0;
}

int puts(const char *line)
{
  while(*line)
    send_byte(*(line++));
  return 0;
}
