#include <msp430f149.h>
#include "com.h"
#include "lcd1000.h"
#include "12864.h"

//��ʱ������IAR�Դ�������ʹ�õ�
#define CPU_F ((double)8000000)   //�ⲿ��Ƶ����8MHZ
//#define CPU_F ((double)32768)   //�ⲿ��Ƶ����32.768KHZ
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0)) 
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0)) 

//�Զ������ݽṹ������ʹ��
#define uchar unsigned char
#define uint  unsigned int
#define ulong unsigned long

//8��LED�ƣ�������P6�ڣ���ͨ���Ͽ���Դֹͣʹ�ã�ADCʹ��ʱ�Ͽ���Դ
#define LED8DIR         P6DIR
#define LED8            P6OUT                             //P6�ڽ�LED�ƣ�8��

//4����������������P10~P13
#define KeyPort         P1IN  
#define key1 5500
#define key2 6200
#define key3 8000
#define key4 9000

int arange=8000;
char a;


//*************************************************************************
//	��ʼ��IO���ӳ���
//*************************************************************************
void Port_init()
{

  P1SEL = 0x00;                   //P1��ͨIO����
  P1DIR = 0xF0;                   //P10~P13����ģʽ���ⲿ��·�ѽ���������
  P6SEL = 0x00;                   //P6����ͨIO����
  P6DIR = 0xFF;                   //P6�����ģʽ
}


//**********************************************************************
//	����ɨ���ӳ��򣬲������ɨ��ķ�ʽ
//**********************************************************************

uchar Key_Scan(void) 
{
  uchar key_check;
  uchar key_checkin;
  key_checkin=KeyPort;          	//��ȡIO��״̬���ж��Ƿ��м�����
  key_checkin&= 0x0F;          		//��ȡIO��״̬���ж��Ƿ��м�����
  if(key_checkin!=0x0F)            	//IO��ֵ�����仯���ʾ�м�����
    {
      delay_ms(20);                  	//������������ʱ20MS
      key_checkin=KeyPort;
      if(key_checkin!=0x1F)
        {  
          key_check=KeyPort;
          switch (key_check & 0x0F)
            {
              case 0x0E:key=1;break;
              case 0x0D:key=2;break;
              case 0x0B:key=3;break;
              case 0x07:key=4;break;
            }
          
        }
      
   }
  else
   {
     key=0xFF;        
   }
  return key;
} 


void main()
{
  P6DIR|=BIT1;//���������
  P6OUT|=BIT1;
    
  P1DIR|=BIT0;//��fpga���ź�
  P1OUT&=~BIT0;
  
  
    WDTCTL = WDTPW + WDTHOLD;   //�رտ��Ź�
    Init_CLK();
    Ini_Lcd();
    Init_IO();
    Init_LDC1000();

 Port_init();  
   delay_ms(100);                         //��ʱ100ms
  while(1)
    {



 Key_Scan();                       //����ɨ�裬���Ƿ��а�������
      if(key!=0xff)                     //����а������£�����ʾ�ð�����ֵ1��4
        {
	  {
            switch(key)
              {
	        case 1: LED8 = 0xFC; arange=key1;break;        //����ͬ�ļ�����ֵ����ֵ1����2��LED��
                case 2: LED8 = 0xF3;arange=key2;break;        //����ͬ�ļ�����ֵ����ֵ2����2��LED��
                case 3: LED8 = 0xCF;arange=key3;break;	 //����ͬ�ļ�����ֵ����ֵ3����2��LED��
                case 4: LED8 = 0x3F;arange=key4;break;	 //����ͬ�ļ�����ֵ����ֵ4����2��LED��
              }

          }
	
       }
     else
      {
        //LED=key;              	//û�а�����ʱ����ʾ�ϴεļ�ֵ
      }
      
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
  
        if(proximtyDataMAX>arange)
        {
          P6OUT&=~BIT1;//����
          P1OUT|=BIT0;//��fpga�ߵ�ƽ����ʾ����Ӳ��
        }
        else 
        {
          P6OUT|=BIT1;
          P1OUT&=~BIT0;
        }
        _NOP();
    }
}