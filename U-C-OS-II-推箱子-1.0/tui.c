#include"IAP15W4K61S4.h"
#include<stdlib.h>
#include<stdio.h>
#include<intrins.h>
#define uchar unsigned char
#define uint  unsigned int
 
sbit RS = P3^5;
sbit RW = P3^6;
sbit EN = P3^7;
 
sbit key_up = P2^1;
sbit key_down = P2^2;
sbit key_left = P2^0;
sbit key_right = P2^3;
 
#define LCD_data P0
int cur_x,cur_y,X,dir[4][2]={-1,0,1,0,0,-1,0,1},where[3][2]={4,6,5,6,6,6};
 
 
uchar code table1[]="LCD12864";
uchar code table2[]="������";
uchar code table3[]="BY laoda";
uchar code table4[]="You Win!";
 
uchar code tmp[]={//�ո�
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
};
/* 0��ʾ�ո�1��ʾǽ��2��ʾ�ˣ�3��ʾ���ӣ�4��ʾĿ�ĵ� */
uchar code mat[8][8]={
1,1,1,1,1,1,1,1,
1,1,1,1,0,2,1,1,
1,1,0,0,3,0,1,1,
1,1,0,1,0,1,1,1,
1,0,0,1,0,1,4,1,
1,0,1,0,0,3,4,1,
1,0,3,0,0,0,4,1,
1,1,1,1,1,1,1,1
};
 
uchar map[8][8];
 
uchar code Qiang[]={//�����м�϶
0xff,0x81,0x81,0x81,0x81,0x81,0x81,0xff
};
 
uchar code Ren[]={//��
0xff,0xff,0xe3,0xc5,0xb1,0xc5,0xe3,0xff
};
 
uchar code Xiang[]={//����  
0xff,0x42,0x24,0x18,0x18,0x24,0x42,0xff  
};
 
uchar code Mudi[]={//Ŀ�ĵ�  
0xff,0xff,0xe7,0x81,0x81,0xe7,0xff,0xff  
};
 
void delay(uint i) //��ʱ���� 
{
	while(--i);
}
 
void read_busy()//��æ������ÿ�ζ�д��Ҫ���ж�æ���� 
{
	RS=0;
	RW=1;
	EN=1;
	while(LCD_data & 0x80);//���λΪBFλ DB7---BF 
	EN=0;
}
 

void write_LCD_command(uchar value)//дָ��� 
{
	read_busy();
	RS=0;
	RW=0;
	EN=0;		//EN��1---0�������� 
	LCD_data=value;
	delay(20);
	EN=1;
	delay(20);
	EN=0;
} 
 
void write_LCD_data(uchar value)//д���ݺ��� 
{
	read_busy();
	RS=1;
	RW=0;
	EN=0;		//EN��1---0�������� 
	LCD_data=value;
	delay(20);
	EN=1;
	delay(20);
	EN=0;
} 
void init_BMP()	 
{
	write_LCD_command(0x38);//CL=1--8λ������ָ��(RE=1)����ͼ��(G=1)
	delay(100);	//�ʵ���ʱ 
	write_LCD_command(0x38);
	delay(37);
	write_LCD_command(0x38);//8λ(CL=1)������ָ��(RE=1)����ͼ��(G=1) 
	delay(100);
	write_LCD_command(0x0c);
	delay(100);
	write_LCD_command(0x06);
	delay(100);
	write_LCD_command(0x01);//����ָ�� 
	delay(1000);
}
 
void init_Hanzi()				//8λ���ڷ�ʽLCD1864��ʼ������ 
{
	delay(4000);			//�ȴ�ʱ��>40ms 
	write_LCD_command(0x30);//�����趨��8λ���ݡ�����ָ����� 
	delay(100);				//�ȴ�ʱ��>100us 
	write_LCD_command(0x30);//�����趨��8λ���ݡ�����ָ����� 
	delay(37);				//�ȴ�ʱ��>37us 
	write_LCD_command(0x0C);//��ʾ�趨��������ʾ���α�ء������� 
	delay(100);				// �ȴ�ʱ��>100us 
	//write_LCD_command(0x01);//����ָ�� 
	//delay(10000);			//�ȴ�ʱ��>10ms 
	write_LCD_command(0x06);//������趨����ַָ���1 
}
 
