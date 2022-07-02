#include <IAP15W4K61S4.h>

/**************************************����ȫ�ֱ���********************************************************************/	
sbit keyOut1 = P2^3;
sbit keyOut2 = P2^2;
sbit keyOut3 = P2^1;
sbit keyOut4 = P2^0;
sbit keyIn1 = P2^5;
sbit keyIn2 = P2^6;
sbit keyIn3 = P2^7;
sbit keyIn4 = P2^4;

unsigned char current[4][4] = {         //��ǰ״̬��
    1, 1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1
};


unsigned char code hashMap[4][4] = {    //hash��ӳ�䰴������
    0x31, 0x32, 0x33, 0x26,
    0x34, 0x35, 0x36, 0x25,
    0x37, 0x38, 0x39, 0x28,
    0x30, 0x1B, 0x0D, 0x27
};

/**************************************�����ⲿ�����ӿ�************************************************************************/
extern void keyAction(unsigned char keycode);

/**************************************��������************************************************************************/
void keyScanner(void);                  //����ɨ�裬��ʱ�ж�
void keyDriver(void);                   //������������
		
void keyScanner(void)                   //����ɨ�裬��ʱ�ж�
{
    unsigned char k;
    static unsigned char keyout = 0;
    static unsigned char state[][4] = { //״̬����
        0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF
    };

    state[keyout][0] = state[keyout][0] << 1 | keyIn1;
    state[keyout][1] = state[keyout][1] << 1 | keyIn2;
    state[keyout][2] = state[keyout][2] << 1 | keyIn3;
    state[keyout][3] = state[keyout][3] << 1 | keyIn4;
    for (k = 0; k < 4; k++) {
        if (0x0F == (0x0F & state[keyout][k])) {
            current[keyout][k] = 1;
        }
        else if ((0x00 == (0x0F & state[keyout][k]))) {
            current[keyout][k] = 0;
        }
    }
    keyout++;
    keyout &= 0x03;
    switch (keyout) {
        case 0:
            keyOut4 = 1; keyOut1 = 0; break;
        case 1:
            keyOut1 = 1; keyOut2 = 0; break;
        case 2:
            keyOut2 = 1; keyOut3 = 0; break;
        case 3:
            keyOut3 = 1; keyOut4 = 0; break;
        default: 
            break;
    }
}

void keyDriver(void)                              //������������
{
    unsigned char k;
    unsigned char i;
	
    static char previous[][4] = {                 //��������һ��״̬
        1, 1, 1, 1,
        1, 1, 1, 1,
        1, 1, 1, 1,
        1, 1, 1, 1
    };

    for (k = 0; k < 4; k++) {
        for (i = 0; i < 4; i++) {
            if (current[k][i] != previous[k][i]) {
                if (0 == current[k][i]) {
                    keyAction(hashMap[k][i]);	  //���������� 
                }
                previous[k][i] = current[k][i];
            }
        }
    }
}
