/*
 * File:   main.c
 * Author: surya
 *
 * Created on 6 May, 2025, 10:37 AM
 */



#include "pick2light.h"
#include<string.h>
extern unsigned int first_flag;
unsigned char pid[5],ust[5],original_pid[5];
int i=0,flag=0,ust_val;
unsigned char ssd[4];
void init_config()
{
    init_ssd_config();
    init_external_interrupt(); 
    init_digital_keypad();
     init_can();
}

void __interrupt() isr()
{
    if(INT0IF)
    {
        i=!i;
        INT0IF=0;
        first_flag=1;
    }
}
void main(void) {
    init_config();
    
    
    unsigned char key;
    while(1)
    {
        switch(i)
        {
            case 1:  
                key = read_digital_keypad(STATE_CHANGE);
                if(key==SWITCH3)
                {
                    flag=!flag;
                }
                else if(key==SWITCH2)
                {
                    i=2;
                }
                display_id();
                break;
            case 0:
                ssd[0]=0x00;
                ssd[1]=0x00;
                ssd[2]=0x00;
                ssd[3]=0x00;
                display(ssd);
                break;
            case 2:
                 set_id();
                break;
            case 3:
                receive_display();
            
        }
            
        if(can_receive())
        {
            pid[0]=can_payload[D0 + 0]-'0';
            pid[1]=can_payload[D0 + 1]-'0';
            pid[2]=can_payload[D0 + 2]-'0';
            pid[3]=can_payload[D0 + 3]-'0';
            pid[4]='\0';
            
            ust[3]=can_payload[D0 + 4]-'0';
            ust[2]=can_payload[D0 + 5]-'0';
            ust[1]=can_payload[D0 + 6]-'0';
            ust[0]=can_payload[D0 + 7]-'0';
            ust[4]='\0';
            
            original_pid[0]= read_internal_eeprom(0x00);
            original_pid[1]= read_internal_eeprom(0x01);
            original_pid[2]= read_internal_eeprom(0x02);
            original_pid[3]= read_internal_eeprom(0x03);
            original_pid[4]='\0';
            if(strcmp(original_pid,pid)==0)
            {
                ust_val=((int)ust[3]*1000)+((int)ust[2]*100)+((int)ust[1]*10)+(int)ust[0];
                i=3;
            }
        }
         
        
    }
    
    return;
}


void init_external_interrupt()
{
    GIE=1;//global interrupt enable
    PEIE=1;//peripheral interrupt enable
    INT0IE=1;
    INT0IF=0;//External interrupt flag
    INTEDG0=1;//raising edge interrupt
}
