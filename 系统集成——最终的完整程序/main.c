#include <msp430f149.h>
#include "com.h"
#include "lcd1000.h"
#include "12864.h"

//延时函数，IAR自带，经常使用到
#define CPU_F ((double)8000000)   //外部高频晶振8MHZ
//#define CPU_F ((double)32768)   //外部低频晶振32.768KHZ
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0)) 
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0)) 

//自定义数据结构，方便使用
#define uchar unsigned char
#define uint  unsigned int
#define ulong unsigned long

//8个LED灯，连接在P6口，可通过断开电源停止使用，ADC使用时断开电源
#define LED8DIR         P6DIR
#define LED8            P6OUT                             //P6口接LED灯，8个

//4个独立按键连接在P10~P13
#define KeyPort         P1IN  
#define key1 5500
#define key2 6200
#define key3 8000
#define key4 9000

int arange=8000;
char a;


//*************************************************************************
//	初始化IO口子程序
//*************************************************************************
void Port_init()
{

  P1SEL = 0x00;                   //P1普通IO功能
  P1DIR = 0xF0;                   //P10~P13输入模式，外部电路已接上拉电阻
  P6SEL = 0x00;                   //P6口普通IO功能
  P6DIR = 0xFF;                   //P6口输出模式
}


//**********************************************************************
//	键盘扫描子程序，采用逐键扫描的方式
//**********************************************************************

uchar Key_Scan(void) 
{
  uchar key_check;
  uchar key_checkin;
  key_checkin=KeyPort;          	//读取IO口状态，判断是否有键按下
  key_checkin&= 0x0F;          		//读取IO口状态，判断是否有键按下
  if(key_checkin!=0x0F)            	//IO口值发生变化则表示有键按下
    {
      delay_ms(20);                  	//键盘消抖，延时20MS
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
  P6DIR|=BIT1;//发光二极管
  P6OUT|=BIT1;
    
  P1DIR|=BIT0;//给fpga的信号
  P1OUT&=~BIT0;
  
  
    WDTCTL = WDTPW + WDTHOLD;   //关闭看门狗
    Init_CLK();
    Ini_Lcd();
    Init_IO();
    Init_LDC1000();

 Port_init();  
   delay_ms(100);                         //延时100ms
  while(1)
    {



 Key_Scan();                       //键盘扫描，看是否有按键按下
      if(key!=0xff)                     //如果有按键按下，则显示该按键键值1～4
        {
	  {
            switch(key)
              {
	        case 1: LED8 = 0xFC; arange=key1;break;        //给不同的键赋键值，键值1，亮2个LED灯
                case 2: LED8 = 0xF3;arange=key2;break;        //给不同的键赋键值，键值2，亮2个LED灯
                case 3: LED8 = 0xCF;arange=key3;break;	 //给不同的键赋键值，键值3，亮2个LED灯
                case 4: LED8 = 0x3F;arange=key4;break;	 //给不同的键赋键值，键值4，亮2个LED灯
              }

          }
	
       }
     else
      {
        //LED=key;              	//没有按键的时候显示上次的键值
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
          P6OUT&=~BIT1;//灯亮
          P1OUT|=BIT0;//给fpga高电平，表示发现硬币
        }
        else 
        {
          P6OUT|=BIT1;
          P1OUT&=~BIT0;
        }
        _NOP();
    }
}