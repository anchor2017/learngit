#include <reg52.h>
#define uint unsigned int
#define uchar unsigned char
sbit S4 = P3^2;

/*��ʱ����*/
void delay(uint z)
{
	uint x, y;
	for(x=z;x>0;x--)
		for(y=114;y>0;y--); 
}

/*��ˮ����ʾģʽ*/
void leddisplay(uint y)
{
	uint x;
	if(y == 1)	 //��������ˮ�ƣ��͵�ƽ����
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

/*�жϷ������⹦�ܼĴ�������*/
void initTimer()
{
	TMOD=0x01;	//����ģʽ
	TH0=0x4c;
	TL0=0x00; //0.5����
	ET0=1;	//����ʱ��0
	TR0=1;	//������ʱ��
	EX0=1;	//���ⲿ�ж�0
	IT0=0;	//�͵�ƽ������ʽ
	EA=1;	//�����ж�
}

/*������*/
int main()
{
   	initTimer();
	while(1);
}

void Timer0() interrupt 1	//��ʱ���жϣ��жϵȼ�1
{
	static uint i, mode;
	i++;
	if(i==1)		 //20��	 
	{
		if(mode == 0)	 //��������ˮ�ƣ��͵�ƽ����
		{
			P1=0xfd; //���	1111 1101
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

void int0() interrupt 0	   //�ⲿ�жϣ��жϵȼ�0
{	
	 static uint mode;
	 if(S4==0)
	 {
	 	delay(10);
		if(!S4)
		{
			switch(mode)
			{
				case 0:			   //ģʽ1
					leddisplay(1);
					mode++;		 //mode = 1
					break;
				case 1:			  //ģʽ2
					leddisplay(2);
					mode++;		 //mode = 2
					break;
				case 2:			  //ģʽ3
					leddisplay(3);
					mode=0;		//mode = 0
					break;			
			}
		}
	 }
}