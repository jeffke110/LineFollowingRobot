/* 
 * File:   Timer3.h
 * Author: jlked
 *
 * Created on November 16, 2019, 9:26 PM
 */

#ifndef TIMER3_H
#define	TIMER3_H

#ifdef	__cplusplus
extern "C" {
#endif




void Increment();
void Timer3_init();
unsigned int SecondsTime();
unsigned int  TenthTime();
void Timer3_Setup();
void SSD_Init();
void SSD_WriteDigits(unsigned char d0, unsigned char d1, unsigned char d2, unsigned char d3, \
            unsigned char dp1, unsigned char dp2, unsigned char dp3, unsigned char dp4);
void SSD_WriteDigitsGrouped(unsigned int val, unsigned char dp);
unsigned char SSD_GetDigitSegments(unsigned char d);

// private functions
void SSD_ConfigurePins();
void SSD_Timer1Setup();

#ifdef	__cplusplus
}
#endif

#endif	/* TIMER3_H */

