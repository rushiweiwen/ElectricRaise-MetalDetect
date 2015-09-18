#include <msp430x14x.h>
#include"Config.h"   //����ͷ�ļ�
uchar Flag=0;                           //��־λ
uchar Time=0; 

void TAPWM()
{ 
  /*********************d��һ·PWM��********************
  // P1SEL|=BIT3;//TA2��P1.3���
  // P1DIR|=BIT3; //TA2��P1.3���
  //TACCTL0=OUTMOD_7;//ģʽ7�ߵ�ƽPWM���PWM���� 
  // TACCR2=16;//TA2ռ�ձ�=16/32=50%
  //TACCTL2=OUTMOD_7;//ģʽ7�ߵ�ƽPWM���
  /*********************d�ڶ�·PWM��********************/
  P1SEL|=BIT2;//TA1��P1.2���
  P1DIR|=BIT2;//TA1��P1.2���
  TACTL|=MC_1+TASSEL_2+ID_3;//ʱ��Դѡ��ACLK��������ģʽTA���� 
  TACCTL1=OUTMOD_7;//ģʽ7�ߵ�ƽPWM���PWM����
  TACCR0=1000;//PWM������=32��ACLK����Լ����1000Hz����PWM������
  TACCR1=500;//TA1ռ�ձ�=16/32=50%����ռ�ձ�
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
void TIMERB_Init(void)                                   //��������ģʽ��������0XFFFF�����ж�
{
  TBCTL |= TBSSEL_2+MC_1+ID0+ID1+CNTL_0 ; //SMCLK��ʱ��Դ��8��Ƶ����������ģʽ��������0XFFFF�����ж�
  TBCCTL0 = CCIE;    	            //����ʱ���ж�
  TBCCR0=50000;
}

//***********************************************************************
//      ������
//***********************************************************************
void main(void)
{    
     WDT_Init();                         //���Ź�����
     Clock_Init();                       //ϵͳʱ������
     Port_init();                        //ϵͳ��ʼ��������IO������
    // TIMERB_Init();                                //����TIMERA 
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
//             TIMERB�жϷ��������Ҫ�ж��ж�����
//***********************************************************************
/*
#pragma vector = TIMERB0_VECTOR
__interrupt void Timer_A(void)
{   
  Time++;                     //���ñ�־λFlag
  if(Time==20)                //20���жϵ�����1��ʱ�䵽
   {
     P6OUT|=BIT0;   //FANGXIANG ZHENG     
    }
   else if(Time>20)
   {
     P6OUT&=~BIT0;
     if(Time==40) Time==40;
   }
}*/