#include <msp430x14x.h>
#include"Config.h"   //配置头文件
uchar Flag=0;                           //标志位
uchar Time=0; 

void TAPWM()
{ 
  /*********************d第一路PWM波********************
  // P1SEL|=BIT3;//TA2从P1.3输出
  // P1DIR|=BIT3; //TA2从P1.3输出
  //TACCTL0=OUTMOD_7;//模式7高电平PWM输出PWM设置 
  // TACCR2=16;//TA2占空比=16/32=50%
  //TACCTL2=OUTMOD_7;//模式7高电平PWM输出
  /*********************d第二路PWM波********************/
  P1SEL|=BIT2;//TA1从P1.2输出
  P1DIR|=BIT2;//TA1从P1.2输出
  TACTL|=MC_1+TASSEL_2+ID_3;//时钟源选择ACLK，增计数模式TA设置 
  TACCTL1=OUTMOD_7;//模式7高电平PWM输出PWM设置
  TACCR0=1000;//PWM总周期=32个ACLK周期约等于1000Hz设置PWM的周期
  TACCR1=500;//TA1占空比=16/32=50%设置占空比
}
// IO
void Port_init()
{
    P6SEL&=~BIT0+~BIT1;
    P6DIR|=BIT0+BIT1;
}
//***********************************************************************
//      TIMERB
//***********************************************************************
void TIMERB_Init(void)                                   //连续计数模式，计数到0XFFFF产生中断
{
  TBCTL |= TBSSEL_2+MC_1+ID0+ID1+CNTL_0 ; //SMCLK做时钟源，8分频，连续计数模式，计数到0XFFFF，开中断
  TBCCTL0 = CCIE;    	            //允许定时器中断
  TBCCR0=50000;
}

//***********************************************************************
//      主程序
//***********************************************************************
void main(void)
{    
     WDT_Init();                         //看门狗设置
     Clock_Init();                       //系统时钟设置
     Port_init();                        //系统初始化，设置IO口属性
    // TIMERB_Init();                                //设置TIMERA 
     TAPWM();
   //  _EINT();
     while (1) 
      {
        
        P6OUT&=~BIT0;
        delay_ms(6000);
        P6OUT|=BIT0;
        delay_ms(4000);
      }
}
//***********************************************************************
//             TIMERB中断服务程序，需要判断中断类型
//***********************************************************************
/*
#pragma vector = TIMERB0_VECTOR
__interrupt void Timer_A(void)
{   
  Time++;                     //设置标志位Flag
  if(Time==20)                //20次中断到来即1秒时间到
   {
     P6OUT|=BIT0;   //FANGXIANG ZHENG     
    }
   else if(Time>20)
   {
     P6OUT&=~BIT0;
     if(Time==40) Time==40;
   }
}*/