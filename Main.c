// Jeffrey Kedda
//* Input :  Mic, IR sensors
// * Output: LCD, Wheels
==============================================================================*/
/*------------------ Board system settings. PLEASE DO NOT MODIFY THIS PART ----------*/
#ifndef _SUPPRESS_PLIB_WARNING          //suppress the plib warning during compiling
    #define _SUPPRESS_PLIB_WARNING      
#endif
#pragma config FPLLIDIV = DIV_2         // PLL Input Divider (2x Divider)
#pragma config FPLLMUL = MUL_20         // PLL Multiplier (20x Multiplier)
#pragma config FPLLODIV = DIV_1         // System PLL Output Clock Divider (PLL Divide by 1)
#pragma config FNOSC = PRIPLL           // Oscillator Selection Bits (Primary Osc w/PLL (XT+,HS+,EC+PLL))
#pragma config FSOSCEN = OFF            // Secondary Oscillator Enable (Disabled)
#pragma config POSCMOD = XT             // Primary Oscillator Configuration (XT osc mode)
#pragma config FPBDIV = DIV_8           // Peripheral Clock Divisor (Pb_Clk is Sys_Clk/8)
/*----------------------------------------------------------------------------*/

     
#include <xc.h>   //Microchip XC processor header which links to the PIC32MX370512L header
#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "Servo.h"
#include "swt.h"
#include "lcd.h"
#include "led.h"

#include "utils.h"

#define S1 PORTDbits.RD9 
#define S2 PORTDbits.RD11 
#define S3 PORTDbits.RD10
#define S4 PORTDbits.RD8

int TimerMode_L = 0;
    int TimerMode_R = 0;
    int TimeSecond = 0;
    int TimeTenth = 0;
    int TimeFULL = 0;
    int StartIncrement = 0;
    int RightSpeed = 0;
    int LeftSpeed = 0;
    
    char buf[16];
    
void MicInitialization();
void MICConfig();
unsigned int ValueMic();
void Other();
void delay_ms(int ms);
void Straight();
void SharpRight();
void SharpLeft();
void Reverse();
void SlightRight();
void SlightLeft();
void Right();
void Left();
void Stop();

int  usVal = 1;


