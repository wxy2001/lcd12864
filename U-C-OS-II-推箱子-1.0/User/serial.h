/*
*********************************************************************************************************
*                                               uC/OS-II
*                                               ʵʱ�ں�
*
*                        (c) Copyright 1992-1998, Jean J. Labrosse, Plantation, FL
*                                               ��Ȩ����
*
*                                            MCU-51 ר�ô���
*                                           KEIL C51��ģʽ����
*
* �ļ��� : SERIAL.H
* ����   : Jean J. Labrosse
* �ı�   : ���� gdtyy@ri.gdt.com.cn ������˾ϵͳ���ɿ����� 2002.09.27
*********************************************************************************************************
*/

//**********************************************************************************
//����    2002/08/20    ��һ��
//�����жϵĴ�����������ʾ����
//**********************************************************************************
//ʹ�÷��������˷����԰������������������򡣣�
//        1.��ͷ����#include <reg51.h>��䣬һ��Ҫ�С�
//        2.��ʼ������        InitSerial();//������Ϊ20MHz���壬300�����ʣ�ģʽ2��ʼ��
//        3.��ʼ�����ڻ�����  InitSerialBuffer();
//        4.ʹ����ʾ�ֽڣ��֣����֣��ַ����ַ���������������
//**********************************************************************************
//�����ʼ��㹫ʽ��
//        TH1=256-(2^SMOD/32*Fosc/12*1/Bound)
//    ���У�SMOD=0��1��Fosc=�������Ƶ�ʣ�Bound=������
//    �����У�SMOD=0��Fosc=20*10E6��Bound=300������TH1=0x52��
//**********************************************************************************
void InitSerial() reentrant;										//���ڳ�ʼ��  ���뺯������
//void InitSerialBuffer(void) reentrant;							    //���ڻ�������ʼ��
//INT8U CommPutChar(unsigned char ch) reentrant;						//��ʾ�ַ�
//void CommPutStr(unsigned char *str, unsigned int n) reentrant; 	    //��ʾ�ַ���
//bit  CommGetChar(unsigned char *ch) reentrant;						//�Ӵ��ڻ�������1�ֽ�����
void Clear_Buf(unsigned char length) reentrant;
void Process_Uart(void)	reentrant;
void serial(void) reentrant;//�����жϷ����ӳ���

void Send_Str_Uart1(unsigned char str[],unsigned int n)	 reentrant;

//�շ�����������
#define TBUF_SIZE        20     
#define RBUF_SIZE        20

