/*
 * File:   newmain.c
 * Author: user
 *
 * Created on 22 Kas?m 2022 Sal?, 16:40
 */
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <pic18f45k22.h>
#include <htc.h>
#define _XTAL_FREQ 8000000
#pragma config FOSC = HSHP
#pragma config WDTEN = OFF
#define RA4 PORTAbits.RA4

unsigned char seg[11]={0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x40};
unsigned int dig_cnt=0;
unsigned int digits[4]={0,0,0,0};

unsigned int cnt_disp=0;
unsigned int cnt=1;

void main(void) {
    ANSELA=0;
    TRISA=0xF0;
    PORTA=0;

    ANSELD=0;
    TRISD=0x00;
    PORTD=0;

    T0CON=0xC3;
    
    TMR0IP=1;
    TMR0IE=1;
    GIE=1;

    while(1){
        if(cnt_disp>0)
        {
            digits[3]=cnt_disp/1000;
            digits[2]=(cnt_disp - (digits[3]*1000))/100;
            digits[1]=(cnt_disp - (digits[3]*1000)-(digits[2]*100))/10;
            digits[0]=cnt_disp%10;           
        }else
        {
            digits[3]=10;
            digits[2]=10;
            digits[1]=10;
            digits[0]=10;
        }
        cnt++;
        if (cnt >= 1000) {
            cnt = 1;
        }
        if(!RA4)
        {
            cnt_disp=cnt;
            while(!RA4);
        }                 
    }
    return;
}
void __interrupt(high_priority) isr(){
    if(TMR0IF){
        TMR0IF=0;
        TMR0L=6;

        PORTA=0;
        PORTD=seg[digits[dig_cnt]]+128*(dig_cnt==3);
        PORTA=(0x01)<<dig_cnt;

        dig_cnt++;
        if(dig_cnt==4){
            dig_cnt=0;
        }
    }
}