int main(int argc, char** argv) {
    //Variables and Initializations
    TRISDbits.TRISD9 = 1, TRISDbits.TRISD11 = 1, TRISDbits.TRISD10 = 1, TRISDbits.TRISD8 = 1, DDPCONbits.JTAGEN = 0,DDPCONbits.JTAGEN = 0;  
    SRV_Init(), SWT_Init(), LED_Init(), LCD_Init(), SSD_Init(), Timer3_init();
    SSD_WriteDigits(0, 0, '1', '1' , 0, 1, 0, 0);
    LCD_DisplayClear();
    delay_ms(300);
    LCD_WriteStringAtPos("TEAM 23", 0,0);
    buf[3] = ' ', buf[4] = ' ', buf[5] = ' ', buf[6] = ' ', buf[7] = ' ', buf[8] = ' ', buf[9] = ' ', buf[10] = ' ', buf[11] = ' ', buf[12] = ' ';
    int Count=0, ClapCount = 0;
    MicInitialization();
    
    while(ClapCount < 2){
        if(ValueMic() > 1000){
            ClapCount++;
            delay_ms(200);
        }
    }
    
    TimerMode_L = 1;
    TimerMode_R = 1;
    
    int BLACKTAPELOCK = 0;
    int BLACKTAPECOUNT = 0;
    int blackInArow = 0;
    
    
    while(S2!=1 && S3 !=1 && S1 != 1 && S4 !=1){
                Straight();
                BLACKTAPELOCK = 0;
    }
    while(BLACKTAPECOUNT != 3){
       
        if(S2==0 && S3==0 && S1==0 && S4==0 && BLACKTAPELOCK == 0){
               //BLACKTAPECOUNT++;
               BLACKTAPELOCK = 1;
                buf[0] = buf[0];
                buf[1] = buf[1];
                buf[2] = buf[2];
                buf[13] = buf[13];
                buf[14] = buf[14];
                buf[15] = buf[15];
        }
        if(BLACKTAPELOCK == 1){
            blackInArow++;
        }
        if(blackInArow == 37){
            BLACKTAPECOUNT++;
            blackInArow=0;
            Straight();
        }
        while(S2==0 && S3==0 && S1==1 && S4==1){
                Straight();
                BLACKTAPELOCK = 0;
           }
        while(S2==1 && S3==1 && S1==1 && S4==1){
                Reverse();
                BLACKTAPELOCK = 0;
        }
        while(S2==0 && S3==0 && S1==1 && S4==0){
                SlightLeft();
                BLACKTAPELOCK = 0;
        }
        while(S2==1 && S3==0 && S1==1 && S4==0){
                Left();
                BLACKTAPELOCK = 0;
        }
        while(S2==1 && S3==1 && S1==1 && S4==0){
                SharpLeft();
                BLACKTAPELOCK = 0;
        }
        while(S2==0 && S3==0 && S1==0 && S4==1){ 
                SlightRight();
                BLACKTAPELOCK = 0;
        }
        while(S2==0 && S3==1 && S1==0 && S4==1){
                Right();
                BLACKTAPELOCK = 0;
        }
        while(S2==1 && S3==1 && S1==0 && S4==1){      
                SharpRight();
                BLACKTAPELOCK = 0;
        }
        
        Other();

    }
    Stop();
     
    return (EXIT_SUCCESS);
}
/* ------------------------------------------------------------ */
/** Other
**	Parameters:
**	Return Value:
**	Description:  These are for when the sensor logic doesn't make sense
*/
void Other(){
     TimeSSD();    
     SRV_SetPulseMicroseconds1(RightSpeed); //1000
            //LED_SetValue(0, 0), LED_SetValue(1, 0), LED_SetValue(2, 1), LED_SetValue(3, 1);
            buf[13] = buf[13];
            buf[14] = buf[14];
            buf[15] = buf[15];
     SRV_SetPulseMicroseconds2(LeftSpeed); //2000
            //LED_SetValue(4, 1), LED_SetValue(5, 1), LED_SetValue(6, 0), LED_SetValue(7, 0);
            buf[0] = buf[0];
            buf[1] = buf[1];
            buf[2] = buf[2];
    LCD_WriteStringAtPos(buf, 1,0);        
}


