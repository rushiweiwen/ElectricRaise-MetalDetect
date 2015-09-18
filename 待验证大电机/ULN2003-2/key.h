

void init_key()
{
    P6SEL = 0x00;
    P6DIR = 0xf0;
    P6OUT |= 0xf0;
}

void delay()
{
    uint tmp;
     
    for(tmp = 12000;tmp > 0;tmp--);
}

void key()
{
    uint8 tem;
    
    P6OUT = 0x70;      // P1.5����ߵ�ƽ  ��1��
    tem = 0x0f & P6IN;
    if(tem != 0x0f)
    {
        delay_ms(5);
        if(tem != 0x0f)
        {
            tem = 0x0f & P6IN;
            switch(tem)       // A 3 2 1
            {
                case 0x0e: 
                  {   stop_cw ^= 0x01;if(stop_cw) timer_a=0; num=0;}   break;
                
                case 0x0d: x_cw ^= 0x01;   break;
                case 0x0b: speed-=100;TBCCR0 = speed;   break;
                case 0x07: speed+=100;TBCCR0 = speed;   break;
            }
        }
        while(tem != 0x0f) {tem = 0x0f & P6IN;}
    }   
    
    P6OUT = 0xb0;      // P1.6����ߵ�ƽ  ��2��
    tem = 0x0f & P6IN;
    if(tem != 0x0f)
    {
        delay_ms(5);
        if(tem != 0x0f)
        {
            tem = 0x0f & P6IN;
            switch(tem)       // B 6 5 4
            {
                case 0x0e:    break;
                case 0x0d: y_cw ^= 0x01;   break;
                case 0x0b: flag_y ^= 0x01;  break;
                case 0x07: flag_x ^= 0x01;  break;
            }
        }
        while(tem != 0x0f) {tem = 0x0f & P6IN;}
    }   
    
    delay_ms(5);      // P1.5����ߵ�ƽ  ��3��
    tem = 0x0f & P6IN;
    if(tem != 0x0f)
    {
        delay();
        if(tem != 0x0f)
        {
            tem = 0x0f & P6IN;
            switch(tem)       // C 9 8 7
            {
                case 0x0e:    break;
                case 0x0d:    break;
                case 0x0b:    break;
                case 0x07:    break;
            }
        }
        while(tem != 0x0f) {tem = 0x0f & P6IN;}
    }   
    
    P6OUT = 0xe0;      // P1.4����ߵ�ƽ   ��4��
    tem = 0x0f & P6IN;
    if(tem != 0x0f)
    {
        delay_ms(5);
        if(tem != 0x0f)
        {
            tem = 0x0f & P6IN;
            switch(tem)       // D # 0 *
            {
                case 0x0e:    break;
                case 0x0d:    break;
                case 0x0b:    break;
                case 0x07:    break;
            }
        }
        while(tem != 0x0f) {tem = 0x0f & P6IN;}
    }
    

}

