/*********************************************************************************************************
*                                               uC/OS-II
*                                               ʵʱ�ں�
*
*                        (c) Copyright 1992-1998, Jean J. Labrosse, Plantation, FL
*                                               ��Ȩ����
*
*                                            MCU-51 ר�ô���
*                                           KEIL C51��ģʽ����
*
* �ļ��� : SERIAL.C
* ����   : Jean J. Labrosse
* �ı�   : ���� gdtyy@ri.gdt.com.cn ������˾ϵͳ���ɿ����� 2002.09.27
********************************************************************************************************/
//**********************************************************************************
//����    2002/08/20    ��һ��
//�����жϵĴ�������
//**********************************************************************************
//�����ص㣺
//        1.�����жϣ��ɲ���ִ��
//        2.���������ã��շ���������С������ַ������ȣ�
//**********************************************************************************
//ʹ�÷���:
//        �������շ���������С�ȿɱ��������serial.h�еĺ궨�壩
//        1.��ͷ����#include <reg51.h>��䣬һ��Ҫ�С�
//        2.��ʼ������        InitSerial();//������Ϊ20MHz���壬300�����ʣ�ģʽ2��ʼ��
//        3.��ʼ�����ڻ�����  InitSerialBuffer();
//**********************************************************************************
//�����ʼ��㹫ʽ��
//        TH1=256-(2^SMOD/32*Fosc/12*1/Bound)
//    ���У�SMOD=0��1��Fosc=�������Ƶ�ʣ�Bound=������
//    �����У�SMOD=0��Fosc=20*10E6��Bound=300������TH1=0x52��
//	  Baud rate(20Mhz)
//	  300(52);1200(D5);2400(EA);4800(F5);9600(FB);19200(FD);38400(FF);
//**********************************************************************************
#ifndef  OS_MASTER_FILE
#include "source\includes.h"
#endif
#include "source\includes.h"

#define COMM_NO_ERR 0
#define COMM_RX_EMPTY 1
#define COMM_TX_FULL 1

//���ٴ�������
//��Լ�������ʵĹؼ������Ǵ��пڵ��жϷ����ӳ����ִ��ʱ�䣻uCOSҪ��  �������� < 19200
//������ģ�ͣ������жϼ������жϺ��������жϣ��˳�ʱ���ж� �Ҳ�֪ͨuCos ��ߴ����ж����ȼ�
INT8U TxBuf[TBUF_SIZE] = {0}, RxBuf[RBUF_SIZE] = {0};         //�շ�������
//INT8U *inTxBuf = NULL, *outTxBuf = NULL, *inRxBuf = NULL, *outRxBuf = NULL; //�շ���������дָ��
INT8U RxNum = 0;
int gPosi_S1 = -1;
bit TIflag = 1;       //�����źţ�Note:It must be 1.
bit gRec_Flag1 = 0;   //�����ź�



void InitSerial() reentrant//���ڳ�ʼ��
{	//11.0592MHZ  9600
	SCON = 0x50;
	PCON &= 0x7f;		//�����ʲ�����
	AUXR |= 0x40;		//��ʱ��1ʱ��ΪFosc,��1T
 	AUXR &= 0xfe;		//����1ѡ��ʱ��1Ϊ�����ʷ�����
	TMOD &= 0x0f;		//�����ʱ��1ģʽλ
	TMOD |= 0x20;		//�趨��ʱ��1Ϊ8λ�Զ���װ��ʽ
	TL1 = 0xDC;		    //1T
	TH1 = 0xDC;		    //
	TR1 = 1;
	ES = 1;
}

/*
void InitSerialBuffer(void) reentrant//���ڻ�������ʼ��
{
        ES = 0;
		inTxBuf = TxBuf;       //��ʼ�����ͽ����������ָ��
		outTxBuf = TxBuf;
        inRxBuf = RxBuf;
		outRxBuf = RxBuf;
        ES = 1;
}
*/
void serial(void) reentrant	  //����Ӧ�ù��ܲ���
{    	//�ж�ϵͳ���ִ����ڻ����ʵ�֣�ȥ��interrupt 4{//�����жϷ����ӳ���
	//INT8U *t;
	/*
    if (RI)	//�����жϴ���
	{	
    	RI = 0;	  				
        t = inRxBuf;
		t++;
        if (t == (RxBuf + RBUF_SIZE))  //��һ��λ�õ�ĩβ
			t = RxBuf;
        if (t == outRxBuf) 
			return;                    //RxBuf Full	  ��Ϊ��ʼ����ʱ������ָ������ͬ��
        *inRxBuf = SBUF;	//��Ž�������
        inRxBuf = t;        //ָ����һ��λ��
    }
	*/
	if (RI)
	{
		RI = 0;
		gRec_Flag1 = 1;	
			
		if (RxNum < RBUF_SIZE)
			RxBuf[RxNum++] = SBUF;
		else
			SBUF = 0;
	}
	/*
	if (TI)					//�����жϴ���
	{
        TI = 0;
		if (inTxBuf == outTxBuf) 	  //�������ָ����ͬ�����ͻ����  ��ʼ����ʱ������ָ������ͬ��
		{
			TIflag = 1;				  //�����ź� = 	1�����͵�һ������֮��������� ֱ�����ͻ���Ϊ�� ͬʱ TIflag = 1
			return;
		}
        SBUF = *(outTxBuf++); 			    //��������  ָ����һ������λ��	ÿ������һ������ TI��Ӳ����1
        if (outTxBuf == TxBuf + TBUF_SIZE) 	//���ָ�볬�����泤�ȣ����¶�λ�������׵�ַ
			outTxBuf = TxBuf;	
     }
	*/
}

