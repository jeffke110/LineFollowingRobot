#ifndef _SRV_H    /* Guard against multiple inclusion */
#define _SRV_H

void SRV_Init();
//void RIGHTwheelCLOCK();
//void RIGHTwheelCOUNTER(unsigned short usVal);
void SRV_SetPulseMicroseconds2(unsigned short usVal);
void SRV_SetPulseMicroseconds1(unsigned short usVal);
//void LEFTwheelCLOCK();
//void RIGHTwheelCOUNTER();
void SRV_Close();

// private functions
void SRV_ConfigureOCs();
void SRV_ConfigurePins();

#endif /* _SRV_H */

/* *****************************************************************************
 End of File
 */