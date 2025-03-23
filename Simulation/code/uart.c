#ifndef _UART_C_
#define	 _UART_C_

#include "reg52.h"

#define uchar unsigned char
#define uint unsigned int


void Uart_init()
{
	
	TMOD= 0X21;		 
	TH1=0Xfd;
	TL1=0Xfd;	    //9600
	TH0=0x4b;
	TL0=0xff;
	TR0=0;
	ET0=1;
	TR1=1; 		      


	SM0=0;	   
	SM1=1; 	    


	REN=1; 		 	 


	ES=1; 	     
	EA=1;		 
	
}



#endif
												       



