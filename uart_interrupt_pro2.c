/*
 * File:   uart_interrupt_pro2.c
 * Author: surya
 *
 * Created on 8 May, 2025, 12:10 PM
 */


#include "p2lserver.h"

extern unsigned char ch;

void init_uart(void)
{
	/* Serial initialization */
	RX_PIN = 1;
	TX_PIN = 0;

	
	BRGH = 1;
	SPEN = 1;
	
	/* TX/RC7 and RX/RC6 act as serial port */
	/* Continous reception enable or disable */ 
	CREN = 1;
	
	

	/* Baud Rate Setting Register */
	/* Set to 10 for 115200, 64 for 19200 and 129 for 9600 */
	SPBRG = 129;
/* Receive Interruot Enable Bit (Enabling the serial port Interrupt) */
    RCIE = 1;


}

void putch(unsigned char byte) 
{
	/* Output one byte */
	/* Set when register is empty */
	while(!TXIF)
	{
		continue;
	}
	TXREG = byte;
}

int puts(const char *s)
{
	while(*s)		
	{
		putch(*s++);	
	}

	return 0;
}

unsigned char getch(void)
{
	/* Retrieve one byte */
	/* Set when register is not empty */
	while(!RCIF)
	{
		continue;
	}

	return RCREG;
}

unsigned char getche(void)
{
	unsigned char c;

	putch(c = getch());

	return (c);
}

void __interrupt() isr(void)
{
    if (RCIF == 1)
    {
        if (OERR == 1)
            OERR = 0;
        
        ch = RCREG;
        
        RCIF = 0;
    }
}
