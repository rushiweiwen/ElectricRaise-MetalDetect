#ifndef _LCD1000_H
#define _LCD1000_H

//================LDC1000�Ĵ�����ַ============
#define LDC1000_CMD_REVID       0x00
#define LDC1000_CMD_RPMAX       0x01
#define LDC1000_CMD_RPMIN       0x02
#define LDC1000_CMD_SENSORFREQ  0x03
#define LDC1000_CMD_LDCCONFIG   0x04
#define LDC1000_CMD_CLKCONFIG   0x05
#define LDC1000_CMD_THRESHILSB  0x06
#define LDC1000_CMD_THRESHIMSB  0x07
#define LDC1000_CMD_THRESLOLSB  0x08
#define LDC1000_CMD_THRESLOMSB  0x09
#define LDC1000_CMD_INTCONFIG   0x0A
#define LDC1000_CMD_PWRCONFIG   0x0B
#define LDC1000_CMD_STATUS      0x20
#define LDC1000_CMD_PROXLSB     0x21
#define LDC1000_CMD_PROXMSB     0x22
#define LDC1000_CMD_FREQCTRLSB  0x23
#define LDC1000_CMD_FREQCLRMID  0x24
#define LDC1000_CMD_FREQCLRMSB  0x25

extern char spi_writeByte( char addr, char data);
extern char spi_readByte(char addr);
extern void Init_LDC1000();
extern void Init_IO();

extern char proximtyData[2];
extern char frequencyData[3];
extern int  proximtyDataMAX;
extern int  frequencyDataMAX;

#endif