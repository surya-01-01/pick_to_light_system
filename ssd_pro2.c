/*
 * File:   ssd_pro2.c
 * Author: surya
 *
 * Created on 6 May, 2025, 10:31 AM
 */


#include "pick2light.h"

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
