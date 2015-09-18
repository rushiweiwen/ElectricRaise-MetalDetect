#include <msp430f149.h>
#include "com.h"
#include "lcd1000.h"
#include "12864.h"
char a;

void main()
{
    
    WDTCTL = WDTPW + WDTHOLD;   //πÿ±’ø¥√≈π∑
    Init_CLK();
    Ini_Lcd();
    Init_IO();
    Init_LDC1000();
    while(1)
    {
        proximtyData[0]  = spi_readByte(LDC1000_CMD_PROXLSB);
        proximtyData[1]  = spi_readByte(LDC1000_CMD_PROXMSB);
        frequencyData[0] = spi_readByte(LDC1000_CMD_FREQCTRLSB);
        frequencyData[1] = spi_readByte(LDC1000_CMD_FREQCTRLSB+1);
        frequencyData[2] = spi_readByte(LDC1000_CMD_FREQCTRLSB+2);
        proximtyDataMAX = ((unsigned char) proximtyData[1]<<8) + proximtyData [0];
        frequencyDataMAX =((unsigned char)frequencyData[1]<<8) + frequencyData[0];
        disNum(proximtyDataMAX%10+0x30,0x86);
        disNum(proximtyDataMAX/10%10+0x30,0x85);
        disNum(proximtyDataMAX/100%10+0x30,0x84);
        disNum(proximtyDataMAX/1000%10+0x30,0x83);
        disNum(proximtyDataMAX/10000%10+0x30,0x82);
        a = spi_readByte(LDC1000_CMD_REVID);
        _NOP();
    }
}