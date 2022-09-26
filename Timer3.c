
#include <xc.h>
#include <sys/attribs.h>
#include "config.h"



int Mode = 0;
int Seconds = 0;
int Tenth = 0;
int counter = 0;
#define TMR_TIME  8000


void __ISR(_TIMER_3_VECTOR, ipl7) Timer3ISR(void) 
{  
       if(Mode == 1){
        if(Tenth == 9){
            Tenth = 0;
            Seconds++;
        }
    
            Tenth++;
      
    }else{
        
    }      
    IFS0bits.T3IF = 0;       // clear interrupt flag
      

}


void Timer3_Setup()
{
  PR3 = (int)(((float)(TMR_TIME * PB_FRQ) / 256) ); 
    
  TMR3 = 0;                           //    initialize count to 0
  T3CONbits.TCKPS = 0;               
  T3CONbits.TGATE = 0;                //    not gated input (the default)
  T3CONbits.TCS = 0;                  //    PCBLK input (the default)
  T3CONbits.ON = 1;                   //    turn on Timer1
  IPC3bits.T3IP = 7;                  //    priority
  IPC3bits.T3IS = 3;                  //    subpriority
  IFS0bits.T3IF = 0;                  //    clear interrupt flag
  IEC0bits.T3IE = 1;                  //    enable interrupt
  macro_enable_interrupts();          //    enable interrupts at CPU
}


void Timer3_init()
{
    Timer3_Setup();  
}


//NAME: SecondsTime
//PARAMETERS: None
//RETURN: int
//DESCRIPTION: Returns the current seconds
unsigned int  SecondsTime()
{
    
  T3CONbits.ON = 0;
  T1CONbits.ON = 0;
  unsigned int d2 = Seconds;
  T3CONbits.ON = 1; 
  T1CONbits.ON = 1;
 
  return d2;
}

//NAME: TenthTime
//PARAMETERS: None
//RETURN: int
//DESCRIPTION: Returns the tenth seconds
unsigned int  TenthTime()
{
     
  T3CONbits.ON = 0;
  T1CONbits.ON = 0;
  unsigned int d1 = Tenth;
  T3CONbits.ON = 1; 
  T1CONbits.ON = 1;
 
  return d1;
    
}

void Increment(){ //input two numbers subtracted from 5 (0.0 - 5.0)
     Mode = 1; //Starts increment Mode  
     Tenth = 0;
    Seconds = 0;
}

void Timer3_Close()
{
    T3CONbits.ON = 0;// turn off Timer1
    Tenth = 0;
    Seconds = 0;
    
}

const unsigned char digitSegments[]= {
    0b1000000, // 0
    0b1111001, // 1
    0b0100100, // 2
    0b0110000, // 3
    0b0011001, // 4
    0b0010010, // 5
    0b0000010, // 6
    0b1111000, // 7
    0b0000000, // 8
    0b0010000, // 9
    0b0001000, // A
    0b0000011, // b
    0b1000110, // C
    0b0100001, // d
    0b0000110, // E
    0b0001110,  // F
    0b0001001   // H
};

#define NO_DIGITS sizeof(digitSegments)/sizeof(digitSegments[0])
unsigned char digits[4];
#define TMR_TIME2    0.003 // 3000 us for each tick


void __ISR(_TIMER_1_VECTOR, ipl7) Timer1ISR(void) 
{  
    static unsigned char idxCurrDigit = 0;
    unsigned char currDigit, idx;

    idx = (idxCurrDigit++) & 3;
    currDigit = digits[idx];
//     1. deactivate all digits (anodes)
    lat_SSD_AN1 = 1; // deactivate digit 1;
    lat_SSD_AN2 = 1; // deactivate digit 2;    
    lat_SSD_AN3 = 1; // deactivate digit 3;   
    lat_SSD_AN0 = 1; // deactivate digit 0;
    
    // 2. drive all the segments (cathodes)
    lat_SSD_CA = currDigit & 0x01;
    lat_SSD_CB = (currDigit & 0x02) >> 1;
    lat_SSD_CC = (currDigit & 0x04) >> 2;
    lat_SSD_CD = (currDigit & 0x08) >> 3;    
    lat_SSD_CE = (currDigit & 0x10) >> 4;
    lat_SSD_CF = (currDigit & 0x20) >> 5;
    lat_SSD_CG = (currDigit & 0x40) >> 6;
    lat_SSD_DP = (currDigit & 0x80) >> 7;     
    
    // 3. activate the current digit (anodes)
    
    switch(idx)
    {
        case 0:
            lat_SSD_AN0 = 0; // activate digit 0;        
            break;
        case 1:
           lat_SSD_AN1 = 0; // activate digit 1;
            break;
        case 2:
           lat_SSD_AN2 = 0; // activate digit 2;
            break;    
        case 3:
            lat_SSD_AN3 = 0; // activate digit 3;   
            break; 
    }    
    
   
    IFS0bits.T1IF = 0;       // clear interrupt flag
    
}


