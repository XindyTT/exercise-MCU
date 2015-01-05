#include<reg51.h>
#include<string.h>

#define uchar unsigned char
#define uint  unsigned int

/*******************位定义***************************/
sbit rs=P3^2;					//1602的三个控制端
sbit rw=P3^3;
sbit e=P3^4;
sbit beiguang=P3^5;			//1602背光灯控制端
	

uchar code mytab0[8]={0x10,0x06,0x09,0x08,0x08,0x08,0x09,0x06};//自定义的摄氏度符号
uchar code name0[]={0x00,0x07,0x00,0x0F,0x01,0x01,0x01,0x07}; //自定义姓名中文显示
uchar code name1[]={0x02,0x1A,0x02,0x1F,0x12,0x1A,0x12,0x1B};  //武
uchar code name2[]={0x00,0x0F,0x08,0x0F,0x0A,0x0A,0x0A,0x12};
uchar code name3[]={0x10,0x1F,0x15,0x15,0x04,0x04,0x0A,0x11};  //欣
uchar code name4[]={0x02,0x04,0x08,0x1F,0x0A,0x04,0x0A,0x11};
uchar code name5[]={0x04,0x1F,0x04,0x1F,0x11,0x0E,0x04,0x0C};  //婷

/********************1602显示部分***************************/
void delay_1602()	      //1602的延时函数
{					  
	uchar y;
	for(y=0;y<0xff;y++){;}
}

void fbusy()	          //检查忙函数
{
	P0=0xff;
	rs=0;
	rw=1;
	e=1;
	e=0;
	while(P0&0x80)
	{
		e=0;
		e=1;
	}
	delay_1602();
}

void wc51r(uchar j)       //1602写命令函数
{
	fbusy(); 
	e=0;
    rs=0;
    rw=0;
	e=1;
    P0=j;
    e=0;
    delay_1602();
}

void wc51ddr(uchar j)     //1602写数据函数
{
	fbusy();
	e=0;
    rs=1;
    rw=0;
	e=1;
    P0=j;
    e=0;
    delay_1602();
}

void writecgram()	      //自定义符号写入1602中
{
	uchar i;
	wc51r(0x40);	      //设置字库命令，地址为0x00
	for(i=0;i<8;i++)
			wc51ddr(mytab0[i]);
	wc51r(0x48);		  //设置字库命令，地址为0x08
	for(i=0;i<8;i++)
			wc51ddr(name0[i]);
	wc51r(0x50);		  //设置字库命令，地址为0x10
	for(i=0;i<8;i++)
			wc51ddr(name1[i]);
	wc51r(0x58);		  //设置字库命令，地址为0x18
	for(i=0;i<8;i++)
			wc51ddr(name2[i]);
	wc51r(0x60);		  //设置字库命令，地址为0x20
	for(i=0;i<8;i++)
			wc51ddr(name3[i]);
	wc51r(0x68);		  //设置字库命令，地址为0x28
	for(i=0;i<8;i++)
			wc51ddr(name4[i]);	
	wc51r(0x70);		  //设置字库命令，地址为0x30
	for(i=0;i<8;i++)
			wc51ddr(name5[i]);			
}
	
void init_1602()   	  //1602初始化函数
{
    wc51r(0x01);       //清屏
    wc51r(0x38);	      //使用8位数据，显示两行，使用5×7的字型
    wc51r(0x0c);       //显示器开，光标开，字符不闪烁。
    wc51r(0x06);	      //字符不动，光标自动右移
    beiguang=0;	      //打开背光显示
	writecgram();
}

void displayname()//显示姓名
{
	wc51r(0x01);
	wc51r(0x80);
	wc51ddr(0x01);wc51ddr(0x02);delay_1602();
	wc51ddr(0x03);wc51ddr(0x04);delay_1602();
	wc51ddr(0x05);wc51ddr(0x06);delay_1602();
	wc51r(0xc0);
	wc51ddr(0x32);wc51ddr(0x30);wc51ddr(0x38);delay_1602();
	wc51ddr(0x31);wc51ddr(0x32);wc51ddr(0x30);delay_1602();
	wc51ddr(0x36);wc51ddr(0x30);wc51ddr(0x38);delay_1602();

}