void display_BMP(uchar fuck,uchar name1,uchar name2,uchar y,uchar x)	
{
	uchar shit,i;
	if(fuck==0)shit=0x80;
	else shit=0x88;
	for(i=0;i<8;i++)		
	{	
		write_LCD_command(0x80+i+8*x);//���ʹ�ֱ��ַ 
		write_LCD_command(shit+y);  //����ˮƽ��ַ ----��ʾͼƬ���ϰ벿�� 
 
		if(name1==0)	write_LCD_data(tmp[i]);
		else if(name1==1)	write_LCD_data(Qiang[i]);
		else if(name1==2)	write_LCD_data(Ren[i]);
		else if(name1==3)	write_LCD_data(Xiang[i]);
		else if(name1==4)	write_LCD_data(Mudi[i]);
 
		if(name2==0)	write_LCD_data(tmp[i]);
		else if(name2==1)	write_LCD_data(Qiang[i]);
		else if(name2==2)	write_LCD_data(Ren[i]);
		else if(name2==3)	write_LCD_data(Xiang[i]);
		else if(name2==4)	write_LCD_data(Mudi[i]);
	}
}
 
void show_map(){
	uchar i,j,x,y;
 
	for(i=0;i<2;i++){
		for(j=0;j<4;j++){
			x=2*i; y=2*j;
			display_BMP(0,map[x][y],map[x][y+1],j,2*i);
			x=2*i+1; y=2*j;
			display_BMP(0,map[x][y],map[x][y+1],j,2*i+1);
		}
	}
 
	for(i=2;i<4;i++){
		for(j=0;j<4;j++){
			x=2*i; y=2*j;
			display_BMP(1,map[x][y],map[x][y+1],j,2*(i-2));
			x=2*i+1; y=2*j;
			display_BMP(1,map[x][y],map[x][y+1],j,2*(i-2)+1);
		}
	}
 
	init_Hanzi();		
	write_LCD_command(0x80+4);
	for(i=0;i<8;i++)
	{
		write_LCD_data(table1[i]);
	}
	write_LCD_command(0x90+4);
	for(i=0;i<8;i++)
	{
		write_LCD_data(table2[i]);
	}
 
	write_LCD_command(0x88+4);
	for(i=0;i<8;i++)
	{
		write_LCD_data(table3[i]);
	}
	delay(100);
}
 
void clear()//ͼ��ģʽ�£�û�������������ֶ�ȫ����0						
{
	uchar i,j;
	for(i=0;i<32;i++){
		write_LCD_command(0x80+i);//���ʹ�ֱ��ַ 
		write_LCD_command(0x80);  //����ˮƽ��ַ ----��ʾͼƬ���ϰ벿�� 
		for(j=0;j<16;j++)write_LCD_data(0);
	}
	for(i=0;i<32;i++){
		write_LCD_command(0x80+i);	//���ʹ�ֱ��ַ
		write_LCD_command(0x88);	//��ʾͼƬ���°벿�� 
		for(j=0;j<16;j++)write_LCD_data(0);
	}
}
 
uchar judge(int x,int y,int id){							//id ��ʾ����������б꣬0,1,2,3 �ֱ��ʾ��������
	int xx,yy,xxx,yyy;
	xx=x+dir[id][0]; yy=y+dir[id][1];
	if(map[xx][yy]==0 || map[xx][yy]==4)return 1;			   //1��ʾǰ���� �ո� ���� Ŀ�ĵأ����ǿ���ֱ���ƶ�
	else if(map[xx][yy]==1)return 0;						   //0��ʾ�޷��ƶ�
	else if(map[xx][yy]==3){
		xxx=xx+dir[id][0]; yyy=yy+dir[id][1];
		if(map[xxx][yyy]==1 || map[xxx][yyy]==3)return 0;
		else if(map[xxx][yyy]==0 || map[xxx][yyy]==4)return 2; //2��ʾ��Ҫ����ƶ����Ȱ�ǰ��������ƶ�һ���������ƶ�һ
	}return 0;
}
 
