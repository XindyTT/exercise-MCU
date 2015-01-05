#include<reg51.h>
#include<string.h>

#define uchar unsigned char
#define uint  unsigned int

sbit dq=P1^0;					//DS18B20端口
sbit bj=P1^1;					//声光报警控制端
sbit high_test_temp_up=P1^4;	//增加高温度报警限
sbit high_test_temp_down=P1^5;	//减小高温度报警限
sbit low_test_temp_up=P1^6;		//增加低温度报警限
sbit low_test_temp_down=P1^7;	//减小低温度报警限



uint temp;					    //存放读到的16位温度信息
uchar bai,shi,ge,xiaoshu,flag;	//温度值的各位数值和符号标志位
uchar high_temp,low_temp;		//读取DS18B20中温度信息高低字节
uchar high_test_temp=0x1c;	    //初始高温28度
uchar low_test_temp=0x08;       //初始低温8度

/*****************************DS18B20部分***********************************/
 void delay_18b20(uchar z)
{ 
    for(;z>0;z--);
}

uchar ow_reset()	         //DS18B20复位函数
{
	uchar reset;
	dq=1;				
	delay_18b20(1);	     //高电平稍作延时
	dq=0;				
	delay_18b20(50);	     //下拉数据线500us左右
	dq=1;
	delay_18b20(3);		 //释放数据线30us左右
	reset=dq;
	delay_18b20(25);	  
	return(reset);
}

void init_18b20()		     //DS18B20初始化函数
{
	ow_reset();			
}

void write_byte(uchar dat)    //向单总线上写入一个字节
{
	uchar i;
	for(i=0;i<8;i++)
	{
		dq=0;	  
		dq=dat&0x01;	     //提取出最低位（每次只能发一位）
		delay_18b20(5);	 //延时50us左右
		dq=1;			 //数据线拉高
		dat=dat>>1;		 //右移一位，准备发送下一位
	}
	delay_18b20(5);
}

uchar read_byte()	         //从单总线上读取一个字节
{
	uchar i, value=0;
	for(i=0;i<8;i++)
	{
		value=value>>1;
		dq=0;
		dq=1;
		delay_18b20(1);
		if(dq==1)
		value=value|0x80;	
		delay_18b20(6);	  //延时60us左右，等待DS18B20完成一个读时序
	}
	return(value);
}

void read_temp()		      //读DS18B20温度函数
{
    ow_reset();
	write_byte(0xcc);	      //跳过ROM
	write_byte(0x44);	      //启动温度转换
	ow_reset();
	write_byte(0xcc);	      //跳过ROM
	write_byte(0xbe);  	      //读内部RAM中内容
	low_temp=read_byte();     //读温度低字节
	high_temp=read_byte();    //读温度高字节
}

void convert_temp()		  //温度转换函数
{	 
	temp=high_temp*256+low_temp;
	if(high_temp>0xf8)      //判断温度是否为负数
	{
		flag=1;
		temp=(~temp)+1;
	}
	temp=temp*0.0625;	  //计算温度数值
	bai=temp/100;		      //计算温度百位数
	shi=temp/10%10;		  //计算温度十位数
	ge=temp%10;			  //计算温度个位数
	xiaoshu=(low_temp&0x0f)*10*0.0625;//温度低字节的低四位代表的是小数值（0.0625*16=1）
}

/*************************温度检测*******************************/
void test_temp()
{
    uchar temp,a,b;
	if(flag==1)
		a=-1;
	else a=1;
	temp=a*(bai*100+shi*10+ge);
	if(temp>=high_test_temp||temp<low_test_temp)
	{
		bj=0;
		for(a=0;a<200;a++)
			for(b=0;b<200;b++){}	
	}
							
}

/*************************按键检测*******************************/
void delay()
{
	uchar u,d;
	for(u=0;u<200;u++)
		for(d=0;d<200;d++)
		{}
}
void test_key()
{
	if(high_test_temp_up==0)
	{
		delay();
		high_test_temp++;
	}
	else if(high_test_temp_down==0)
	{
		delay();
		high_test_temp--;
	}
	else if(low_test_temp_up==0)
	{
		delay();
		low_test_temp++;
	}
	else if(low_test_temp_down==0)
	{
		delay();
		low_test_temp--;
	}
	
}	
