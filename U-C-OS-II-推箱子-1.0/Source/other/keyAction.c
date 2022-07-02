#include <stdio.h>                   //sprintf()
#include <stdlib.h>                  //�����
#include <IAP15W4K61S4.h>

typedef unsigned char uChar8;

/**************************************����ȫ�ֱ���***********************************************************************/	
sbit LED = P5^5; 
sbit GLED = P4^2;                     //LED��
static int ANS[5], i, j, grade = 0;  //ANS[]�洢ÿһ�ֵĴ𰸣�grade��ʾ�÷֣�i, jΪѭ������
static int KEY[5] = {0, 0, 0, 0, 0}; //KEY[]�洢�û�����Ĵ�
uChar8 sign = 0;                     //sign��ʾ������±꣬ͨ��sign��һ���һ��ʵ�������ƶ�
uChar8 string[20];                   //Ҫ��ӡ����Ļ�ϵ���Ϣ����ʱȫ�ֱ���
uChar8 interface = 0;                //һ����������, interface = 0��ʾ������Ӧ��ʼ����Ĳ�����= 1��ʾ������Ӧ��Ϸҳ�棻 = 2��ʾ��Ӧ��Ϸ��������
uChar8 startUnderLineId = 0;         //startUnderLineId = 0 ��ʾ�»�����"��ʼ"����= 1 ��ʾ�»�����"�˳�"��
uChar8 gameModel = 0 ;               //gameModel = 0 ��ʾ��ǰ���ڲ����֣�= 1��ʾ����ĸ

/**************************************�����ⲿ�����ӿ�************************************************************************/
extern void startInterface(void);                                //��ʼ����
extern void gameInterface(void);                                 //��Ϸ����
extern void LCD12864ShowString(uChar8 x, uChar8 y, uChar8* str); //��y�е�x�У���ʼ��ӡstr
extern void LCD12864EraseArea(uChar8 x, uChar8 y, uChar8 size);  //�ӵ�y�е�x�п�ʼ��ɾ��size�ַ�
extern void LCD12864EraseAll(void);                              //����������ʾ��
extern void img_line_start1(void);                               //����ʼ����"��ʼ"�����»���
extern void img_dele_line_start1(void);                          //ɾ����ʼ����"��ʼ"�����»���
extern void img_line_start2(void);                               //����ʼ����"US"�����»���
extern void img_dele_line_start2(void);                          //ɾ����ʼ����"US"�����»���
extern void startUnderLine(uChar8 id);
extern void img_dele_underLine(void);                            //ɾ�������»���
extern void display(void);                                       //����ʱ��ʾ����
extern uChar8 isStudentInterface;                                //����LCD12864Task���Ƿ�ִ���Ƕκ�����������ϵͳ����Ϣ���ݻ���
extern int time;                                                 //ʱ��

extern uChar8 judge(int x, int y, int id);
extern void update(uChar8 a, uChar8 b);
extern void fun(uChar8 key, uChar8 dirction);
extern void R_right(void);
extern void R_left(void);
extern void R_down(void);
extern void R_up(void);
extern void LCD12864WriteCommand(uChar8 command);	                
/**************************************��������*******************************************************************************/
void KeyAction(uChar8 KeyValue);     //������������ 
void up(void);                       //����
void down(void);                     //����
void enterGame(void);                //ȷ�ϼ���������Ϸ

void x_up(void);                     //���Ʋ���ʾ
void x_down(void);

void enterCmp(void);                 //�Ƚ��û��ش��Ƿ���ȷ
void left(void);                     //����
void right(void);                    //����
void tostop(void);                    //��ͣ��Ϸ
void stoptogo(void);									//����ͣ�лָ�                 
void esc(void);
void Re(void);                   //�������¿�ʼ

void steTime(void);                  //���ü�ʱ��
	
void GLEDFLASH(void);

uChar8 FLAG=0;										//������ͣ�ı�ʶ

void GLEDFLASH(){
	int i;
	i=100000;
	GLED=!GLED;
	while(--i);
	GLED=!GLED;
}
//������������
void KeyAction(uChar8 KeyValue) 
{
	if(interface == 0)               //��ʼ����İ�������
	{
		switch(KeyValue) 
		{
			case 0x32: up(); break;
			case 0x38: down(); break;
			case 0x35: enterGame(); break;
			default: break;
		}
	}
	
	if(interface == 1)               //��Ϸ����İ�������
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
	
	if(interface == 2)               //��ʾѧ�ŵİ�������
	{
		switch(KeyValue) 
		{
			case 0x31: esc(); break;
			case 0x37: Re(); break;
			default: break;
		}		
	}
	
	if(interface == 3)               //��ͣ��Ϸ�İ�������
	{
		switch(KeyValue) 
		{
			case 0x39: stoptogo(); break;
			//case 0x31: esc(); break;
			default: break;
		}		
	}
	
}














/************************************��ʼ���水��*****************************************************************************/
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
		interface = 1;    //������Ϸҳ��
		gameInterface();
	}
	if(startUnderLineId == 1) //xuehao
	{
		isStudentInterface = 1;
		startUnderLineId = 0;
		interface = 2;
	}
	
}

/************************************�����Ӱ������ܺ���*****************************************************************************/

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
	uChar8 str[]="��ͣ";
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
	isStudentInterface = 0;     //�ر�����2
	IE2 = 0x00;                 //�رն�ʱ��2
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
	IE2 |= 0x04;    //�򿪶�ʱ��2
	time = 50;     //����ʱ��
	display();
}

 