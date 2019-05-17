/***************************************************************************
   File Name:       LCD1602.h
   Author:          xin xing guang dian
   Created:         2015/10/10
   Modified:		NO
   Revision: 		1.0  
***************************************************************************/

#ifndef _LCD1602_h
#define _LCD1602_h

#include <intrins.h>
#define delayNOP(); {_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();};	//nop  单时钟周期为0.083333us 
//Port Definitions**********************************************************
sbit LcdRs		= P2^5;  //LCD寄存器选择  1 选数据寄存器  0 选指令寄存器   27
sbit LcdRw		= P2^6;  //LCD读/写操作  1 把LCM数据读到单片机  0 把单片机数据写入LCM  26
sbit LcdEn  	= P2^7	;          //LCD使能端	   25
sfr  DBPort 	= 0x80;		//P0=0x80,P1=0x90,P2=0xA0,P3=0xB0.数据端口  sfr程序状态字寄存器
 
/*******1ms基准延时程序*******************/
void Delay1602ms(unsigned int t)    //1ms延时程序  11.0592MHz 单周期， 1时，取285us
{
	unsigned char j;
	unsigned int i;
	for(i=t;i>0;i--)
	{
		for(j=125;j>0;j--);
	}
}
/****************************************/
/****内部等待函数(判断忙碌标志位BF）BF由数据口D7表示***********************/
unsigned char LCD1602_Wait(void)
{
	LcdRs=0;
	LcdRw=1;
	Delay1602ms(1);
	LcdEn=1;
	Delay1602ms(5);
//while(DBPort&0x80);//在用Proteus仿真时，注意用屏蔽此语句，在调用GotoXY()时，会进入死循环，
						 //可能在写该控制字时，该模块没有返回写入完备命令，即DBPort&0x80==0x80
						 //实际硬件时打开此语句
delayNOP(); //延时1us，跳过读忙检测 
delayNOP(); //延时1us，跳过读忙检测 
delayNOP(); //延时1us，跳过读忙检测 
delayNOP(); //延时1us，跳过读忙检测 
delayNOP(); //延时1us，跳过读忙检测 
delayNOP(); //延时1us，跳过读忙检测 
delayNOP(); //延时1us，跳过读忙检测 
delayNOP(); //延时1us，跳过读忙检测 
delayNOP(); //延时1us，跳过读忙检测 
delayNOP(); //延时1us，跳过读忙检测 
	LcdEn=0;
	return DBPort;		
}
/**************************************************************************/
//向LCD1602写入命令或数据************************************************************
#define LCD1602_command	 0      // Command指令
#define LCD1602_data	 1      // Data数据
#define LCD1602_clear	0x01      // 清屏指令
#define LCD1602_return  0x02      // 光标返回原点
void LCD1602_Write(bit style, unsigned char input)
{
	LcdEn=0;
	LcdRs=style;
	LcdRw=0;
	Delay1602ms(1);
	DBPort=input;
	Delay1602ms(1);
	LcdEn=1;
	Delay1602ms(1);		
	LcdEn=0;
	Delay1602ms(1);
//	LCD1602_Wait();	//忙检测 
}

//设置显示模式************************************************************
#define LCD1602_open			0x04    //显示器开
#define LCD1602_close			0x00    //显示器关	  

#define LCD1602_cursor			0x02 	//显示光标
#define LCD1602_NO_cursor		0x00    //无光标		     

#define LCD1602_flash			0x01    //光标闪动
#define LCD1602_NO_flash		0x00    //光标不闪动

void LCD1602_SetDisplay(unsigned char DisplayMode)
{
	LCD1602_Write(LCD1602_command, 0x08|DisplayMode);	
}

//设置输入模式************************************************************
#define LCD1602_AC_UP			0x02      //地址计数器AC加1
#define LCD1602_AC_DOWN			0x00      // default

#define LCD1602_MOVE			0x01      // 画面可平移
#define LCD1602_NO_MOVE			0x00      //default默认画面不移动

void LCD1602_SetInput(unsigned char InputMode)
{
	LCD1602_Write(LCD1602_command, 0x04|InputMode);
}

/******移动光标或屏幕*****************************************************

#define LCD1602_cursor		0x02      
#define LCD1602_screen		0x08
#define LCD1602_left		0x00
#define LCD1602_right		0x04
void LCD1602_Move(unsigned char object, unsigned char direction)
{
	if(object==LCD1602_cursor)
		LCD1602_Write(LCD1602_command,0x10|direction);
	if(object==LCD1602_screen)
		LCD1602_Write(LCD1602_command,0x18|direction);
}
/*************************************************************************/
//初始化LCD1602************************************************************
void LCD1602_init()
{   
	LcdEn=0;
	Delay1602ms(52);//延时约15ms   52
	LCD1602_Write(LCD1602_command,0x38); //写指令38H ,不检测忙
	Delay1602ms(17);//延时约5ms	   17
	LCD1602_Write(LCD1602_command,0x38); //写指令38H ,不检测忙
	Delay1602ms(17);//延时约5ms	   17
	LCD1602_Write(LCD1602_command,0x38); //写指令38H ,不检测忙,以后每次读写都需要忙检测
	LCD1602_SetDisplay(LCD1602_open|LCD1602_NO_cursor);    //设置显示模式  开启显示, 无光标
	LCD1602_Wait();	//忙检测 
	LCD1602_Write(LCD1602_command,LCD1602_clear);   //清屏
	LCD1602_Wait();	//忙检测 
	LCD1602_SetInput(LCD1602_AC_UP|LCD1602_NO_MOVE);//设置输入模式  AC递增, 画面不动
	Delay1602ms(100);
}

//************************************************************************
void GotoXY(unsigned char x, unsigned char y)    //设置显示数据地址
{
	if(y==0)
		LCD1602_Write(LCD1602_command,0x80|x);          //第一行地址0x80+x
		LCD1602_Wait();	//忙检测 
	if(y==1)
		LCD1602_Write(LCD1602_command,0x80|(x-0x40));   // 第二行地址0x80+(0x40+x)==0xc0+x
		LCD1602_Wait();	//忙检测 
}
void Print(unsigned char *str)
{
	while(*str!='\0')        // \0  结束标志
	{
		LCD1602_Write(LCD1602_data,*str);
		LCD1602_Wait();	//忙检测 
		str++;
	}
}

void LCD1602_Print(unsigned char x, unsigned char y, unsigned char *str)
{
  GotoXY(x,y);
  Print(str);
}


/*
void LCD1602_LoadChar(unsigned char user[8], unsigned char place)
{
	unsigned char i;
	LCD1602_Write(LCD1602_command,0x40|(place*8));
	for(i=0; i<8; i++)
		LCD1602_Write(LCD1602_data,user[i]);
}
*/
//************************************************************************
#endif
