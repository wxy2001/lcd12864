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
uchar code table2[]="推箱子";
uchar code table3[]="BY laoda";
uchar code table4[]="You Win!";
 
uchar code tmp[]={//空格
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
};
/* 0表示空格，1表示墙，2表示人，3表示箱子，4表示目的地 */
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
 
uchar code Qiang[]={//方格，有间隙
0xff,0x81,0x81,0x81,0x81,0x81,0x81,0xff
};
 
uchar code Ren[]={//人
0xff,0xff,0xe3,0xc5,0xb1,0xc5,0xe3,0xff
};
 
uchar code Xiang[]={//箱子  
0xff,0x42,0x24,0x18,0x18,0x24,0x42,0xff  
};
 
uchar code Mudi[]={//目的地  
0xff,0xff,0xe7,0x81,0x81,0xe7,0xff,0xff  
};
 
void delay(uint i) //延时函数 
{
	while(--i);
}
 
void read_busy()//读忙函数。每次读写都要进行读忙操作 
{
	RS=0;
	RW=1;
	EN=1;
	while(LCD_data & 0x80);//最高位为BF位 DB7---BF 
	EN=0;
}
 

void write_LCD_command(uchar value)//写指令函数 
{
	read_busy();
	RS=0;
	RW=0;
	EN=0;		//EN从1---0锁存数据 
	LCD_data=value;
	delay(20);
	EN=1;
	delay(20);
	EN=0;
} 
 
void write_LCD_data(uchar value)//写数据函数 
{
	read_busy();
	RS=1;
	RW=0;
	EN=0;		//EN从1---0锁存数据 
	LCD_data=value;
	delay(20);
	EN=1;
	delay(20);
	EN=0;
} 
void init_BMP()	 
{
	write_LCD_command(0x38);//CL=1--8位。扩充指令(RE=1)，绘图打开(G=1)
	delay(100);	//适当延时 
	write_LCD_command(0x38);
	delay(37);
	write_LCD_command(0x38);//8位(CL=1)，扩充指令(RE=1)，绘图打开(G=1) 
	delay(100);
	write_LCD_command(0x0c);
	delay(100);
	write_LCD_command(0x06);
	delay(100);
	write_LCD_command(0x01);//清屏指令 
	delay(1000);
}
 
void init_Hanzi()				//8位并口方式LCD1864初始化函数 
{
	delay(4000);			//等待时间>40ms 
	write_LCD_command(0x30);//功能设定：8位数据、基本指令操作 
	delay(100);				//等待时间>100us 
	write_LCD_command(0x30);//功能设定：8位数据、基本指令操作 
	delay(37);				//等待时间>37us 
	write_LCD_command(0x0C);//显示设定：整体显示、游标关、不反白 
	delay(100);				// 等待时间>100us 
	//write_LCD_command(0x01);//清屏指令 
	//delay(10000);			//等待时间>10ms 
	write_LCD_command(0x06);//进入点设定：地址指针加1 
}
 
void display_BMP(uchar fuck,uchar name1,uchar name2,uchar y,uchar x)	
{
	uchar shit,i;
	if(fuck==0)shit=0x80;
	else shit=0x88;
	for(i=0;i<8;i++)		
	{	
		write_LCD_command(0x80+i+8*x);//先送垂直地址 
		write_LCD_command(shit+y);  //再送水平地址 ----显示图片的上半部分 
 
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
 
void clear()//图形模式下，没有清屏函数，手动全部送0						
{
	uchar i,j;
	for(i=0;i<32;i++){
		write_LCD_command(0x80+i);//先送垂直地址 
		write_LCD_command(0x80);  //再送水平地址 ----显示图片的上半部分 
		for(j=0;j<16;j++)write_LCD_data(0);
	}
	for(i=0;i<32;i++){
		write_LCD_command(0x80+i);	//先送垂直地址
		write_LCD_command(0x88);	//显示图片的下半部分 
		for(j=0;j<16;j++)write_LCD_data(0);
	}
}
 
uchar judge(int x,int y,int id){							//id 表示方向数组的行标，0,1,2,3 分别表示上下左右
	int xx,yy,xxx,yyy;
	xx=x+dir[id][0]; yy=y+dir[id][1];
	if(map[xx][yy]==0 || map[xx][yy]==4)return 1;			   //1表示前面是 空格 或者 目的地，就是可以直接移动
	else if(map[xx][yy]==1)return 0;						   //0表示无法移动
	else if(map[xx][yy]==3){
		xxx=xx+dir[id][0]; yyy=yy+dir[id][1];
		if(map[xxx][yyy]==1 || map[xxx][yyy]==3)return 0;
		else if(map[xxx][yyy]==0 || map[xxx][yyy]==4)return 2; //2表示需要间接移动，先把前面的箱子移动一，再让人移动一
	}return 0;
}
 
void update(uchar a,uchar b){
	uchar i,j,x,y;
 
	write_LCD_command(0x3E);//8位(CL=1)，扩充指令(RE=1)，绘图打开(G=1) 
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
	/* 0表示空格，1表示墙，2表示人，3表示箱子，4表示目的地 */
	if(key==1){//直接移动
		if(mat[cur_x][cur_y]==4)map[cur_x][cur_y]=4;
		else map[cur_x][cur_y]=0;
		update(cur_x,cur_y);
 
		cur_x+=dir[dirction][0]; cur_y+=dir[dirction][1];
		map[cur_x][cur_y]=2;
		update(cur_x,cur_y);
					
	}
	else if(key==2){//间接移动
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
 
	/* 0表示空格，1表示墙，2表示人，3表示箱子，4表示目的地 */
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