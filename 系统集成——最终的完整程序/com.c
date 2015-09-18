#include <msp430f149.h>

//=================================================================  
// 函数名称 ：void Init_CLK(void)
// 函数功能 ：时钟的选择与初始化  将子系统时钟与主系统时钟均设为高频晶体振荡器
// 入口参数 ：无
// 出口参数 ：无
//================================================================= 
void Init_CLK(void)
{
    unsigned int i;
      BCSCTL1&=~XT2OFF;   //XT2开启，LFTX1工作在低频模式，ACLK子系统时钟分频为1
      do
      {
           IFG1&=~OFIFG;
           for(i=0x20;i>0;i--);//清除OSCFault标志                     
      }
      while((IFG1&OFIFG)==OFIFG);
      BCSCTL2=0x00;    //先清空寄存器
      BCSCTL2=SELM_2 + SELS ;    //MCLK和SMCLK均选择XT2，不分频
}