/* ------------------------------------------------------------ */
/** Straight
**	Parameters:
**	Return Value:
**	Description:  Goes Straight
*/
void Straight(){
     TimeSSD();    
     SRV_SetPulseMicroseconds1(1400); //1000
     RightSpeed = 1400;
            LED_SetValue(0, 0), LED_SetValue(1, 0), LED_SetValue(2, 1), LED_SetValue(3, 1);
            buf[13] = 'F';
            buf[14] = 'W';
            buf[15] = 'D';
     SRV_SetPulseMicroseconds2(1600); //2000
     LeftSpeed = 1600;
            LED_SetValue(4, 1), LED_SetValue(5, 1), LED_SetValue(6, 0), LED_SetValue(7, 0);
            buf[0] = 'F';
            buf[1] = 'W';
            buf[2] = 'D';
    LCD_WriteStringAtPos(buf, 1,0);        
}
/* ------------------------------------------------------------ */
/** SharpRight
**	Parameters:
**	Return Value:
**	Description:  Makes a sharp Right Turn
*/
void SharpRight(){
    TimeSSD();
    SRV_SetPulseMicroseconds1(1600); //2000
    RightSpeed = 1600;
            LED_SetValue(0, 1), LED_SetValue(1, 1), LED_SetValue(2, 0), LED_SetValue(3, 0);
            buf[13] = 'R';
            buf[14] = 'E';
            buf[15] = 'V';
    SRV_SetPulseMicroseconds2(1600); //2000
    LeftSpeed = 1600;
            LED_SetValue(4, 1) ,LED_SetValue(5, 1), LED_SetValue(6, 0), LED_SetValue(7, 0);
            buf[0] = 'F';
            buf[1] = 'W';
            buf[2] = 'D';
            LCD_WriteStringAtPos(buf, 1,0);

}
/* ------------------------------------------------------------ */
/** SharpLeft
**	Parameters:
**	Return Value:
**	Description:  Makes a sharp LEFT Turn
*/
void SharpLeft(){
    TimeSSD();
    SRV_SetPulseMicroseconds2(1400); //1000
    LeftSpeed = 1400;
            LED_SetValue(4, 0), LED_SetValue(5, 0), LED_SetValue(6, 1), LED_SetValue(7, 1);
            buf[0] = 'R';
            buf[1] = 'E';
            buf[2] = 'V'; 
    SRV_SetPulseMicroseconds1(1400); //1000
    RightSpeed = 1400;
            LED_SetValue(0, 0), LED_SetValue(1, 0), LED_SetValue(2, 1), LED_SetValue(3, 1);
            buf[13] = 'F';
            buf[14] = 'W';
            buf[15] = 'D';
            LCD_WriteStringAtPos(buf, 1,0);
}
/* ------------------------------------------------------------ */
/** Reverse
**	Parameters:
**	Return Value:
**	Description:  Go Reverse
*/
void Reverse(){
    TimeSSD();  
    SRV_SetPulseMicroseconds2(1400); //1000
    LeftSpeed = 1400;
            LED_SetValue(4, 0), LED_SetValue(5, 0), LED_SetValue(6, 1), LED_SetValue(7, 1);
            buf[0] = 'R';
            buf[1] = 'E';
            buf[2] = 'V';
    SRV_SetPulseMicroseconds1(1600); //2000
    RightSpeed = 1600;
           LED_SetValue(0, 1), LED_SetValue(1, 1), LED_SetValue(2, 0), LED_SetValue(3, 0);
            buf[13] = 'R';
            buf[14] = 'E';
            buf[15] = 'V';
            LCD_WriteStringAtPos(buf, 1,0);

            
}
/* ------------------------------------------------------------ */
/** SlightRight
**	Parameters:
**	Return Value:
**	Description:  Makes a slight right turn
*/
void SlightRight(){
    TimeSSD();   
    SRV_SetPulseMicroseconds2(1600); //2000
    LeftSpeed = 1600;
            LED_SetValue(4, 1), LED_SetValue(5, 1), LED_SetValue(6, 0), LED_SetValue(7, 0);
            buf[0] = 'F';
            buf[1] = 'W';
            buf[2] = 'D';     
    SRV_SetPulseMicroseconds1(1450); //1000
    RightSpeed = 1450;
            LED_SetValue(0, 0), LED_SetValue(1, 0), LED_SetValue(2, 1), LED_SetValue(3, 1);
            buf[13] = 'F';
            buf[14] = 'W';
            buf[15] = 'D';
            LCD_WriteStringAtPos(buf, 1,0);

}
/* ------------------------------------------------------------ */
/** SlightLeft
**	Parameters:
**	Return Value:
**	Description:  Makes a slight left turn
*/
void SlightLeft(){
    TimeSSD();   
    SRV_SetPulseMicroseconds1(1400); //1000
    RightSpeed = 1400;
            LED_SetValue(0, 0), LED_SetValue(1, 0), LED_SetValue(2, 1), LED_SetValue(3, 1);
            buf[13] = 'F';
            buf[14] = 'W';
            buf[15] = 'D';
    SRV_SetPulseMicroseconds2(1550); //2000
    LeftSpeed = 1550;
            LED_SetValue(4, 1), LED_SetValue(5, 1), LED_SetValue(6, 0), LED_SetValue(7, 0);
            buf[0] = 'F';
            buf[1] = 'W';
            buf[2] = 'D';
            LCD_WriteStringAtPos(buf, 1,0);
}
/* ------------------------------------------------------------ */
/** Right
**	Parameters:
**	Return Value:
**	Description:  Makes a  right turn
*/
void Right(){
    TimeSSD();   
    SRV_SetPulseMicroseconds2(1600); //2000
    LeftSpeed = 1600;
            LED_SetValue(4, 1), LED_SetValue(5, 1), LED_SetValue(6, 0), LED_SetValue(7, 0);
            buf[0] = 'F';
            buf[1] = 'W';
            buf[2] = 'D';
    SRV_SetPulseMicroseconds1(1500); 
    RightSpeed = 1500; 
            LED_SetValue(0, 0), LED_SetValue(1, 0), LED_SetValue(2, 0), LED_SetValue(3, 0);
            buf[13] = 'S';
            buf[14] = 'T';
            buf[15] = 'P';
            LCD_WriteStringAtPos(buf, 1,0);
}
/* ------------------------------------------------------------ */
/** Left
**	Parameters:
**	Return Value:
**	Description:  Makes a Left turn
*/
void Left(){
    TimeSSD();       
    SRV_SetPulseMicroseconds2(1500);
    LeftSpeed = 1500;
            LED_SetValue(4, 0), LED_SetValue(5, 0), LED_SetValue(6, 0), LED_SetValue(7, 0);
            buf[0] = 'S';
            buf[1] = 'T';
            buf[2] = 'P';   
    SRV_SetPulseMicroseconds1(1400); //1000
    RightSpeed = 1400;
            LED_SetValue(0, 0), LED_SetValue(1, 0), LED_SetValue(2, 1), LED_SetValue(3, 1);
            buf[13] = 'F';
            buf[14] = 'W';
            buf[15] = 'D';
            LCD_WriteStringAtPos(buf, 1,0);
}
/* ------------------------------------------------------------ */
/** Stop
**	Parameters:
**	Return Value:
**	Description:  Makes a Stop
*/
void Stop(){
    SRV_SetPulseMicroseconds2(1500);
            LED_SetValue(4, 0), LED_SetValue(5, 0), LED_SetValue(6, 0), LED_SetValue(7, 0);
            buf[0] = 'S';
            buf[1] = 'T';
            buf[2] = 'P';     
    SRV_SetPulseMicroseconds1(1500); 
            LED_SetValue(0, 0), LED_SetValue(1, 0), LED_SetValue(2, 0), LED_SetValue(3, 0);
            buf[13] = 'S';
            buf[14] = 'T';
            buf[15] = 'P';
            LCD_WriteStringAtPos(buf, 1,0);

}


