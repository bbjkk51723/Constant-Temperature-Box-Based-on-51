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
#define delayNOP(); {_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();};	//nop  ��ʱ������Ϊ0.083333us 
//Port Definitions**********************************************************
sbit LcdRs		= P2^5;  //LCD�Ĵ���ѡ��  1 ѡ���ݼĴ���  0 ѡָ��Ĵ���   27
sbit LcdRw		= P2^6;  //LCD��/д����  1 ��LCM���ݶ�����Ƭ��  0 �ѵ�Ƭ������д��LCM  26
sbit LcdEn  	= P2^7	;          //LCDʹ�ܶ�	   25
sfr  DBPort 	= 0x80;		//P0=0x80,P1=0x90,P2=0xA0,P3=0xB0.���ݶ˿�  sfr����״̬�ּĴ���
 
/*******1ms��׼��ʱ����*******************/
void Delay1602ms(unsigned int t)    //1ms��ʱ����  11.0592MHz �����ڣ� 1ʱ��ȡ285us
{
	unsigned char j;
	unsigned int i;
	for(i=t;i>0;i--)
	{
		for(j=125;j>0;j--);
	}
}
/****************************************/
/****�ڲ��ȴ�����(�ж�æµ��־λBF��BF�����ݿ�D7��ʾ***********************/
unsigned char LCD1602_Wait(void)
{
	LcdRs=0;
	LcdRw=1;
	Delay1602ms(1);
	LcdEn=1;
	Delay1602ms(5);
//while(DBPort&0x80);//����Proteus����ʱ��ע�������δ���䣬�ڵ���GotoXY()ʱ���������ѭ����
						 //������д�ÿ�����ʱ����ģ��û�з���д���걸�����DBPort&0x80==0x80
						 //ʵ��Ӳ��ʱ�򿪴����
delayNOP(); //��ʱ1us��������æ��� 
delayNOP(); //��ʱ1us��������æ��� 
delayNOP(); //��ʱ1us��������æ��� 
delayNOP(); //��ʱ1us��������æ��� 
delayNOP(); //��ʱ1us��������æ��� 
delayNOP(); //��ʱ1us��������æ��� 
delayNOP(); //��ʱ1us��������æ��� 
delayNOP(); //��ʱ1us��������æ��� 
delayNOP(); //��ʱ1us��������æ��� 
delayNOP(); //��ʱ1us��������æ��� 
	LcdEn=0;
	return DBPort;		
}
/**************************************************************************/
//��LCD1602д�����������************************************************************
#define LCD1602_command	 0      // Commandָ��
#define LCD1602_data	 1      // Data����
#define LCD1602_clear	0x01      // ����ָ��
#define LCD1602_return  0x02      // ��귵��ԭ��
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
//	LCD1602_Wait();	//æ��� 
}

//������ʾģʽ************************************************************
#define LCD1602_open			0x04    //��ʾ����
#define LCD1602_close			0x00    //��ʾ����	  

#define LCD1602_cursor			0x02 	//��ʾ���
#define LCD1602_NO_cursor		0x00    //�޹��		     

#define LCD1602_flash			0x01    //�������
#define LCD1602_NO_flash		0x00    //��겻����

void LCD1602_SetDisplay(unsigned char DisplayMode)
{
	LCD1602_Write(LCD1602_command, 0x08|DisplayMode);	
}

//��������ģʽ************************************************************
#define LCD1602_AC_UP			0x02      //��ַ������AC��1
#define LCD1602_AC_DOWN			0x00      // default

#define LCD1602_MOVE			0x01      // �����ƽ��
#define LCD1602_NO_MOVE			0x00      //defaultĬ�ϻ��治�ƶ�

void LCD1602_SetInput(unsigned char InputMode)
{
	LCD1602_Write(LCD1602_command, 0x04|InputMode);
}

/******�ƶ�������Ļ*****************************************************

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
//��ʼ��LCD1602************************************************************
void LCD1602_init()
{   
	LcdEn=0;
	Delay1602ms(52);//��ʱԼ15ms   52
	LCD1602_Write(LCD1602_command,0x38); //дָ��38H ,�����æ
	Delay1602ms(17);//��ʱԼ5ms	   17
	LCD1602_Write(LCD1602_command,0x38); //дָ��38H ,�����æ
	Delay1602ms(17);//��ʱԼ5ms	   17
	LCD1602_Write(LCD1602_command,0x38); //дָ��38H ,�����æ,�Ժ�ÿ�ζ�д����Ҫæ���
	LCD1602_SetDisplay(LCD1602_open|LCD1602_NO_cursor);    //������ʾģʽ  ������ʾ, �޹��
	LCD1602_Wait();	//æ��� 
	LCD1602_Write(LCD1602_command,LCD1602_clear);   //����
	LCD1602_Wait();	//æ��� 
	LCD1602_SetInput(LCD1602_AC_UP|LCD1602_NO_MOVE);//��������ģʽ  AC����, ���治��
	Delay1602ms(100);
}

//************************************************************************
void GotoXY(unsigned char x, unsigned char y)    //������ʾ���ݵ�ַ
{
	if(y==0)
		LCD1602_Write(LCD1602_command,0x80|x);          //��һ�е�ַ0x80+x
		LCD1602_Wait();	//æ��� 
	if(y==1)
		LCD1602_Write(LCD1602_command,0x80|(x-0x40));   // �ڶ��е�ַ0x80+(0x40+x)==0xc0+x
		LCD1602_Wait();	//æ��� 
}
void Print(unsigned char *str)
{
	while(*str!='\0')        // \0  ������־
	{
		LCD1602_Write(LCD1602_data,*str);
		LCD1602_Wait();	//æ��� 
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
