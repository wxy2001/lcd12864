#include "includes.h"

typedef unsigned char uChar8;

/**************************************����ȫ�ֱ���********************************************************************/	
sbit LED = P5^5;                                //���е�
OS_STK gameTaskStk[MaxStkSize+1];               //�����Լ��������ջ
OS_STK LEDTaskStk[MaxStkSize+1];                //�����Լ��������ջ
OS_STK LCD12864TaskStk[MaxStkSize+1];           //�����Լ��������ջ
uChar8 isStudentInterface = 0;                  //��־λ��Ϊ1��ʾִ�У���1��ʾ��ִ��LCD����

/**************************************�����ⲿ�����ӿ�*****************************************************************/	
extern void IO_init(void);                      //IAP15�˿ڳ�ʼ��
extern void initLCD12864(void);                 //LCD12864��ʼ��
extern void keyDriver(void);                    //��������
extern void configureTimer0(void) reentrant;    //ϵͳʱ��time0��ʼ��
extern void configureTimer1(uChar8 ms);         //��ʱ��1��ʼ��
extern void configureTimer2(void);              //��ʱ��2��ʼ��
extern void startInterface(void);               //��ʼ����
extern void studentInterface(void);             //��ʼ����
extern void gameInterface(void);

/**************************************�����û�����***********************************************************************/	
void LEDTask(void *ppdata) reentrant;           //LED״̬��������
void LCD12864Task(void *ppdata) reentrant;      //LCD��������
void gameTask(void *ppdata) reentrant;          //��Ϸ����

/**************************************������**********************************************************************/	
void main(void)
{
	IO_init();                                  //IAP15�˿ڳ�ʼ��
	initLCD12864();                             //LCD12864��ʼ��
	configureTimer1(1);                         //��ʱ��1��ʼ��
	configureTimer2();                          //��ʱ��2��ʼ��
	EA  = 1;                                    //���������жϿ���
	
	OSInit();                                   //ucosiiϵͳ��ʼ��
	configureTimer0();                          //ϵͳʱ��time0  	    

	OSTaskCreate(LEDTask, (void*)0, &LEDTaskStk[0], 1);
	OSTaskCreate(LCD12864Task, (void*)0, &LCD12864TaskStk[0], 2);
	OSTaskCreate(gameTask, (void*)0, &gameTaskStk[0], 3);
	
	OSStart();                                  //��ʼ�������
}




void LEDTask(void *ppdata) reentrant
{
	ppdata = ppdata;                            //��ֹ�������
	ET0 = 1;                                    //������ʱ�ӽ����ж�
	
	while(1)
    {
		LED = !LED;
        OSTimeDly(OS_TICKS_PER_SEC);            //�û�������ʱ1s
    }    
}

void LCD12864Task(void *ppdata) reentrant
{
	ppdata = ppdata;                         
	
	while(1)
    {
		if(isStudentInterface == 1)             //��־λ��LCD�Ƿ�ִ����ʾѧ����Ϣ����
		{
			studentInterface();                 //ѧ����Ϣ����
		}
		OSTimeDly(OS_TICKS_PER_SEC);
    }  
}

void gameTask(void *ppdata) reentrant
{
	ppdata = ppdata;                           
	startInterface();                           //��Ϸ��ʼ����
	//gameInterface();
	while(1)
  {
		
		keyDriver();                            //��������
        OSTimeDly(OS_TICKS_PER_SEC / 10);       //�û�������ʱ0.1s
  }

}






