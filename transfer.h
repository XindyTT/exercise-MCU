#include<reg51.h>
#include<string.h>

uchar code message0[]="Please choose your operation";
uchar code message1[]="1:get high_test_temp";
uchar code message2[]="2:get low_test_temp";
uchar code message3[]="3:get current temp";

void send_message0()
{
	uchar i;
	for(i=0;i<strlen(message0);i++)
	{
		SBUF=message0[i];
		while(TI!=1);
		TI=0;			
	}
	SBUF=0x0a;
}
void send_message1()
{
	uchar i;
	for(i=0;i<strlen(message1);i++)
	{
		SBUF=message1[i];
		while(TI!=1);
		TI=0;			
	}
	SBUF=0x0a;
}

void send_message2()
{
	uchar i;
	for(i=0;i<strlen(message2);i++)
	{
		SBUF=message2[i];
		while(TI!=1);
		TI=0;			
	}
	SBUF=0x0a;
}

void send_message3()
{
	uchar i;
	for(i=0;i<strlen(message3);i++)
	{
		SBUF=message3[i];
		while(TI!=1);
		TI=0;			
	}
	SBUF=0x0a;
}


void show0(void)
{
	send_message0();send_message1();
	send_message2();send_message3();SBUF=0x0a;
}

void show1(void)
{
	send_message1();
	SBUF=high_test_temp/10+0x30;while(TI!=1);TI=0; //高温度的十位
	SBUF=high_test_temp%10+0x30;while(TI!=1);TI=0; //高温度的个位
	SBUF='C';while(TI!=1);TI=0;	SBUF=0x0a;				 //温度单位
		//高温度
}

void show2()
{
     send_message2();
	 SBUF=low_test_temp/10+0x30;while(TI!=1);TI=0;	 //低温度的十位
	 SBUF=low_test_temp%10+0x30;while(TI!=1);TI=0;	 //低温度的个位
	 SBUF='C';while(TI!=1);TI=0;SBUF=0x0a;					 //温度单位
        //低温度
}

void show3()
{
	send_message3();
	SBUF='+';  
	SBUF=shi+0x30;			 //当前温度的十位
	SBUF=ge+0x30;				 //当前温度的个位
	SBUF='.';					 //小数点
	SBUF=xiaoshu+0x30;			 //当前温度的小数位
	SBUF='C';while(TI!=1);TI=0;SBUF=0x0a;					 //温度单位
     //当前温度
}

void show4()
{
	send_message0();send_message1();
	send_message2();send_message3();SBUF=0x0a;
}
