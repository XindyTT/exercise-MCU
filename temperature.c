#include<reg51.h>
#include<string.h>
#include<ds18b20.h>
#include<lcd1602.h>
#include<transfer.h>

#define OSC 12000000	    //晶振频率
#define BAUDRATE 9600	    //波特率


/*************************串口通信*****************************/
void init_cxk()
{
	TMOD=0X20;  		  //T1工作在方式2，作为波特率发生器
	SCON=0X50;			  //串口方式1，REN=1(允许接收)
	PCON=0X80;			  //SMODE=1
	TL1=256-(OSC/12/16/BAUDRATE);
	TH1=256-(OSC/12/16/BAUDRATE);//设置定时器初值
	TR1=1;				  //启动定时器，输出波特率时钟
	RI=0;				  
	TI=0;				  //确保程序开始时RI和TI无效
	EA=1;
	ES=1;				  //开串行口中断
}

/*********************串行口中断处理子程序*********************/
void cxk() interrupt 4
{
	uchar c;		      
	if(RI==1)             //接收中断
	{
		RI=0;
		c=SBUF;
		switch(c)
		{	
			case 0x30:show0();break;
			case 0x31:show1();break;
			case 0x32:show2();break;
			case 0x33:show3();break;
			default: show4();break;
		}
	}
	else TI=0;
}

void displaytem()		     //温度信息显示函数：TEMP=温度℃
{
	wc51r(0x01);
	wc51r(0x80);wc51ddr(0x48);wc51ddr(0x3d);		                                //H=
	wc51ddr(high_test_temp/10+0x30);wc51ddr(high_test_temp%10+0x30);wc51ddr(0x00);
	wc51r(0x86);
	wc51ddr(0x4c);wc51ddr(0x3d);		                                            //L=
	wc51ddr(low_test_temp/10+0x30);wc51ddr(low_test_temp%10+0x30);wc51ddr(0x00);
    wc51r(0xc0);
    wc51ddr(0x54);wc51ddr(0x45); wc51ddr(0x4d); wc51ddr(0x50);wc51ddr(0x3d);	    //显示TEMP=
    if(flag==0)
        wc51ddr(0x2b);
    else 
   	    wc51ddr(0x2d);   //显示温度正负号
    if(bai!=0)		        //百位为0不显示
	{ 
		wc51ddr(bai+0x30);
	}
	wc51ddr(shi+0x30);	//显示温度的十位数
    wc51ddr(ge+0x30);    //显示温度的个位数
    wc51ddr(0x2e);	    //显示小数点
    wc51ddr(xiaoshu+0x30);
	wc51ddr(0x00);	    //自定义的摄氏度符号显示
}

void displaynew0()    //刷新当前的温度限度数值
{
	test_key();
	displaytem();
}
 
void displaynew1()	   //按修改极限数值刷新数值
{ 	
	if((P1 & 0xf0)!=0xf0)
	{	
		displaynew0();
	}
}

	
 
/*********************keyscan************************************/
void delay1ms(int ms)
{
	int i;
	while(ms--)
	for(i=0;i<100;i++);
}
void keyscan(void)
{
	uchar ScanCode;
	P2=0xfe;   //仅扫描第一行0
    ScanCode=P2 & 0xf0;
	if(ScanCode != 0xf0)
	{	
		bj=1;
		delay1ms(20); 
		bj=0;
	}   
	ScanCode=P2 & 0xf0;
	switch (ScanCode)
	{
	case 0xe0: displayname();break;
	case 0xd0: displaynew0();break;
	default:break;
	}
}
 
/*************************主函数*******************************/
void main()
{
	init_cxk();
    init_1602();
	init_18b20();
	EA=1;
	ES=1;				  
    while(1)
    { 
		bj=1;			  //在新的一次测温开始，蜂鸣器无效
		flag=0;			  //在新的一次测温开始，flag清零
		
	  	read_temp();	  //读DS18B20温度
	    convert_temp();	  //转换温度
		test_temp();	  //检测温度
		keyscan();		  //检测按键，显示温度
		displaynew1();
   	}					  //为了实时检测温度，因此每次测温结束不用延时
}
