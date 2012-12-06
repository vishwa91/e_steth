/******************************************************************************
 *                        Hardware header file
 *
 * Description: This file is the main header file for all the hardware routines
 *				Include this file in main.c to use the routines.
 *
 *				The following hardware is supported:
 *				1. 7 segment display
 *				2. UART
 *				3. ADC				
 *
 * Author:  Vishwanath & Pavan
 * Repo:	github.com/vishwa91/e_steth
 * Date:    06-23-12
 ******************************************************************************/
 
// ADC prototypes
#include <adc.c>
void init_adc(void);
unsigned int get_sample(void);

// 7 segment display driver
//#include <7seg.c>

// UART prototypes
#include <uart.c>
void uart_init(void);
void send_byte(unsigned char);
unsigned char receive_byte(void);

// Display driver
#include <display.c>

void putchar(unsigned char);
void puts(char *);

static void xtoa(unsigned long, const unsigned long *);		// For converting Hex to ASCII
static void puth(unsigned);									// Print Hex values
void printf(char *, ...);									// printf routine