void update(uchar a,uchar b){
	uchar i,j,x,y;
 
	write_LCD_command(0x3E);//8λ(CL=1)������ָ��(RE=1)����ͼ��(G=1) 
	delay(100);
 
	if(a<4){
		i=a/2; j=b/2;
		x=2*i; y=2*j;
		display_BMP(0,map[x][y],map[x][y+1],j,2*i);
		x=2*i+1; y=2*j;
		display_BMP(0,map[x][y],map[x][y+1],j,2*i+1);
	}
	else {
		i=a/2; j=b/2;
		x=2*i; y=2*j;
		display_BMP(1,map[x][y],map[x][y+1],j,2*(i-2));
		x=2*i+1; y=2*j;
		display_BMP(1,map[x][y],map[x][y+1],j,2*(i-2)+1);
	}
}
 
void fun(uchar key,uchar dirction){
	uchar x,y;
	/* 0��ʾ�ո�1��ʾǽ��2��ʾ�ˣ�3��ʾ���ӣ�4��ʾĿ�ĵ� */
	if(key==1){//ֱ���ƶ�
		if(mat[cur_x][cur_y]==4)map[cur_x][cur_y]=4;
		else map[cur_x][cur_y]=0;
		update(cur_x,cur_y);
 
		cur_x+=dir[dirction][0]; cur_y+=dir[dirction][1];
		map[cur_x][cur_y]=2;
		update(cur_x,cur_y);
					
	}
	else if(key==2){//����ƶ�
		if(mat[cur_x][cur_y]==4)map[cur_x][cur_y]=4;
		else map[cur_x][cur_y]=0;
		update(cur_x,cur_y);
					
		cur_x+=dir[dirction][0]; cur_y+=dir[dirction][1];
		map[cur_x][cur_y]=2;
		update(cur_x,cur_y);
 
		x=cur_x+dir[dirction][0]; y=cur_y+dir[dirction][1];
		map[x][y]=3;
		update(x,y);
	}
}
 
void lcd_init(){
	uchar i,j;
 
	init_BMP();
 
	cur_x=1; cur_y=5;
	X=3;
	
	for(i=0;i<8;i++){
		for(j=0;j<8;j++)map[i][j]=mat[i][j];
	}
}
 
void main()
{
	uchar i,num,index=6;
	lcd_init();
	clear();
	show_map();
 
	/* 0��ʾ�ո�1��ʾǽ��2��ʾ�ˣ�3��ʾ���ӣ�4��ʾĿ�ĵ� */
	while(1){
		num=0;
		for(i=0;i<3;i++){
			if(map[ where[i][0] ][ where[0][1] ]==3)num++;
			else break;
		}
		if(num==X)goto WIN;
		if(key_up==0){
			delay(500);
			if(key_up==0){
				index=judge(cur_x,cur_y,0);
				fun(index,0);
			}while(!key_up);
		}
		else if(key_down==0){
			delay(500);
			if(key_down==0){
				index=judge(cur_x,cur_y,1);
				fun(index,1);
			}while(!key_down);
		}
		else if(key_left==0){
			delay(500);
			if(key_left==0){
				index=judge(cur_x,cur_y,2);
				fun(index,2);
			}while(!key_left);
		}
		else if(key_right==0){
			delay(500);
			if(key_right==0){
				index=judge(cur_x,cur_y,3);
				fun(index,3);
			}while(!key_right);
		}
	}
WIN:
	init_Hanzi();		
 
	write_LCD_command(0x98+4);
	for(i=0;i<8;i++)
	{
		write_LCD_data(table4[i]);
	}
	delay(1000);
	while(1); 
}