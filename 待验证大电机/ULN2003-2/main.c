
#include <msp430x14x.h>
#include "init.h"

#include "ldc1000.h"
#include "key.h"
#include "OLED.h"


uchar phasecw[8] ={0x99,0x11,0x33,0x22,0x66,0x44,0xcc,0x88};//正转 电机导通相序 D-C-B-A
uchar phaseccw[8]={0x88,0xcc,0x44,0x66,0x22,0x33,0x11,0x99};//反转 电机导通相序 A-B-C-D


void int_pwm()
{
  P1SEL |= BIT2 + BIT3 ;     //选择 p1.2-TA1  ,P1.3-TA2  作为PWM输出　第二功能
  P1SEL &= ~(BIT0 + BIT1) ;     //选择 p1.0 p1.2 第一功能
  P1DIR |= BIT0 + BIT1 +BIT2 + BIT3 ;
  TACCR0 = 1000;    //PWM信号频率
  TACCR1 = 500 ;    // 占空比
  TACCTL1 = OUTMOD_7;
 // TACCR2 = 4000 ;
 // TACCTL2 = OUTMOD_7;
  TACTL |= TBSSEL_2 + MC_1 ;  //选择SMCLK 增加模式
  TACCTL0 =  CCIE;
  EN;
  U_DIR;
  
}



void timer_init()
{
  
  TBCTL = TBSSEL_2 + MC_1;
  TBCCTL0 = CCIE;                           // TBCCR0 interrupt enabled
  TBCCR0 = 20000;
 
  _EINT();  
}

void main( void )
{
  
  Clock_Init();
  WDTCTL = WDTPW + WDTHOLD;
  
 
  init_key();
  OLED_Init();
  
  Init_IO();
  Init_LDC1000();
  
  timer_init();
  int_pwm();
  
   P5SEL |= 0x00;
   P5DIR |= 0xff;
   
 
                               
   P2SEL=0x00;                    //设置IO口为普通I/O模式
   P2DIR = 0xff;                  //设置IO口方向为输出
   P2OUT = 0x00;

   delay_ms(10);
   while(1)
   {  
     
      key();
      
      ldc_readdata();  
      if(proximtyDataMAX>=5200) stop_cw=0;
  
      lcd_display();
     
      if(stop_cw) {EN;}
      else { U_EN;}
      if(flag_y) {EN;}
  
      if(y_cw) {DIR;}
      else {U_DIR;}
      

   }
}


#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A(void)
{
  // P2OUT ^=0XFF;
   if(stop_cw) timer_a++;
 
}


#pragma vector=TIMERB0_VECTOR
__interrupt void Timer_B (void)
{
 // timer_a++;
  
  if(timer_a>=20300)
  {
    timer_a=0;
    y_cw ^= 0x01;
    num++;
  }
  
  if(num>=21) { num=0;x_cw ^= 0x01; }
  
  if(x_cw) P5OUT= stop_cw *( 0x0f & phaseccw[tb_count]);
  else  P5OUT= stop_cw * ( 0x0f & phasecw[tb_count]);
  
  if(flag_x) 
  {
      if(x_cw) P5OUT=( 0x0f & phaseccw[tb_count]);
      else  P5OUT= ( 0x0f & phasecw[tb_count]);
   }
   
   tb_count++;
   if(tb_count>=8) tb_count=0;
                            
}