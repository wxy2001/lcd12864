#include <IAP15W4K61S4.h>

#define LCD12864_DB P0
#define uchar unsigned char
#define uint  unsigned int
typedef unsigned char uChar8;

/**************************************����ȫ�ֱ���***********************************************************************/	
sbit LCD12864_RS = P3^5;
sbit LCD12864_RW = P3^6;
sbit LCD12864_E = P3^7;

uChar8 i, j;                                                //ѭ������

/**************************************��������*******************************************************************************/
void LCD12864WaitReady(void);                               //���12864�Ƿ�æ
void LCD12864WriteCommand(uChar8 command);	                //д�����
void initLCD12864(void);	                                //��ʼ��12864
void LCD12864WriteData(uChar8 dat);                     	//д���ݺ���
void LCD12864SetCursor(uChar8 x, uChar8 y) ;	            //�ӵ�y�е�x��д����
void LCD12864ShowString(uChar8 x, uChar8 y, uChar8* str) ;	//��y�е�x�У���ʼ��ӡstr
void LCD12864EraseArea(uChar8 x, uChar8 y, uChar8 size);	//�ӵ�y�е�x�п�ʼ��ɾ��size�ַ�
void LCD12864EraseAll(void);	                            //����������ʾ��
void IO_init(void);
void img_disp(uChar8 code *img); 	                        //��128 * 64������ͼƬ
void img_clear(void);                                       //����
void BMP_disp(uChar8 x, uChar8 y, uChar8 code *img);

void startUnderLine(uChar8 id);                             //����ʼ�����»���
void img_line_start1(void); 	                            //��һ���»���	
void img_dele_line_start1(void);                            //ɾ�������»���
void img_line_start2(void);
void img_dele_line_start2(void);	
	


/**************************************LCD12864���ܺ���**************************************************************/	
//���12864�Ƿ�æ
void LCD12864WaitReady(void)
{
    uChar8 state;

    LCD12864_DB = 0xFF; //P0
    LCD12864_RS = 0;
    LCD12864_RW = 1;
    
    do {
        LCD12864_E = 1;
        state = LCD12864_DB;
        LCD12864_E = 0;
    } while(state & 0x80);
}

//д�����
void LCD12864WriteCommand(uChar8 command)
{
    LCD12864WaitReady();
    LCD12864_RS = 0;
    LCD12864_RW = 0;
    LCD12864_DB = command;
    LCD12864_E = 1;
    LCD12864_E = 0;
}

//��ʼ��12864
void initLCD12864(void)
{
    LCD12864WriteCommand(0x38);
    LCD12864WriteCommand(0x0C);
    LCD12864WriteCommand(0x06);
    LCD12864WriteCommand(0x01);
}

//д���ݺ���
void LCD12864WriteData(uChar8 dat)
{
    LCD12864WaitReady();
    LCD12864_RS = 1;
    LCD12864_RW = 0;
    LCD12864_DB = dat;
    LCD12864_E = 1;
    LCD12864_E = 0;
}

//�ӵ�y�е�x��д����
void LCD12864SetCursor(uChar8 x, uChar8 y) 
{
    uChar8 address;
    if (0 == y) 
	{
        address = 0x80 + x;
    } else if (1 == y) 
	{
        address = 0x90 + x;
    } else if (2 == y) 
	{
    	address = 0x88 + x;
	} else if (3 == y)
	{
		address = 0x98 + x;
	}
    LCD12864WriteCommand(address);
}

//��y�е�x�У���ʼ��ӡstr
void LCD12864ShowString(uChar8 x, uChar8 y, uChar8* str) 
{
    LCD12864SetCursor(x, y);
    while (*str != '\0') {
        LCD12864WriteData(*str++);
    }
}

//�ӵ�y�е�x�п�ʼ��ɾ��size�ַ�
void LCD12864EraseArea(uChar8 x, uChar8 y, uChar8 size)
{
    LCD12864SetCursor(x, y); //��λ
    while(size--) {
         LCD12864WriteData(' '); //����ԭ����
    }
}

//����������ʾ��
void LCD12864EraseAll(void)
{
    LCD12864WriteCommand(0x01);
}

//���IAP15W4K61S4��STC15W4K56S4ϵ�� IO�ڳ�ʼ��
void IO_init(void)
{
	P0M0 = 0X00;
	P0M1 = 0X00;

	P1M0 = 0X00;
	P1M1 = 0X00;

	P2M0 = 0X00;
	P2M1 = 0X00;

	P3M0 = 0X00;
	P3M1 = 0X00;

	P4M0 = 0X00;
	P4M1 = 0X00;  
}

