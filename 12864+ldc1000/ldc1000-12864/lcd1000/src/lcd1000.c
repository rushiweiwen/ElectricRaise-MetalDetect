#include <msp430f149.h>
#include "lcd1000.h"

//===========函数的定义================
void SetVCoreUp(unsigned int level);

//====RP推算金属的距离=================
#define RPMAX    0x15
#define RPMIN    0x39

//=======================IO=============
#define  MISO_0     P3OUT &=~BIT2      
#define  MISO_1     P3OUT |= BIT2

#define  MOSI_0     P3OUT &=~BIT1         
#define  MOSI_1     P3OUT |= BIT1

#define  SCK_0      P3OUT &=~BIT3         
#define  SCK_1      P3OUT |= BIT3

#define  CSN_0      P3OUT &=~BIT0 
#define  CSN_1      P3OUT |= BIT0

//============变量的定义===============
static char DATA_BUF;
static char txaddr;
char rxbuff;

char proximtyData[2];
char frequencyData[3];

int  proximtyDataMIN;
int  frequencyDataMIN;

int  proximtyDataMAX;
int  frequencyDataMAX;

int  proximtyDataTEMP;
int  frequencyDataTEMP;

static unsigned char SpiRead(void)
{
	unsigned char i;
	for (i=0;i<8;i++)
	{
		DATA_BUF=DATA_BUF<<1;
		SCK_1;
		if ((P3IN&0x04))	//读取最高位，保存至最末尾，通过左移位完成整个字节
		{
			DATA_BUF|=0x01;
		}
		else
		{
			DATA_BUF&=~(0x01);
		}
		SCK_0;
	 }
	 return DATA_BUF;
}

static void SpiWrite(unsigned char send)
{
	unsigned char i;
	DATA_BUF=send;
	for (i=0;i<8;i++)
	{
	if (((DATA_BUF&0x80) != 0))	//总是发送最高位
        {
	    MOSI_1;
	 }
	else
	 {
	    MOSI_0;
	 }
	SCK_1;
	DATA_BUF=DATA_BUF<<1;
	SCK_0;
	}
}

char spi_writeByte( char addr, char data)
{
    txaddr = addr & ~0x80;
    CSN_0;
    SpiWrite(txaddr);
    SpiWrite(data);
    CSN_1;
    _NOP();
    return 0;
}

char spi_readByte(char addr)
{
    txaddr = addr | 0x80;
    CSN_0;
    SpiWrite(txaddr);
    rxbuff = SpiRead();
    _NOP();
    CSN_1;
    _NOP();
    return rxbuff;
}

void Init_LDC1000()
{
        spi_writeByte(LDC1000_CMD_RPMAX,       RPMAX);
        spi_writeByte(LDC1000_CMD_RPMIN,       RPMIN);
	spi_writeByte(LDC1000_CMD_SENSORFREQ,  0x94);
	spi_writeByte(LDC1000_CMD_LDCCONFIG,   0x17);
	spi_writeByte(LDC1000_CMD_CLKCONFIG,   0x02);
	spi_writeByte(LDC1000_CMD_INTCONFIG,   0x02);

	spi_writeByte(LDC1000_CMD_THRESHILSB,  0x50);
	spi_writeByte(LDC1000_CMD_THRESHIMSB,  0x14);
	spi_writeByte(LDC1000_CMD_THRESLOLSB,  0xC0);
	spi_writeByte(LDC1000_CMD_THRESLOMSB,  0x12);

	spi_writeByte(LDC1000_CMD_PWRCONFIG,   0x01);
}
void Init_IO()
{
    P3DIR |= 0xC0; 
    P3DIR |= 0xFB;   
    P3SEL&=0xF0;
    CSN_1;				            // Spi 	disable
    SCK_0;
}