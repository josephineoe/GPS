#include <REG52.h>
#include "uart.c"


#define LCM_Data  P0		 
#define uchar unsigned char 
#define uint  unsigned int


sbit lcd1602_rs=P2^7;
sbit lcd1602_rw=P2^6;
sbit lcd1602_en=P2^5;

#define RsBuf_N		800
unsigned char xdata RsBuf[RsBuf_N];	
uchar code sousuo[]="GPS Searching...";
uint RsPoint;
bit dis_flag=0;
unsigned long weidunum=0;
unsigned long jingdunum=0;

void delay(uint xms)
{
	uint x,y;
	for(x=xms;x>0;x--)
	 for(y=10;y>0;y--);
}


void write_1602com(uchar com)
{
	lcd1602_rs=0;
	lcd1602_rw=0;
	P0=com;
	delay(1);
	lcd1602_en=1;
	delay(1);
	lcd1602_en=0;
}


void write_1602dat(uchar dat)
{
	lcd1602_rs=1;
	lcd1602_rw=0; 
	P0=dat;
	delay(1);
	lcd1602_en=1; 
	lcd1602_en=0; 
}


void lcd_init(void)
{
	uchar i;
	write_1602com(0x38);
	write_1602com(0x0c);
	write_1602com(0x06);
	write_1602com(0x01);
	delay(1000);
	write_1602com(0x80);
	for(i=0;i<16;i++)
	write_1602dat(sousuo[i]);
}

//  ͨѶ�жϽ��ճ���   �жϺ����޷���ֵ
void uart_rx(void)  interrupt 4	using 3	  
{
	EA=0; 
   	if((RsPoint<RsBuf_N)&&RI) 
    {
    	RI=0;
		if(SBUF=='$')
		RsPoint=0;
    	RsBuf[RsPoint++]=SBUF;
    }
	EA=1;
}

void main(void)
{
 	uint i;
	uchar shi;
 	lcd_init();
	Uart_init();
	for(i=0;i<RsBuf_N;i++)
	RsBuf[i]='0';
	while(1)
	{
		   if(RsBuf[0]=='$'&&RsBuf[1]=='G'&&RsBuf[2]=='P'&&RsBuf[3]=='G'&&RsBuf[4]=='G'&&RsBuf[5]=='A'&&RsBuf[44]!='0')
		   {
			if(RI==0&&((RsBuf[30]-48)*100+(RsBuf[31]-48)*10+(RsBuf[32]-48))>=0&&((RsBuf[30]-48)*100+(RsBuf[31]-48)*10+(RsBuf[32]-48))<=180)
			{
				TR0=1;
				if(dis_flag==0)
				{	
					write_1602com(0x80);
					write_1602dat('T');
					write_1602dat('i');
					write_1602dat('m');
					write_1602dat('e');
					write_1602dat(':');
					shi=((RsBuf[7]-0x30)*10+(RsBuf[8]-0x30)+8)%24;
					write_1602dat(shi/10+0x30);
					write_1602dat(shi%10+0x30);
					write_1602dat('-');
					write_1602dat(RsBuf[9]);
					write_1602dat(RsBuf[10]);
					write_1602dat('-');
					write_1602dat(RsBuf[11]);
					write_1602dat(RsBuf[12]);
					write_1602dat(' ');
					write_1602dat(' ');
					write_1602dat(' ');
		
					write_1602com(0xc0);
					write_1602dat('W');
					write_1602dat('X');
					write_1602dat(':');
					write_1602dat(RsBuf[46]);
					write_1602dat(RsBuf[47]);  
					write_1602dat(' ');
					write_1602dat(' ');

					write_1602com(0xc7);
					write_1602dat('J');
					write_1602dat('D');
					write_1602dat(':');
					write_1602dat(RsBuf[49]);
					write_1602dat(RsBuf[50]);
					write_1602dat(RsBuf[51]);
					write_1602dat(RsBuf[52]);
					write_1602dat(' ');
					write_1602dat(' ');
				}
				else
				{
					weidunum=	((RsBuf[17]-48)*10000000+(RsBuf[18]-48)*1000000)
								+(((((RsBuf[19]-48)*10)+(RsBuf[20]-48))*100000)/6)
								+(((((RsBuf[22]-48)*1000)+((RsBuf[23]-48)*100)+((RsBuf[24]-48)*10)+(RsBuf[25]-48))*10)/6);	 
					write_1602com(0x80);
					write_1602dat('W');
					write_1602dat('D');
					write_1602dat(':');
					write_1602dat(' ');
					write_1602dat(weidunum/10000000+0x30);
					write_1602dat(weidunum%10000000/1000000+0x30);
					write_1602dat('.');
					write_1602dat(weidunum%1000000/100000+0x30);
					write_1602dat(weidunum%100000/10000+0x30);
					write_1602dat(weidunum%10000/1000+0x30);
					write_1602dat(weidunum%1000/100+0x30);
					write_1602dat(weidunum%100/10+0x30);
					write_1602dat(weidunum%10+0x30);	
					write_1602dat(RsBuf[28]);
					write_1602dat(' ');
					write_1602dat(' ');
		
		
					jingdunum=	(((RsBuf[30]-48)*100000000)+((RsBuf[31]-48)*10000000)+((RsBuf[32]-48)*1000000))
								+(((((RsBuf[33]-48)*10)+(RsBuf[34]-48))*100000)/6)
								+(((((RsBuf[36]-48)*1000)+((RsBuf[37]-48)*100)+((RsBuf[38]-48)*10)+(RsBuf[39]-48))*10)/6);	   
					write_1602com(0xc0);
					write_1602dat('J');
					write_1602dat('D');
					write_1602dat(':');
					write_1602dat(jingdunum/100000000+0x30);
					write_1602dat(jingdunum%100000000/10000000+0x30);
					write_1602dat(jingdunum%10000000/1000000+0x30);
					write_1602dat('.');
					write_1602dat(jingdunum%1000000/100000+0x30);
					write_1602dat(jingdunum%100000/10000+0x30);
					write_1602dat(jingdunum%10000/1000+0x30);
					write_1602dat(jingdunum%1000/100+0x30);
					write_1602dat(jingdunum%100/10+0x30);
					write_1602dat(jingdunum%10+0x30);		
					write_1602dat(RsBuf[42]);
					write_1602dat(' ');
					write_1602dat(' ');
				}	
			}
		}
	}
}
void T0_time() interrupt 1
{
	uchar mm;
	TH0=0x4b;
	TL0=0xff;
	mm++;
	if(mm>=60)
	{
		mm=0;
		dis_flag=!dis_flag;
	}
}