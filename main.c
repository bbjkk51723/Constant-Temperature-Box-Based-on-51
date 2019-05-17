#include<reg51.h>
#include<intrins.h>
#include"LCD1602.h"   //LCD1602液晶头文件
#include"ds18b20.h"     ///ds18b20头文件
#include "eeprom52.h" 

#define uint unsigned int
#define uchar unsigned char


uchar set_flag;  //设置标志

uchar alarmMax_flag;alarmMin_flag;  //报警标志
uchar beep_flag;   //蜂鸣标志     
uchar j=0; 
uchar alarm_flag;

sbit beep=P3^6;  //蜂鸣器
sbit SET_key=P1^5;  //设置键
sbit ADD_key=P1^6;	//加键
sbit DEC_key=P1^7;	//减键
sbit Feng=P3^4;//风扇继电器
sbit Jiare=P3^5; //加热继电器
uchar ds18b20RT[7] ;  //十位 个位 小数点 十分位 百分位
uchar wendu_s[3];
uchar wendu_x[3];
uchar ds18b20[1];
uchar wendus[1];
uchar wendux[1];



void jiemian();
void jiemian1();
void key();
void wendu_ds();



void delay_ms(uchar z)
{
	uchar i,j;
		for(i=z;i>0;i--)
			for(j=120;j>0;j--);
}



void wendu_ds() //显示格式化的温度上限和下限 用于显示
{
  read_eeprom();
	wendu_s[0]=wen_ds/10+0x30;
	wendu_s[1]=wen_ds%10+0x30;
	wendu_s[2]='\0';
	wendu_x[0]=wen_dx/10+0x30;
	wendu_x[1]=wen_dx%10+0x30;
	wendu_x[2]='\0';
	wendus[0]=wen_ds;
	wendux[0]=wen_dx;
	LCD1602_Print(5,1,&wendu_s); 
	LCD1602_Print(13,1,&wendu_x); 
}

void Ds18b20Read(void)    //显示温度
{
		ReadTemperature();
    ds18b20RT[0]=sdata/10+0x30;
	  ds18b20RT[1]=sdata%10+0x30;
	  ds18b20RT[2]='.';
	  ds18b20RT[3]=xiaoshu1+0x30;
	  ds18b20RT[4]=xiaoshu2+0x30;
	  ds18b20RT[5]='C';
	  ds18b20RT[6]='\0';
	  ds18b20[0]=sdata;
    LCD1602_Print(10,0,&ds18b20RT);
}


void jiemian(void)
{
    GotoXY(0,0);    //设置显示数据地址  列位，行 
	  Print("RealTimeT:      ");
	  GotoXY(0,1);    //设置显示数据地址  列位，行
	  Print("Max:    Min:    ");

}

void jiemian1(void)
{
        GotoXY(0,0);    //设置显示数据地址  列位，行 
				Print("SET Alarm Value ");	  //PM2.5空气质量指数
				GotoXY(0,1);    //设置显示数据地址  列位，行
	  Print("Max:    Min:    ");


}