/* ------------------------------------------------------------ */
/** TimeSSD
**	Parameters:
**	Return Value:
**	Description:  Controls Time
*/
void TimeSSD(){
     if((TimerMode_R == 1 || TimerMode_L == 1) && (StartIncrement == 0) ){
            Increment();
            StartIncrement = 1;
        }
    if(TimerMode_R == 1 || TimerMode_L == 1){
            
        TimeTenth = TenthTime();
        if(TimeTenth == 9){
           TimeSecond++;
                TimeTenth = 0; 
        }
        if(TimeSecond == 1){
                TimeFULL++;
                TimeSecond = 0;     
        }
        int Time1 = TimeFULL%10;
        int Time10 = (TimeFULL/10) %10;
        int Time100 = (TimeFULL/100) %10;
        int Time1000 = (TimeFULL/1000) %10;
            
        if(TimeFULL < 100){
                int Time1 = TimeFULL%10;
                int Time10 = (TimeFULL/10) %10;
                SSD_WriteDigits(Time1, Time10, '1', '1' , 0, 1, 0, 0);
        }
        
        if(TimeFULL < 1000 && TimeFULL >= 100){
                int Time1 = TimeFULL%10;
                int Time10 = (TimeFULL/10) %10;
                
                SSD_WriteDigits(Time1, Time10, Time100, '1' , 0, 1, 0, 0);
        }
        if(TimeFULL < 10000 && TimeFULL >= 1000){
           int Time1 = TimeFULL%10;
           int Time10 = (TimeFULL/10) %10;
           int Time100 = (TimeFULL/100) %10;
           SSD_WriteDigits(Time1, Time10, Time100, Time1000 , 0, 1, 0, 0);
         }      
     
    }
}


void delay_ms(int ms){
	int		i,counter;
	for (counter=0; counter<ms; counter++){
        for(i=0;i<1426;i++){}   //software delay 1 millisec
    }
}

//NAME: MicInitialization
//PARAMETERS: None
//RETURN: None
//DESCRIPTION: Gets Mic ready!!
void MicInitialization()
{
    MICConfig();
    ADC_Init();
}

//NAME: MICConfig
//PARAMETERS: None
//RETURN: None
//DESCRIPTION: Gets Mic ready!!
void MICConfig()
{
    tris_MIC = 1;    
    ansel_MIC = 1;   
}

//NAME: ValueMic
//PARAMETERS: None
//RETURN: None
//DESCRIPTION: Gets Mic ready for analog!!
unsigned int ValueMic()
{
    return ADC_AnalogRead(4);   
}