#ifndef _COM_H
#define _COM_H

#define CPU_F ((double)8000000)   
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))  
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0)) 
#define PI 3.1415926

typedef unsigned char uchar;
typedef unsigned int uint;

extern void Init_CLK(void);


#endif