void SSD_Timer1Setup()
{
  PR1 = (int)(((float)(TMR_TIME2 * PB_FRQ) / 256) + 0.5); //set period register, generates one interrupt every 3 ms
  TMR1 = 0;                           //    initialize count to 0
  T1CONbits.TCKPS = 2;                //    1:64 prescale value
  T1CONbits.TGATE = 0;                //    not gated input (the default)
  T1CONbits.TCS = 0;                  //    PCBLK input (the default)
  T1CONbits.ON = 1;                   //    turn on Timer1
  IPC1bits.T1IP = 7;                  //    priority
  IPC1bits.T1IS = 2;                  //    subpriority
  IFS0bits.T1IF = 0;                  //    clear interrupt flag
  IEC0bits.T1IE = 1;                  //    enable interrupt
  macro_enable_interrupts();          //    enable interrupts at CPU
}


void SSD_Init()
{
    SSD_ConfigurePins();
    SSD_Timer1Setup();  
}


void SSD_ConfigurePins()
{
    // set pins as digital outputs.
    tris_SSD_CA = 0;
    tris_SSD_CB = 0;
    tris_SSD_CC = 0;
    tris_SSD_CD = 0;  
    tris_SSD_CE = 0;
    tris_SSD_CF = 0;
    tris_SSD_CG = 0;
    tris_SSD_DP = 0;  
    
    tris_SSD_AN0 = 0;
    tris_SSD_AN1 = 0;
    tris_SSD_AN2 = 0;
    tris_SSD_AN3 = 0;    
    
    // disable analog (set pins as digital))    
    ansel_SSD_AN0 = 0;
    ansel_SSD_AN1 = 0;
    
    PMCONbits.ON = 0;   // turn PM off
}



unsigned char SSD_GetDigitSegments(unsigned char d)
{
    unsigned char bResult = -1;
    if(d < NO_DIGITS)
    {
        bResult = digitSegments[d];
    }
    return bResult;
}



void SSD_WriteDigits(unsigned char d1, unsigned char d2, unsigned char d3, unsigned char d4, \
        unsigned char dp1, unsigned char dp2, unsigned char dp3, unsigned char dp4)
{
      T1CONbits.ON = 0;                   // turn off Timer1
    digits[0] = SSD_GetDigitSegments(d1);
    digits[1] = SSD_GetDigitSegments(d2);
    digits[2] = SSD_GetDigitSegments(d3);
    digits[3] = SSD_GetDigitSegments(d4);
    
    if(!dp1)
    {
        digits[0] |= 0x80;
    }
    if(!dp2)
    {
        digits[1] |= 0x80;
    }
    if(!dp3)
    {
        digits[2] |= 0x80;
    }
    if(!dp4)
    {
        digits[3] |= 0x80;
    }    
  T1CONbits.ON = 1;                   //  turn on Timer1
}


void SSD_WriteDigitsGrouped(unsigned int val, unsigned char dp)
{
    SSD_WriteDigits(val & 0xF, (val & 0xF0) >> 4, (val & 0xF00) >> 8, (val & 0xF000) >> 12,
            dp&1, (dp & 2) >> 1, (dp & 4) >> 2, (dp & 8) >> 3);
}

void SSD_Close()
{
    // stop the timer
    T1CONbits.ON = 0;// turn off Timer1
    // turn off digits
    lat_SSD_AN1 = 1; // deactivate digit 1;
    lat_SSD_AN2 = 1; // deactivate digit 2;    
    lat_SSD_AN3 = 1; // deactivate digit 3;   
    lat_SSD_AN0 = 1; // deactivate digit 0;
}