void Send_Char_Uart1(unsigned char ch) reentrant
{
  ES = 0;                      //�ش���1�жϣ��ڲ�����������жϱ�־ʱ�������������жϺ���
  TI = 0;                      //���㴮�ڷ�������жϱ�־
  SBUF = ch;                   //��������
  while(TI == 0);	           //�ȴ��������
  TI = 0;					   //���㷢������жϱ�־
  ES = 1;						
}

void Send_Str_Uart1(unsigned char str[],unsigned int n)	 reentrant
{
   unsigned int i;
   for(i = 0;i < n;i++) Send_Char_Uart1(str[i]);
}

/*
bit CommGetChar(unsigned char *ch) reentrant//�Ӵ��ڻ�������1�ֽ�����
{
        //ES=0;        
        if(inRxBuf == outRxBuf) //���ջ����
		{
			ES = 1;
			return 0;
		}
        *ch = *outRxBuf;  
		outRxBuf++;
        if(outRxBuf == RxBuf + RBUF_SIZE) 
			outRxBuf = RxBuf;
        //ES=1; 
		return 0;       
}
*/
/*
INT8U CommPutChar(unsigned char ch) reentrant//��ʾ�ַ�
{
        unsigned char *t;
        ES = 0;        
        if (TIflag)	     //�����ź� = 1  ��һ�η�������
		{		
        	TIflag = 0;	   
            TI = 1;		 //�����жϷ���
        }
        t = inTxBuf;	 //����ָ��
		t++;
        if (t == TxBuf + TBUF_SIZE) 	//����ָ�볬�����泤�ȣ����¶�λ�������׵�ַ
			t = TxBuf;
        if (t == outTxBuf) 	 //���ͻ�����  ���ܷ���
		{
			ES = 1;
			return (COMM_TX_FULL);
		}                 
        *inTxBuf = ch;	  //�����ݷ��뷢�ͻ���ȴ�����
        inTxBuf = t;	  //����ָ���1
        ES = 1;
		return (COMM_NO_ERR);
}
*/
/*
void CommPutStr(unsigned char *str, unsigned int n) reentrant//��ʾ�ַ���
{
        unsigned char i;
	
        for(i = 0;i < n;i++) CommPutChar(*(str + i));
}
*/

/*���ڽ��մ���*/
void Process_Uart(void)	reentrant
{
	INT8U CMD = 0xff;
	//Feed_WDT();
	if (gRec_Flag1)
	{
	    OSTimeDly(OS_TICKS_PER_SEC/10);	 //100ms
		Send_Str_Uart1(RxBuf, 20);
		gPosi_S1 = strpos(RxBuf, 0x7e);
		if (gPosi_S1 != -1) 		   //����ҵ�
		{
			CMD = RxBuf[gPosi_S1 + 2];   //��ȡ����

			switch (CMD)
			{						   
				case 0x0a:        //ȡ����������ģʽ
//					ZY1730_CONFIG = 0;
					break;
				case 0xdd:
					DE485 = 1;    //����ʹ��
				//	Send_Str_Uart1("485 Test!\r\n", 11);
					DE485 = 0;
					break;
				default:		  //Ƕ�ײ�����������������������Ϣ
					switch (CMD)
					{
						case 0x10:			  //¼��
						//	ZY1730_CONFIG = 1;
							if((RxBuf[gPosi_S1 + 3] >= 0x10) && (RxBuf[gPosi_S1 + 4] <= 0xff))
							{
						//		Send_Str_Uart1("Start Rec!",10);
						//		Start_Rec_Voice(S1_Rec_Buf[gPosi_S1 + 3], S1_Rec_Buf[gPosi_S1 + 4]);  //¼�����̾��������εĴ�С
							}
							break;
						case 0x11:			  //7e 02 11 xx xx 0d 0a	   ����
						//	ZY1730_CONFIG = 1;
							if((RxBuf[gPosi_S1 + 3] >= 0x10) && (RxBuf[gPosi_S1 + 4] <= 0xff))
							{
						//		Send_Str_Uart1("Playing Rec!",12);
						//		Play_Voice(S1_Rec_Buf[gPosi_S1 + 3], S1_Rec_Buf[gPosi_S1 + 4]);//����	   0x10~0xff
							}
							break;
						case 0x12:			  //����
						//	ZY1730_CONFIG = 1;
							if((RxBuf[gPosi_S1 + 3] >= 0x10) && (RxBuf[gPosi_S1 + 4] <= 0xff))
							{
						//		Send_Str_Uart1("Eerseing!",9);
						//		Erase_Voice(S1_Rec_Buf[gPosi_S1 + 3], S1_Rec_Buf[gPosi_S1 + 4]);
							}
							break;						
					}
			}
		}
		Clear_Buf(RxNum);
		gRec_Flag1 = 0;	
		RxNum = 0;
		gPosi_S1 = -1;
	}
}

void Clear_Buf(unsigned char length) reentrant
{
	unsigned char i;
	ES = 0;
	for (i = 0; i < length; i++)
		RxBuf[i] = 0;
	ES = 1;
}



