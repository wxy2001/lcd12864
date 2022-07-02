#include <stdio.h>                   //sprintf()
#include <stdlib.h>                  //随机数
#include <IAP15W4K61S4.h>

typedef unsigned char uChar8;

/**************************************定义全局变量***********************************************************************/	
sbit LED = P5^5; 
sbit GLED = P4^2;                     //LED灯
static int ANS[5], i, j, grade = 0;  //ANS[]存储每一轮的答案，grade表示得分，i, j为循环变量
static int KEY[5] = {0, 0, 0, 0, 0}; //KEY[]存储用户输入的答案
uChar8 sign = 0;                     //sign表示数组的下标，通过sign加一或减一来实现左右移动
uChar8 string[20];                   //要打印到屏幕上的信息，临时全局变量
uChar8 interface = 0;                //一共三个界面, interface = 0表示键盘响应开始界面的操作；= 1表示按键响应游戏页面； = 2表示响应游戏结束界面
uChar8 startUnderLineId = 0;         //startUnderLineId = 0 表示下划线在"开始"处；= 1 表示下划线在"退出"处
uChar8 gameModel = 0 ;               //gameModel = 0 表示当前处于猜数字；= 1表示猜字母

/**************************************定义外部函数接口************************************************************************/
extern void startInterface(void);                                //开始界面
extern void gameInterface(void);                                 //游戏界面
extern void LCD12864ShowString(uChar8 x, uChar8 y, uChar8* str); //第y行第x列，开始打印str
extern void LCD12864EraseArea(uChar8 x, uChar8 y, uChar8 size);  //从第y行第x列开始，删除size字符
extern void LCD12864EraseAll(void);                              //擦除整个显示屏
extern void img_line_start1(void);                               //画开始界面"开始"处的下划线
extern void img_dele_line_start1(void);                          //删除开始界面"开始"处的下划线
extern void img_line_start2(void);                               //画开始界面"US"处的下划线
extern void img_dele_line_start2(void);                          //删除开始界面"US"处的下划线
extern void startUnderLine(uChar8 id);
extern void img_dele_underLine(void);                            //删除所有下划线
extern void display(void);                                       //倒计时显示函数
extern uChar8 isStudentInterface;                                //任务LCD12864Task内是否执行那段函数，来代替系统内信息传递机制
extern int time;                                                 //时间

extern uChar8 judge(int x, int y, int id);
extern void update(uChar8 a, uChar8 b);
extern void fun(uChar8 key, uChar8 dirction);
extern void R_right(void);
extern void R_left(void);
extern void R_down(void);
extern void R_up(void);
extern void LCD12864WriteCommand(uChar8 command);	                
/**************************************函数声明*******************************************************************************/
void KeyAction(uChar8 KeyValue);     //按键驱动函数 
void up(void);                       //上移
void down(void);                     //下移
void enterGame(void);                //确认键：进入游戏

void x_up(void);                     //控制并显示
void x_down(void);

void enterCmp(void);                 //比较用户回答是否正确
void left(void);                     //左移
void right(void);                    //右移
void tostop(void);                    //暂停游戏
void stoptogo(void);									//从暂停中恢复                 
void esc(void);
void Re(void);                   //快速重新开始

void steTime(void);                  //重置计时器
	
void GLEDFLASH(void);

uChar8 FLAG=0;										//控制暂停的标识

void GLEDFLASH(){
	int i;
	i=100000;
	GLED=!GLED;
	while(--i);
	GLED=!GLED;
}
//按键驱动函数
void KeyAction(uChar8 KeyValue) 
{
	if(interface == 0)               //开始界面的按键驱动
	{
		switch(KeyValue) 
		{
			case 0x32: up(); break;
			case 0x38: down(); break;
			case 0x35: enterGame(); break;
			default: break;
		}
	}
	
	if(interface == 1)               //游戏界面的按键驱动
	{
		
			switch(KeyValue) 
			{
				case 0x31: esc(); break;																								
				case 0x32: x_up(); break;																								
				case 0x33: tostop(); break;																								
				case 0x34: left(); break;																								
				case 0x35: enterCmp(); break;																								
				case 0x36: right(); break;																								
				case 0x37: Re(); break;																								
				case 0x38:  x_down(); break;																								
				//case 0x39:  break;																								
				default: break;
			}
			
	}
	
	if(interface == 2)               //显示学号的按键驱动
	{
		switch(KeyValue) 
		{
			case 0x31: esc(); break;
			case 0x37: Re(); break;
			default: break;
		}		
	}
	
	if(interface == 3)               //暂停游戏的按键驱动
	{
		switch(KeyValue) 
		{
			case 0x39: stoptogo(); break;
			//case 0x31: esc(); break;
			default: break;
		}		
	}
	
}














/************************************开始界面按键*****************************************************************************/
void up(void)
{
	GLEDFLASH();
	if(startUnderLineId == 0)
	{
		startUnderLineId = 0;
	}
	else
	{
		startUnderLineId--;
	}
	
	startUnderLine(startUnderLineId);
}
void down(void)
{
	GLEDFLASH();
	if(startUnderLineId == 1)
	{
		startUnderLineId = 1;
	}
	else
	{
		startUnderLineId++;
	}

	startUnderLine(startUnderLineId);
}

void enterGame(void)
{
	GLEDFLASH();
	if(startUnderLineId == 0)
	{
		interface = 1;    //进入游戏页面
		gameInterface();
	}
	if(startUnderLineId == 1) //xuehao
	{
		isStudentInterface = 1;
		startUnderLineId = 0;
		interface = 2;
	}
	
}

/************************************推箱子按键功能函数*****************************************************************************/

void x_up(void)
{
	GLEDFLASH();
	R_up();
} 

void x_down(void)
{
	GLEDFLASH();
  R_down();
	
}

void left(void)
{
	GLEDFLASH();
  R_left();
}

void right(void)
{
	GLEDFLASH();
	R_right();
}



void stoptogo(void)
{
	if(FLAG==1){
	interface=1;
	FLAG=0;
	IE2 = 0x04;
	LCD12864WriteCommand(0x38);
  LCD12864WriteCommand(0x0C);
  LCD12864WriteCommand(0x06);
	LCD12864EraseArea(6, 3, 8);
	}
}


void tostop(void)
{
	uChar8 str[]="暂停";
	GLEDFLASH();
	if(FLAG==0){
	interface=3;
	FLAG=1;
	IE2 = 0x00;
	LCD12864WriteCommand(0x38);
  LCD12864WriteCommand(0x0C);
  LCD12864WriteCommand(0x06);
	LCD12864EraseArea(6, 3, 8);
	LCD12864ShowString(6, 3, str);
	}

}



void esc(void)
{
	GLEDFLASH();
	interface = 0;
	isStudentInterface = 0;     //关闭任务2
	IE2 = 0x00;                 //关闭定时器2
	LCD12864EraseAll();
	startInterface();
}

void Re(void)
{
	GLEDFLASH();
	esc();
	enterGame();
}


void enterCmp(void)
{
	GLEDFLASH();
}


void steTime(void)
{
	IE2 |= 0x04;    //打开定时器2
	time = 50;     //重置时间
	display();
}

 