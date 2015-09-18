#include <MSP430F149.h>
#include "math.h"
#include "com.h"
#include "12864.h"


#define LCD_DataIn    P4DIR=0x00    //数据口方向设置为输入
#define LCD_DataOut   P4DIR=0xff    //数据口方向设置为输出
#define LCD2MCU_Data  P4IN
#define MCU2LCD_Data  P4OUT
#define LCD_CMDOut    P5DIR|=0xff    //P3口的低三位设置为输出
#define LCD_RS_H      P5OUT|=BIT5      //P3.0
#define LCD_RS_L      P5OUT&=~BIT5   //P3.0
#define LCD_RW_H      P5OUT|=BIT6   //P3.1
#define LCD_RW_L      P5OUT&=~BIT6     //P3.1
#define LCD_EN_H      P5OUT|=BIT7   //P3.2
#define LCD_EN_L      P5OUT&=~BIT7    //P3.2



/*******************************************
函数名称：Delay_1ms
功    能：延时约1ms的时间
参    数：无
返回值  ：无
********************************************/
static void Delay_1ms(void)
{
	uchar i;
    
	for(i = 150;i > 0;i--)  _NOP();
} 
/*******************************************
函数名称：Delay_Nms
功    能：延时N个1ms的时间
参    数：n--延时长度
返回值  ：无
********************************************/
static void Delay_Nms(uint n)
{
    uint i;
    
    for(i = n;i > 0;i--)    Delay_1ms();
}
/*******************************************
函数名称:Clear_GDRAM
功    能:清除液晶GDRAM中的随机数据
参    数:无
返回值  :无
********************************************/
void Clear_GDRAM(void)
{
    uchar i,j,k;
     
    Write_Cmd(0x34);        //打开扩展指令集
    i = 0x80;            
    for(j = 0;j < 32;j++)
    {
        Write_Cmd(i++);
        Write_Cmd(0x80);
        for(k = 0;k < 16;k++)
        {
            Write_Data(0x00);
        }
    }
    i = 0x80;
    for(j = 0;j < 32;j++)
    {
        Write_Cmd(i++);
        Write_Cmd(0x88);       
        for(k = 0;k < 16;k++)
        {
            Write_Data(0x00);
        } 
    }   
    Write_Cmd(0x30);        //回到基本指令集
}
/*******************************************
函数名称：Write_Cmd
功    能：向液晶中写控制命令
参    数：cmd--控制命令
返回值  ：无
********************************************/
void Write_Cmd(uchar cmd)
{
    uchar lcdtemp = 0;
			
    LCD_RS_L;
    LCD_RW_H;
    LCD_DataIn;  
    do                       //判忙
    {    
        LCD_EN_H;
       						
       	lcdtemp = LCD2MCU_Data; 
        _NOP();
        
       	LCD_EN_L;
        
    }
    while(lcdtemp & 0x80); 
    
    LCD_DataOut;    
    LCD_RW_L;  		
    MCU2LCD_Data = cmd; 
    LCD_EN_H;
    _NOP();
    _NOP();
    _NOP();						  			
    LCD_EN_L;
}
/*******************************************
函数名称：Write_Data
功    能：向液晶中写显示数据
参    数：dat--显示数据
返回值  ：无
********************************************/
void  Write_Data(uchar dat)
{
    uchar lcdtemp = 0;   
        
    LCD_RS_L;
    LCD_RW_H;  
    LCD_DataIn;   
    do                       //判忙
    {    
        LCD_EN_H;
        _NOP();						
        lcdtemp = LCD2MCU_Data; 
        LCD_EN_L;      
    }
    while(lcdtemp & 0x80);  
    
    LCD_DataOut; 
    LCD_RS_H;
    LCD_RW_L;  
        
    MCU2LCD_Data = dat;
    LCD_EN_H;
    _NOP();
    _NOP();
    _NOP();
    LCD_EN_L;
}  
/*******************************************
函数名称：Ini_Lcd
功    能：初始化液晶模块
参    数：无
返回值  ：无
********************************************/
void Ini_Lcd(void)
{                  
    LCD_CMDOut;    //液晶控制端口设置为输出
    
    Delay_Nms(500);
    Write_Cmd(0x30);   //基本指令集
    Delay_1ms();
    Write_Cmd(0x02);   // 地址归位
    Delay_1ms();
	Write_Cmd(0x0c);   //整体显示打开,游标关闭
    Delay_1ms();
	Write_Cmd(0x01);   //清除显示
    Delay_1ms();
	Write_Cmd(0x06);   //游标右移
    Delay_1ms();
	Write_Cmd(0x80);   //设定显示的起始地址
        
    Clear_GDRAM();
}
/*******************************************
函数名称：Disp_HZ
功    能：控制液晶显示汉字
参    数：addr--显示位置的首地址
          pt--指向显示数据的指针
          num--显示字符个数
返回值  ：无
********************************************/
void Disp_HZ(uchar addr,const uchar * pt,uchar num)
{
    uchar i;
		
    Write_Cmd(addr); 
      for(i = 0;i < (num*2);i++) 
      Write_Data(*(pt++)); 
} 

