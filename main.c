#include <reg52.h>
#define uint unsigned int
#define uchar unsigned char
sbit S4 = P3^2;

/*延时函数*/
void delay(uint z)
{
	uint x, y;
	for(x=z;x>0;x--)
		for(y=114;y>0;y--); 
}

/*流水灯显示模式*/
void leddisplay(uint y)
{
	uint x;
	if(y == 1)	 //共阳极流水灯，低电平点亮
	{
		P1=0x7f;	//0111 1111
		for(x=0;x<7;x++)
		{
			P1=P1>>1;
			delay(100);
		}
	}
	else if(y == 2)
	{
		P1=0xfe;	  //1111 1110
		for(x=0;x<7;x++)
		{
			P1=P1<<1;
			delay(100);
		}
	}
	else if(y == 3)
	{
		P1=0x55;	//0101 0101
		delay(100);
	}
}

/*中断服务特殊功能寄存器配置*/
void initTimer()
{
	TMOD=0x01;	//工作模式
	TH0=0x4c;
	TL0=0x00; //0.5毫秒
	ET0=1;	//开定时器0
	TR0=1;	//启动定时器
	EX0=1;	//开外部中断0
	IT0=0;	//低电平触发方式
	EA=1;	//开总中断
}

/*主函数*/
int main()
{
   	initTimer();
	while(1);
}

void Timer0() interrupt 1	//定时器中断，中断等级1
{
	static uint i, mode;
	i++;
	if(i==1)		 //20秒	 
	{
		if(mode == 0)	 //共阳极流水灯，低电平点亮
		{
			P1=0xfd; //检测	1111 1101
			leddisplay(1);
			mode++;
		}
		else if(mode == 1)
		{
			leddisplay(2);
			mode++;
		}
		else if(mode == 2)
		{
			leddisplay(3);
			mode = 0;
		}
		i=0;
	}
}

void int0() interrupt 0	   //外部中断，中断等级0
{	
	 static uint mode;
	 if(S4==0)
	 {
	 	delay(10);
		if(!S4)
		{
			switch(mode)
			{
				case 0:			   //模式1
					leddisplay(1);
					mode++;		 //mode = 1
					break;
				case 1:			  //模式2
					leddisplay(2);
					mode++;		 //mode = 2
					break;
				case 2:			  //模式3
					leddisplay(3);
					mode=0;		//mode = 0
					break;			
			}
		}
	 }
}