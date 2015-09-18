typedef unsigned char uint8;
typedef unsigned short int uint16;
typedef unsigned long int uint32;
typedef signed char int8;
typedef short int int16;
typedef int int32;


typedef unsigned char uchar;
typedef unsigned int uint;

#define CPU_F ((double)8000000) 
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0)) 
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0)) 

#define EN  P1OUT |= 0x01        //大电机控制EN , DIR
#define U_EN  P1OUT &= ~(0x01)
#define DIR  P1OUT |= 0x02       
#define U_DIR  P1OUT &= ~(0x02)

uint32 speed=10000;

uint8 x_cw=1,y_cw=1,stop_cw=0;

uchar i=0,k=1;
uchar tb_count=0;
uchar ta_count=0;
uint32 timer_a=0;
uchar num=0;
uchar flag_x=0,flag_y=0;




void Clock_Init()
{
  uchar i;
  BCSCTL1&=~XT2OFF;                 //打开XT振荡器
  BCSCTL2|=SELM1+SELS;              //MCLK为8MHZ，SMCLK为8MHZ
  do{
    IFG1&=~OFIFG;                   //清除震荡标志
    for(i=0;i<100;i++)
       _NOP();                      //延时等待
  }
  while((IFG1&OFIFG)!=0);           //如果标志为1，则继续循环等待
  IFG1&=~OFIFG; 
}