/************************************LCD12864��ͼ����**************************************************************/	
//��128 * 64������ͼƬ
void img_disp(uChar8 code *img) 
{
	for(j = 0; j < 32; j++)     //�ϰ���
	{
		for(i = 0; i < 8; i++)
		{
			LCD12864WriteCommand(0x34);      
			LCD12864WriteCommand(0x80 + j);   
			LCD12864WriteCommand(0x80 + i);    
			LCD12864WriteCommand(0x30);      
			LCD12864WriteData(img[j * 16 + i * 2]); 
			LCD12864WriteData(img[j * 16 + i * 2 + 1]);  
		}
	}
	for(j = 32; j < 64; j++)    //�°���   
	{
		for(i = 0; i<8; i++)
		{
			LCD12864WriteCommand(0x34);        
			LCD12864WriteCommand(0x80 + j - 32);
			LCD12864WriteCommand(0x88 + i);    
			LCD12864WriteCommand(0x30);       
			LCD12864WriteData(img[j * 16 + i * 2]);    
			LCD12864WriteData(img[j * 16 + i * 2 + 1]);    
		} 
	}
	LCD12864WriteCommand(0x36); 
	LCD12864WriteCommand(0x30);	
}


//����
void img_clear(void)
{
	for(j = 0; j < 32; j++)
	{
		for(i = 0; i < 8; i++)
		{
			LCD12864WriteCommand(0x34);      
			LCD12864WriteCommand(0x80 + j);   
			LCD12864WriteCommand(0x80 + i);    
			LCD12864WriteCommand(0x30);      
			LCD12864WriteData(0x00); 
			LCD12864WriteData(0x00);  
		}
	}
	for(j = 32; j < 64; j++)      
	{
		for(i = 0; i < 8; i++)
		{
			LCD12864WriteCommand(0x34);        
			LCD12864WriteCommand(0x80 + j - 32);
			LCD12864WriteCommand(0x88 + i);    
			LCD12864WriteCommand(0x30);       
			LCD12864WriteData(0x00); 
			LCD12864WriteData(0x00); 
		} 
	}
	LCD12864WriteCommand(0x36);
	LCD12864WriteCommand(0x30);	
}

/***************************startInterface******************************************************************/
void startUnderLine(uChar8 id)     //������Ϸ�����»���
{
	img_dele_line_start1();
	img_dele_line_start2();
	//img_dele_line_start3();
	switch(id)
	{
		case 0: 
			img_line_start1();
			break;
		case 1: 
			img_line_start2();
			break;
		default: break;
	}
}

//��һ���»���
void img_line_start1(void)
{
	j = 28;
	LCD12864WriteCommand(0x34);      
	LCD12864WriteCommand(0x80 + 28);   
	LCD12864WriteCommand(0x80 + 5);    
	LCD12864WriteCommand(0x30);      
	LCD12864WriteData(0x00);  
	LCD12864WriteData(0xFF);
	for(i = 6; i<7; i++)
	{
		LCD12864WriteCommand(0x34);      
		LCD12864WriteCommand(0x80 + j);   
		LCD12864WriteCommand(0x80 + i);    
		LCD12864WriteCommand(0x30);      
		LCD12864WriteData(0xFF);  
		LCD12864WriteData(0xFF);
	}
	LCD12864WriteCommand(0x34);      
	LCD12864WriteCommand(0x80 + 28);   
	LCD12864WriteCommand(0x80 + 7);    
	LCD12864WriteCommand(0x30);
	LCD12864WriteData(0xFF);	
	LCD12864WriteData(0x00);  

	LCD12864WriteCommand(0x36); 
	LCD12864WriteCommand(0x30);	
}

//ɾ�������»���
void img_dele_line_start1(void)
{
	j = 28;
	for(i = 5; i < 8; i++)
	{
		LCD12864WriteCommand(0x34);      
		LCD12864WriteCommand(0x80 + j);   
		LCD12864WriteCommand(0x80 + i);    
		LCD12864WriteCommand(0x30);      
		LCD12864WriteData(0x00);  
		LCD12864WriteData(0x00);
	}
	LCD12864WriteCommand(0x36); 
	LCD12864WriteCommand(0x30);	
}




void img_line_start2(void)
{
	j = 50;
	LCD12864WriteCommand(0x34);      
	LCD12864WriteCommand(0x80 + 50);   
	LCD12864WriteCommand(0x80 + 5);    
	LCD12864WriteCommand(0x30);      
	LCD12864WriteData(0x00);  
	LCD12864WriteData(0xFF);
	for(i = 6; i < 7; i++)
	{
		LCD12864WriteCommand(0x34);        
		LCD12864WriteCommand(0x80 + j - 32);
		LCD12864WriteCommand(0x88 + i);    
		LCD12864WriteCommand(0x30);       
		LCD12864WriteData(0xFF); 
		LCD12864WriteData(0xFF); 
	} 
	LCD12864WriteCommand(0x36); 
	LCD12864WriteCommand(0x30);	
}

void img_dele_line_start2(void)
{
	j = 50;
	for(i = 5; i < 7; i++)
	{
		LCD12864WriteCommand(0x34);        
		LCD12864WriteCommand(0x80 + j - 32);
		LCD12864WriteCommand(0x88 + i);    
		LCD12864WriteCommand(0x30);       
		LCD12864WriteData(0x00); 
		LCD12864WriteData(0x00); 
	} 
	LCD12864WriteCommand(0x36); 
	LCD12864WriteCommand(0x30);	
}


