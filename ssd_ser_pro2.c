/*
 * File:   ssd_ser_pro2.c
 * Author: surya
 *
 * Created on 8 May, 2025, 12:48 PM
 */


#include "p2lserver.h"

void init_ssd_config(void)
{
    TRISD=0X00;
    TRISA=TRISA & 0XF0;
    PORTA=PORTA & 0XF0;
}
void display(unsigned char *ssd)
{
    for(int i=0;i<4;i++)
    {
        PORTD=ssd[i];
        PORTA=(PORTA & 0xF0)|(1<<i);
        for(unsigned int wait=500;wait--;);
    }
}
