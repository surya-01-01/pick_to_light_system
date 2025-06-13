/*
 * File:   main.c
 * Author: surya
 *
 * Created on 8 May, 2025, 11:46 AM
 */


#include "p2lserver.h"

extern unsigned char can_payload[13];
unsigned char ch;
unsigned char ssd[4];
unsigned char digits[]={0XE7,0X21,0XCB,0X6B,0X2D,0X6E,0XEE,0x23,0XEF,0X6F};
unsigned char flag=0,i=0,j=0,start_pid=0,start_ust=0,message=1;
unsigned char pid[5],ust[5],pid2[5],ust2[5]; 

void init_config(void)
{
    init_can();
    init_uart();
    init_ssd_config();
    PEIE = 1;
    GIE = 1;
}

void main(void) {
    
    init_config();
    unsigned char key;
    
    puts("Press Enter to continue\n\r");
    while(1)
    {
        if(can_receive())
        {
            pid[0]=can_payload[D0 + 0]+'0';
            pid[1]=can_payload[D0 + 1]+'0';
            pid[2]=can_payload[D0 + 2]+'0';
            pid[3]=can_payload[D0 + 3]+'0';
            pid[4]='\0';
            
            ust[0]=can_payload[D0 + 4]+'0';
            ust[1]=can_payload[D0 + 5]+'0';
            ust[2]=can_payload[D0 + 6]+'0';
            ust[3]=can_payload[D0 + 7]+'0';
            ust[4]='\0';
           
          
            
            puts("Pid->\n\r");
            puts(pid);
            puts("\n\r");
            puts("Ust->\n\r");
            puts(ust);
            puts("\n\r");
            
        }
        
        if (ch != '\0')
        {
           if (!start_pid && ch == '\r' && !start_ust)
            {
                puts("\n\rStart typing PID (max 4 chars). Press Enter to finish:\n\r");
                start_pid = 1;
                i = 0;
            }
            else if (start_pid)
            {
                if (ch == '\r')
                {
                    pid2[i] = '\0';
                    puts("\n\rYou typed PID:\n\r");
                    puts(pid2);
                    start_pid = 0;
                    start_ust = 1;
                    message = 1;
                    j = 0;
                }
                else if (i < 4)
                {
                    pid2[i++] = ch;
                    putch(ch);
                }
            }
            else if (start_ust)
            {
                if (message)
                {
                    puts("\n\rStart typing UST (max 4 chars). Press Enter to finish:\n\r");
                    message = 0;
                }

                if (ch == '\r')
                {
                    ust2[j] = '\0';
                    puts("\n\rYou typed UST:\n\r");
                    puts(ust2);
                    start_ust = 0;
                    can_transmit();
                    delay(1000);
                    puts("\n\rTransmitted successfully \n\r");
                }
                else if (j < 4)
                {
                    ust2[j++] = ch;
                    putch(ch);
                }
            }

            ch = '\0';

        }
      
    }
        
}