void key()
{
 if(!SET_key)   
 {
 delay_ms(20);
	 if(!SET_key)
	 {
		 set_flag++;
		 switch(set_flag)
		 {
			 case 1:
				 jiemian1();
							 wendu_s[0]=wen_ds/10+0x30;
	             wendu_s[1]=wen_ds%10+0x30;
	             wendu_s[2]='\0';
		          LCD1602_Print(5,1,&wendu_s);
							wendu_s[0]=wen_dx/10+0x30;
							wendu_s[1]=wen_dx%10+0x30;
							wendu_s[2]='\0';
							LCD1602_Print(13,1,&wendu_x);
							LCD1602_SetDisplay(LCD1602_open|LCD1602_cursor|LCD1602_flash);
							GotoXY(6,1);
			 break;
			 case 2:GotoXY(14,1);
			 break;
			 default : set_flag=0;
			          write_eeprom();
								jiemian();  
			          wendu_ds();
			          Ds18b20Read();
			          LCD1602_SetDisplay(LCD1602_open|LCD1602_NO_cursor|LCD1602_NO_flash);
		   break;	 
		 } 
	 }
	 while(!SET_key);
 }
 
 
 if(!ADD_key)
 {
   delay_ms(20);
	 if(!ADD_key&&set_flag!=0)
	 {
	 switch(set_flag)
	 {
		 case 1: wen_ds++;
		 if(wen_ds>99)
			 wen_ds=0;
		 if(wen_ds<=wen_dx)
			 wen_ds=wen_dx+1;
		   wendu_s[0]=wen_ds/10+0x30;
	     wendu_s[1]=wen_ds%10+0x30;
	     wendu_s[2]='\0';
		   LCD1602_Print(5,1,&wendu_s);
			 GotoXY(6,1);
			break;		
		 case 2: wen_dx++;
		 if(wen_dx>99)
			 wen_dx=0;
		 if(wen_ds<=wen_dx)
			 wen_dx=wen_ds-1;
		   wendu_x[0]=wen_dx/10+0x30;
	     wendu_x[1]=wen_dx%10+0x30;
	     wendu_x[2]='\0';
		   LCD1602_Print(13,1,&wendu_x);
			 GotoXY(14,1);
			break;		
		 }
	 
	 }
		 while(!ADD_key); 
 
 
 } 
  if(!DEC_key)
	{
		delay_ms(20);
	if(!DEC_key&&set_flag!=0)
	{
	switch(set_flag)
   {
	 case 1: wen_ds--;
		 if(wen_ds>99)
			 wen_ds=0;
		 if(wen_ds<=wen_dx)
			 wen_ds=wen_dx+1;
		   wendu_s[0]=wen_ds/10+0x30;
	     wendu_s[1]=wen_ds%10+0x30;
	     wendu_s[2]='\0';
		   LCD1602_Print(5,1,&wendu_s);
			 GotoXY(6,1);
			break;		
		 case 2: wen_dx--;
		 if(wen_dx>99)
			 wen_dx=0;
		 if(wen_ds<=wen_dx)
			 wen_dx=wen_ds-1;
		   wendu_x[0]=wen_dx/10+0x30;
	     wendu_x[1]=wen_dx%10+0x30;
	     wendu_x[2]='\0';
		   LCD1602_Print(13,1,&wendu_x);
			 GotoXY(14,1);
			break;		
	 
	 
	 while(!DEC_key); 
	 }
		
	}
	
	}
 
 }

void Timer0Init(void)		//100微秒@11.0592MHz
{
	
	TMOD=0x01;	//设置定时器模式
	TL0 = 0x3c;		//设置定时初值
	TH0 = 0xb0;		//设置定时初值
	TR0 = 0;		//关闭定时器0
}

 
 
void timer0(void) interrupt 1
{

  TL0 = 0xcd;
	TH0 = 0xf8;		
    j++;
    if(j==10)
  {
    j=0;
    beep_flag=~beep_flag;
  }
}	
 
 
void main()
	{
	
		LCD1602_init();  //LCD初始化
		init_eeprom();    //初始化数据 
		read_eeprom();    //将保存到EEPROM中的数据读出
		jiemian();         //显示温度
		Ds18b20Read();
		wendu_ds();       
		set_flag=0;    //设置键标志位清零
    alarm_flag=0;  //报警标志标志位清零
    beep_flag=1;   //蜂鸣标志标志位清零
		alarmMax_flag=0;
		alarmMin_flag=0;
		beep=1;       //关蜂鸣器
		Feng=1;       //关风扇
		Jiare=1;      //关加热

	  Timer0Init(); //定时器0初始化
		
		EA=1;
		ET0=1;
		
		





		
		while(1)
			{
		 
			if(set_flag==0)
			{					
				Ds18b20Read();
				wendu_ds(); 
			
			}
				
			key();
			
			if(set_flag==0)
			{
				if(wendus[0]<ds18b20[0])
				{
					TR0=1;
					alarmMax_flag=1;
					
				}else{		
				alarmMax_flag=0;
				}
				
				if(wendux[0]>ds18b20[0])
				{
					TR0=1;
					alarmMin_flag=1;
				
				}else{			
				alarmMin_flag=0;								
				}
				
				if(alarmMax_flag==0&&alarmMin_flag==0)
				{
					TR0=0;
					beep=1;       //关蜂鸣器
		      Feng=1;       //关风扇
		      Jiare=1;      //关加热

				}
				
				if(alarmMax_flag==1)
				{
					Feng=0;   //开风扇
					
				}
			
			if(alarmMin_flag==1)
				{
					Jiare=0;   //开加热
					
				}
			
			
			
			if(TR0==1)
       {
			  if(beep_flag==1){	
					beep=0;
				}else{
         beep=1;
				}  
			 
			 }	
			
			
			}
				
		
		}


 }







