#include <MSP430F149.h>
#include "math.h"
#include "com.h"
#include "12864.h"


#define LCD_DataIn    P4DIR=0x00    //���ݿڷ�������Ϊ����
#define LCD_DataOut   P4DIR=0xff    //���ݿڷ�������Ϊ���
#define LCD2MCU_Data  P4IN
#define MCU2LCD_Data  P4OUT
#define LCD_CMDOut    P5DIR|=0xff    //P3�ڵĵ���λ����Ϊ���
#define LCD_RS_H      P5OUT|=BIT5      //P3.0
#define LCD_RS_L      P5OUT&=~BIT5   //P3.0
#define LCD_RW_H      P5OUT|=BIT6   //P3.1
#define LCD_RW_L      P5OUT&=~BIT6     //P3.1
#define LCD_EN_H      P5OUT|=BIT7   //P3.2
#define LCD_EN_L      P5OUT&=~BIT7    //P3.2



/*******************************************
�������ƣ�Delay_1ms
��    �ܣ���ʱԼ1ms��ʱ��
��    ������
����ֵ  ����
********************************************/
static void Delay_1ms(void)
{
	uchar i;
    
	for(i = 150;i > 0;i--)  _NOP();
} 
/*******************************************
�������ƣ�Delay_Nms
��    �ܣ���ʱN��1ms��ʱ��
��    ����n--��ʱ����
����ֵ  ����
********************************************/
static void Delay_Nms(uint n)
{
    uint i;
    
    for(i = n;i > 0;i--)    Delay_1ms();
}
/*******************************************
��������:Clear_GDRAM
��    ��:���Һ��GDRAM�е��������
��    ��:��
����ֵ  :��
********************************************/
void Clear_GDRAM(void)
{
    uchar i,j,k;
     
    Write_Cmd(0x34);        //����չָ�
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
    Write_Cmd(0x30);        //�ص�����ָ�
}
/*******************************************
�������ƣ�Write_Cmd
��    �ܣ���Һ����д��������
��    ����cmd--��������
����ֵ  ����
********************************************/
void Write_Cmd(uchar cmd)
{
    uchar lcdtemp = 0;
			
    LCD_RS_L;
    LCD_RW_H;
    LCD_DataIn;  
    do                       //��æ
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
�������ƣ�Write_Data
��    �ܣ���Һ����д��ʾ����
��    ����dat--��ʾ����
����ֵ  ����
********************************************/
void  Write_Data(uchar dat)
{
    uchar lcdtemp = 0;   
        
    LCD_RS_L;
    LCD_RW_H;  
    LCD_DataIn;   
    do                       //��æ
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
�������ƣ�Ini_Lcd
��    �ܣ���ʼ��Һ��ģ��
��    ������
����ֵ  ����
********************************************/
void Ini_Lcd(void)
{                  
    LCD_CMDOut;    //Һ�����ƶ˿�����Ϊ���
    
    Delay_Nms(500);
    Write_Cmd(0x30);   //����ָ�
    Delay_1ms();
    Write_Cmd(0x02);   // ��ַ��λ
    Delay_1ms();
	Write_Cmd(0x0c);   //������ʾ��,�α�ر�
    Delay_1ms();
	Write_Cmd(0x01);   //�����ʾ
    Delay_1ms();
	Write_Cmd(0x06);   //�α�����
    Delay_1ms();
	Write_Cmd(0x80);   //�趨��ʾ����ʼ��ַ
        
    Clear_GDRAM();
}
/*******************************************
�������ƣ�Disp_HZ
��    �ܣ�����Һ����ʾ����
��    ����addr--��ʾλ�õ��׵�ַ
          pt--ָ����ʾ���ݵ�ָ��
          num--��ʾ�ַ�����
����ֵ  ����
********************************************/
void Disp_HZ(uchar addr,const uchar * pt,uchar num)
{
    uchar i;
		
    Write_Cmd(addr); 
      for(i = 0;i < (num*2);i++) 
      Write_Data(*(pt++)); 
} 

/*******************************************
�������ƣ�Read_Data
��    �ܣ���ȡҺ���е�����
��    ������
����ֵ  ��temp����ȡ������ֵ
********************************************/
uchar Read_Data()
{
  uchar temp;   
  LCD_DataIn;
  LCD_RW_H; //������,�ߵ�ƽ��Ч
  LCD_EN_L;
  LCD_RS_H;     //�����ݣ��ߵ�ƽ��Ч
  delay_us(15); //��ʱ��Сֵ1500ns 
  _NOP();
  _NOP();
 
  LCD_EN_H; //lcdenһ�����ߣ����ݿ�ʼ����
  temp=LCD2MCU_Data;//�������ݿڵ�ֵ       
  delay_us(15); //�����ݳ���ʱ����Сֵ1500ns
  _NOP();
  _NOP();
   _NOP();
    _NOP();
     _NOP();
 
  LCD_EN_L; //�ٴ��õͣ�׼����һ������
  LCD_DataOut;
  return temp;  //�����������ݷ���
}

/*******************************************
��������: Lcd_set_dot
��    ��:ST7920���Ƶ�12864����λ�ô�㺯��
��    ��:uchar x,uchar y�������,��Χx[0:127],y[0:63]
����ֵ  :��
˵��������ϵʾ��  
      -----0---x---127----------
     |0
     |y
     |63
********************************************/
void Draw_Dot(uchar x,uchar y)
{
 // Write_Cmd(0x00);
  uchar x_byte,x_bit;   //ȷ��dot�ں����ϵ��ĸ��ֽ��ڵ��ĸ�λ
  uchar y_byte,y_bit;   //ȷ��dot�������ϵ��ĸ��ֽ��ڵ��ĸ�λ
  uchar TempH,TempL;    //������Ŷ�������ԭʼ���ݵĸ�8λ�͵�8λ��
            //Ŀ����Ϊ�˱����µ����ݵ�д��ȥ�󸲸ǵ�ǰ��ĵ�
 
  x=x&0x7f;     //���ƺ��������ڷ�Χ��Ϊ0-127
  y=y&0x3f;     //�������������ڷ�Χ��Ϊ0-31(�ϰ���)  32-63(�°���)
 
  x_byte=x/16;      //ȷ����dot����һ���ֽ�  
  x_bit=x%16;       //ȷ����dot����һ����
  y_byte=y/32;      //ȷ����dot���Ǹ�����,0:�ϰ��� 1:�°��� 
  y_bit=y%32;       //ȷ����dot�ڵڼ���(��Ϊ�����°���Ҳ�Ǵ�0�п�ʼ��31��)
     
  Write_Cmd(0x34);          //����չָ�,�رջ�������ģʽ����ͼ��ʾ�ر�
  Write_Cmd(0x80+y_bit);        //�Ƚ����������д��ָ����
  Write_Cmd(0x80+x_byte+8*y_byte);  //�ٽ���������д��ָ��λ��,��y_byteΪ1,��˵��������31��(��0������)
                    //���轫ACָ��д��0x88����ST7920�����µĵ�3��
  
  Read_Data();               //�����趨AC��ַָ�����Ҫ��ȡ������ҪԤ��dummy readһ�Σ��Ż������ȷ����
                 //��2�ζ�ȡʱ����Ҫdummy read,�����������趨ACλ�ò����ٴ�dummy read
 
  TempH=Read_Data();            //��ȡ��8λ����
  TempL=Read_Data();            //��ȡ��8λ����                                           
     
  Write_Cmd(0x80+y_bit);        //��������ı�ACλ�ã�������Ҫ����д��1��
  Write_Cmd(0x80+x_byte+8*y_byte);  
 
  if(x_bit<8)
  {
    Write_Data(TempH | (0x01<<(7-x_bit))); //���Ҫд��dot�ڸ�8λ,������д����or������������ݸ���ԭ������
    Write_Data(TempL);          //ԭ�ȵĵ�8λ�����ճ�д��
  }
  else
  {
    Write_Data(TempH);                      //���Ҫд��dot�ڵ�8λ,ԭ�ȵĸ�8λ�����ճ�д��
    Write_Data(TempL | (0x01<<(15-x_bit))); //��8λ������д����or������������ݸ���ԭ������
  }
    
  
  Write_Cmd(0x36);      //����ͼ��ʾ                                 
  Write_Cmd(0x30);      //�ص�GDRAM��ʾģʽ
}
/*******************************************
��������: sin_wave_y
��    ��:LCD�����Ҳ�
��    ��:t��������
����ֵ  :y�������� 
********************************************/
uchar sin_wave_y(uchar t,uchar w)
{      
 //unsigned int t;
 float y;
 
 uchar high = 30; //��ߵ�������Զ��(���½�)�Ĵ�ֱ����-2

 //for(t=0;t<127;t++)
 //{
    //Draw_Dot(t,31);
    //y=(high/2)*(1-sin(2*pi*t/f)); //�����y������
    y=(high/2)*(1-sin(2*PI*t/20)); //�����y������
    
    //Draw_Dot(t,61 -(uchar)y); 
 //}
    return 62-(uchar)y;
}
/*******************************************
��������: Draw_Line
��    ��:LCD��ֱ��
��    ��:uchar x1,uchar y1,uchar x2,uchar y2
����ֵ  :�� 
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
