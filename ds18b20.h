#ifndef _ds18b20_h
#define _ds18b20_h
#define uchar unsigned char

sbit DQ=P3^2; 				//数据传输线接单片机的相应的引脚 
unsigned char tempL=0; 		//设全局变量
unsigned char tempH=0; 
unsigned int sdata;			//测量到的温度的整数部分
unsigned char xiaoshu1;		//小数第一位
unsigned char xiaoshu2;		//小数第二位
unsigned char xiaoshu;		//两位小数
bit fg=1;        			//温度正负标志

void delay(unsigned char i)
{
	for(i;i>0;i--);
}

void delay1(uchar i)
{
	uchar j,k; 
	for(j=i;j>0;j--)
		for(k=125;k>0;k--);
}

void Init_DS18B20(void) 
{
	unsigned char x=0;
	DQ=1; 					//DQ先置高 
	delay(8); 				//稍延时
	DQ=0; 					//发送复位脉冲 
	delay(80); 				//延时（>480us) 
	DQ=1; 					//拉高数据线 
	delay(5); 				//等待（15~60us) 
	x=DQ; 					//用X的值来判断初始化有没有成功，18B20存在的话X=0，否则X=1 
	delay(20); 
}

//读一个字节
ReadOneChar(void)  			//主机数据线先从高拉至低电平1us以上，再使数据线升为高电平，从而产生读信号
{
	unsigned char i=0; 		//每个读周期最短的持续时间为60us，各个读周期之间必须有1us以上的高电平恢复期
	unsigned char dat=0; 
	for (i=8;i>0;i--) 		//一个字节有8位 
	{
		DQ=1; 
		delay(1); 
		DQ=0;
		dat>>=1; 
		DQ=1; 
		if(DQ) 
		dat|=0x80; 
		delay(4);
	} 
	return(dat);
}

//写一个字节
void WriteOneChar(unsigned char dat) 
{ 
	unsigned char i=0; 		//数据线从高电平拉至低电平，产生写起始信号。15us之内将所需写的位送到数据线上，
	for(i=8;i>0;i--) 		//在15~60us之间对数据线进行采样，如果是高电平就写1，低写0发生。 
	{
		DQ=0; 				//在开始另一个写周期前必须有1us以上的高电平恢复期。 
		DQ=dat&0x01; 
		delay(5); 
		DQ=1; 
		dat>>=1;
	} 
	delay(4);
}

//读温度值（低位放tempL;高位放tempH;）
void ReadTemperature(void) 
{ 
	Init_DS18B20(); 					//初始化
	WriteOneChar(0xcc); 				//跳过读序列号的操作
	WriteOneChar(0x44); 				//启动温度转换
	delay(125); 						//转换需要一点时间，延时 
	Init_DS18B20(); 					//初始化
	WriteOneChar(0xcc); 				//跳过读序列号的操作 
	WriteOneChar(0xbe); 				//读温度寄存器（头两个值分别为温度的低位和高位） 
	tempL=ReadOneChar(); 				//读出温度的低位LSB
	tempH=ReadOneChar(); 				//读出温度的高位MSB	
	if(tempH>0x7f)      				//最高位为1时温度是负
	{
		tempL=~tempL;					//补码转换，取反加一
		tempH=~tempH+1;       
		fg=0;      						//读取温度为负时fg=0
	}
	sdata = tempL/16+tempH*16;      	//整数部分
	xiaoshu1 = (tempL&0x0f)*10/16; 		//小数第一位
	xiaoshu2 = (tempL&0x0f)*100/16%10;	//小数第二位
	xiaoshu=xiaoshu1*10+xiaoshu2; 		//小数两位
}

#endif