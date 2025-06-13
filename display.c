/*
 * File:   display.c
 * Author: surya
 *
 * Created on 6 May, 2025, 11:08 AM
 */


#include "pick2light.h"
extern unsigned char ssd[4];
extern int flag,i,ust_val;
unsigned char digits[]={0XE7,0X21,0XCB,0X6B,0X2D,0X6E,0XEE,0x23,0XEF,0X6F};

unsigned char trans_buffer[8];
unsigned int a=0,b=0,c=0,d=0,first_flag=1;
signed int dp=3;

void display_id()
{
    if(!flag)
    {
        ssd[0]=0xE5;
        ssd[1]=0x40;
        ssd[2]=0x6E;
        ssd[3]=0xCC;
    }
    else
    {
        ssd[0]=0x8F;
        ssd[1]=0x40;
        ssd[2]=0x84;
        ssd[3]=0xE9;
    }
    display(ssd);
}

void set_id()
{
    unsigned char key;
    key = read_digital_keypad(STATE_CHANGE);
    if(first_flag)
    {
        if(!flag)
        {
            d= read_internal_eeprom(0x04);
            c= read_internal_eeprom(0x05);
            b= read_internal_eeprom(0x06);
            a= read_internal_eeprom(0x07);
        }
        else
        {
            d= read_internal_eeprom(0x00);
            c= read_internal_eeprom(0x01);
            b= read_internal_eeprom(0x02);
            a= read_internal_eeprom(0x03);
        }
        first_flag=0;
    }
    
    if(key==SWITCH1)
    {
        switch(dp)
        {
                case 0:
                    d++;
                    d=d%10;
                    break;
                case 1:
                    c++;
                    c=c%10;
                    break;
                case 2:
                    b++;
                    b=b%10;
                    break;
                case 3: 
                    a++;
                    a=a%10;
                    break;
        }
    }
    else if(key==SWITCH2)
    {
        dp--;
        if(dp<0)
            dp=3;
    }
    else if(key==SWITCH3)
    {
         if(!flag)
        {
             write_internal_eeprom(0x04, d);
             write_internal_eeprom(0x05, c);
             write_internal_eeprom(0x06, b);
             write_internal_eeprom(0x07, a);
        }
        else
        {
             write_internal_eeprom(0x00, d);
             write_internal_eeprom(0x01, c);
             write_internal_eeprom(0x02, b);
             write_internal_eeprom(0x03, a);
        }
        ready_for_transmit();
        can_transmit();
		delay(1000);
        i=0;
    }
    
        ssd[3]=digits[a];
        ssd[2]=digits[b];
        ssd[1]=digits[c];
        ssd[0]=digits[d];
        ssd[dp]=ssd[dp]|0x10;
        display(ssd);
}

/* delay 1ms function */
void delay(unsigned short factor)
{
	unsigned short i, j;

	for (i = 0; i < factor; i++)
	{
		for (j = 500; j--; );
	}
}

void ready_for_transmit(void)
{
    for(int i=0;i<8;i++)
        trans_buffer[i]=read_internal_eeprom((char)i);
}

void receive_display(void)
{
    unsigned char key;
    key = read_digital_keypad(STATE_CHANGE);
    if(key==SWITCH1)
    {
        ust_val++;
    }
    else if(key==SWITCH2)
    {
        ust_val--;
    }
    else if(key==SWITCH3)
    {
             write_internal_eeprom(0x04, ust_val/1000);
             write_internal_eeprom(0x05, (ust_val/100)%10);
             write_internal_eeprom(0x06, (ust_val/10)%10);
             write_internal_eeprom(0x07, ust_val%10);
             i=0;
    }
        ssd[0]=digits[ust_val/1000];//showing the fourth digit
        ssd[1]=digits[(ust_val/100)%10];//showing the third digit
        ssd[2]=digits[(ust_val/10)%10];//showing the second digit
        ssd[3]=digits[ust_val%10];
        display(ssd);
}