/*******************************************
函数名称：Read_Data
功    能：读取液晶中的数据
参    数：无
返回值  ：temp，读取的数据值
********************************************/
uchar Read_Data()
{
  uchar temp;   
  LCD_DataIn;
  LCD_RW_H; //读数据,高电平有效
  LCD_EN_L;
  LCD_RS_H;     //读数据，高电平有效
  delay_us(15); //延时最小值1500ns 
  _NOP();
  _NOP();
 
  LCD_EN_H; //lcden一旦拉高，数据开始读入
  temp=LCD2MCU_Data;//读出数据口的值       
  delay_us(15); //读数据持续时间最小值1500ns
  _NOP();
  _NOP();
   _NOP();
    _NOP();
     _NOP();
 
  LCD_EN_L; //再次置低，准备下一次拉高
  LCD_DataOut;
  return temp;  //将读到的数据返回
}

/*******************************************
函数名称: Lcd_set_dot
功    能:ST7920控制的12864任意位置打点函数
参    数:uchar x,uchar y，坐标点,范围x[0:127],y[0:63]
返回值  :无
说明：坐标系示意  
      -----0---x---127----------
     |0
     |y
     |63
********************************************/
void Draw_Dot(uchar x,uchar y)
{
 // Write_Cmd(0x00);
  uchar x_byte,x_bit;   //确定dot在横轴上的哪个字节内的哪个位
  uchar y_byte,y_bit;   //确定dot在竖轴上的哪个字节内的哪个位
  uchar TempH,TempL;    //用来存放读出来的原始数据的高8位和低8位，
            //目的是为了避免新的数据点写进去后覆盖掉前面的点
 
  x=x&0x7f;     //限制横坐标所在范围，为0-127
  y=y&0x3f;     //限制纵坐标所在范围，为0-31(上半屏)  32-63(下半屏)
 
  x_byte=x/16;      //确定该dot在哪一个字节  
  x_bit=x%16;       //确定该dot在哪一个点
  y_byte=y/32;      //确定该dot在那个半屏,0:上半屏 1:下半屏 
  y_bit=y%32;       //确定该dot在第几行(因为到了下半屏也是从0行开始到31行)
     
  Write_Cmd(0x34);          //打开扩展指令集,关闭基本功能模式，绘图显示关闭
  Write_Cmd(0x80+y_bit);        //先将光标纵坐标写到指定行
  Write_Cmd(0x80+x_byte+8*y_byte);  //再将光标横坐标写到指定位置,若y_byte为1,则说明大于了31行(从0行算起)
                    //则需将AC指针写到0x88处即ST7920控制下的第3行
  
  Read_Data();               //当下设定AC地址指令后，若要读取数据需要预先dummy read一次，才会读到正确数据
                 //第2次读取时不需要dummy read,除非又重新设定AC位置才需再次dummy read
 
  TempH=Read_Data();            //读取高8位数据
  TempL=Read_Data();            //读取低8位数据                                           
     
  Write_Cmd(0x80+y_bit);        //读操作会改变AC位置，所以需要重新写入1次
  Write_Cmd(0x80+x_byte+8*y_byte);  
 
  if(x_bit<8)
  {
    Write_Data(TempH | (0x01<<(7-x_bit))); //如果要写的dot在高8位,则这样写利用or运算避免新数据覆盖原先数据
    Write_Data(TempL);          //原先的低8位数据照常写入
  }
  else
  {
    Write_Data(TempH);                      //如果要写的dot在低8位,原先的高8位数据照常写入
    Write_Data(TempL | (0x01<<(15-x_bit))); //低8位则这样写利用or运算避免新数据覆盖原先数据
  }
    
  
  Write_Cmd(0x36);      //开绘图显示                                 
  Write_Cmd(0x30);      //回到GDRAM显示模式
}
/*******************************************
函数名称: sin_wave_y
功    能:LCD画正弦波
参    数:t，横坐标
返回值  :y，纵坐标 
********************************************/
uchar sin_wave_y(uchar t,uchar w)
{      
 //unsigned int t;
 float y;
 
 uchar high = 30; //最高点与坐标远点(右下角)的垂直距离-2

 //for(t=0;t<127;t++)
 //{
    //Draw_Dot(t,31);
    //y=(high/2)*(1-sin(2*pi*t/f)); //计算出y的坐标
    y=(high/2)*(1-sin(2*PI*t/20)); //计算出y的坐标
    
    //Draw_Dot(t,61 -(uchar)y); 
 //}
    return 62-(uchar)y;
}
/*******************************************
函数名称: Draw_Line
功    能:LCD画直线
参    数:uchar x1,uchar y1,uchar x2,uchar y2
返回值  :无 
********************************************/
void Draw_Line(uchar x1,uchar y1,uchar x2,uchar y2)
{

   int dx,dy,e;
   dx=x2-x1; 
   dy=y2-y1;
      
   if(dx>=0)
   {
    if(dy >= 0) // dy>=0
    {
     if(dx>=dy) // 1/8 octant
     {
      e=dy-dx/2;
      while(x1<=x2)
      {
       Draw_Dot(x1,y1);
       if(e>0){y1+=1;e-=dx;} 
       x1+=1;
       e+=dy;
      }
     }
     else  // 2/8 octant
     {
      e=dx-dy/2;
      while(y1<=y2)
      {
       Draw_Dot(x1,y1);
       if(e>0){x1+=1;e-=dy;} 
       y1+=1;
       e+=dx;
      }
     }
    }
    else     // dy<0
    {
     dy=-dy;   // dy=abs(dy)
  
     if(dx>=dy) // 8/8 octant
     {
      e=dy-dx/2;
      while(x1<=x2)
      {
       Draw_Dot(x1,y1);
       if(e>0){y1-=1;e-=dx;} 
       x1+=1;
       e+=dy;
      }
     }
     else  // 7/8 octant
     {
      e=dx-dy/2;
      while(y1>=y2)
      {
       Draw_Dot(x1,y1);
       if(e>0){x1+=1;e-=dy;} 
       y1-=1;
       e+=dx;
      }
     }
    } 
   }
   else //dx<0
   {
    dx=-dx;  //dx=abs(dx)
    if(dy >= 0) // dy>=0
    {
     if(dx>=dy) // 4/8 octant
     {
      e=dy-dx/2;
      while(x1>=x2)
      {
       Draw_Dot(x1,y1);
       if(e>0){y1+=1;e-=dx;} 
       x1-=1;
       e+=dy;
      }
     }
     else  // 3/8 octant
     {
      e=dx-dy/2;
      while(y1<=y2)
      {
       Draw_Dot(x1,y1);
       if(e>0){x1-=1;e-=dy;} 
       y1+=1;
       e+=dx;
      }
     }
    }
    else     // dy<0
    {
     dy=-dy;   // dy=abs(dy)
  
     if(dx>=dy) // 5/8 octant
     {
      e=dy-dx/2;
      while(x1>=x2)
      {
       Draw_Dot(x1,y1);
       if(e>0){y1-=1;e-=dx;} 
       x1-=1;
       e+=dy;
      }
     }
     else  // 6/8 octant
     {
      e=dx-dy/2;
      while(y1>=y2)
      {
       Draw_Dot(x1,y1);
       if(e>0){x1-=1;e-=dy;} 
       y1-=1;
       e+=dx;
      }
     }
    } 
   }

}

void disNum(unsigned char num,unsigned char addr)
{
 
  Write_Cmd(addr);
  Write_Data(num);  
}
