#include <IAP15W4K61S4.h>
#include <stdio.h>

typedef unsigned char uChar8;
typedef unsigned int uInt16;

/**************************************定义全局变量********************************************************************/	
sbit LED = P5^5;                                                 //LED灯
uInt16 count = 0;                                                //计数器
int time = 50;                                                  //倒计时初始时间
struct {                                                         //定时器初始值结构体
    uChar8 TH;
    uChar8 TL;
}Timer1, Timer2;

/**************************************定义外部函数接口************************************************************************/
extern void LCD12864ShowString(uChar8 x, uChar8 y, uChar8* str); //第y行第x列，开始打印str
extern void LCD12864EraseArea(uChar8 x, uChar8 y, uChar8 size);  //从第y行第x列开始，删除
extern void keyScanner(void);                                    //按键扫描
extern uChar8 interface;                                         //全局变量，表示当前所处界面
extern void LCD12864EraseAll(void);		
extern void LCD12864WriteCommand(uChar8 command);	                //写命令函数
extern void LCD12864WriteData(uChar8 dat);                     	//写数据函数
/**************************************函数声明************************************************************************/
void configureTimer0(void) reentrant;                            //系统时钟time0配置
void configureTimer1(uChar8 ms);                                 //定时器1配置
void TimerCounter_1 (void);                                      //定时器1中断：扫描按键
void configureTimer2(void);                                      //定时器2配置
void TimerCounter_2 (void);                                      //定时器1中断：倒计时
void display(void);                                              //显示倒计时


//系统时钟time0配置
void configureTimer0(void) reentrant
{
    TMOD &= 0xF0;
	TMOD |= 0x01;                                                //定时器0：模式1(16位定时器)，仅受TR0控制；定时器1：波特率发生器
//  TH0 = 0xDC;                                                  //定义Tick=100次/秒(即0.01秒/次),TH,TL值与CPU的频率有关(11.0592M)
//  TL0 = 0x00;  	                                             //OS_CPU_C.C中定时器中断响应也要设置，OS_CFG.H中OS_TICKS_PER_SEC也有关系
    TR0  = 1;		                                             //必须启动
}

//定时器1配置
void configureTimer1(uChar8 ms)
{
    unsigned long timer = 11059200 / 12;
    timer = timer * ms / 1000;
    timer = 65536 - timer;
    TH1 = (uChar8)(timer >> 8);
    TL1 = (uChar8)timer;
    Timer1.TH = TH1;
    Timer1.TL = TL1;
    TMOD &= 0xF0;
    TMOD |= 0x01;
    ET1 = 1;
    TR1 = 1;
}

//定时器1中断
void TimerCounter_1 (void) interrupt 3 
{ 
    TH1 = Timer1.TH;
    TL1 = Timer1.TL;
	keyScanner();
}


//定时器2配置
void configureTimer2(void)
{
	AUXR &= 0xFB;
	T2H = 0xFC;
	T2L = 0X67;
	Timer2.TH = T2H;
    Timer2.TL = T2L;
	AUXR |= 0x10;
}

//定时器2中断
void TimerCounter_2 (void) interrupt 12
{ 
	T2H = Timer2.TH;
	T2L = Timer2.TL;
	count++;
	if(count == 1000)
	{
		display();
		count = 0;
	}
}


//显示倒计时
void display(void)
{
	uChar8 str[8];
	LCD12864WriteCommand(0x38);
  LCD12864WriteCommand(0x0C);
  LCD12864WriteCommand(0x06);
	sprintf(str, "%03d", time--);
	LCD12864EraseArea(4, 3, 8);
	LCD12864ShowString(4, 3, str);
	if(time == -1)
	{
		sprintf(str, "LOSE");
		LCD12864EraseAll();
		LCD12864ShowString(3, 3, str);
		IE2 = 0x00;                 //关闭定时器2
		interface = 2;
	}
}