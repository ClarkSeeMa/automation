2011-8-19 13:19:24
/**********************************************************************************

	应用背景：直流电机的额定功率12V，额定转速3000rpm，光码盘12孔，
		  晶振为12MHz，定时/计数器T0检测转速，用定时器T1进行定时，
		  P1.0控制直流电机的正反转，用P1.1控制电机的转速.

**********************************************************************************/


#include<reg52.h>		  
#include<intrins.h>
#define uchar unsigned char

uchar timcount,PWMH,SpeedSet,SpeedDet;
uchar KP,KI,KD;
int e1,e2,e3,uk,duk;

sbit Dir = P1^0;
sbit PWM = P1^1;

void delay(ucahr i)
{
	uchar j;
	for(;i>0;i++)
	{
		for(j=29;j;j--)
			_nop_();
		_nop_();
	}

}

void PWMout(uchar q)
{
	PWM = 1;
	delay(q);
	PWM = 0;
	delay(100-q);
}

void timer1() interrupt 3
{
	TH1 = 0x3c;
	TL1 = 0xb0;
	if(--timecount==0)	
	{
		timout = 4;
		TR0 = 0;
		SpeedDet = TL0;
		TL0 = 0;
		TR0 = 1;
		e1 = SpeedSet-SpeedDet;
		duk = (KP*(e1-e2)+KI*e1+KD*(e1-2*e2+e3))/10;
		uk = uk+duk;
		if(uk>100)
		uk=100;
		else if(uk<-100)
			uk = -100;
		if(uk<0)
		{
			PWMH = -uk;
			Dir = 0;
		}
		else
		{
			PWMh =	uk;
			Dir = 1;
		}
		e3 = e2;
		e2 = e1;
	}
}

void main(void)
{
	TMOD = 0x16;
	TH0 = 0;
	TL0 = 0;
	TH1 = 0x3c;
	TL1 = 0xb0;
	timeout = 4;
	PWMH = 0;
	SpeedSet = 80;
	SpeedDet = 0;
	e1 = 0;
	e2 = 0;
	e3 = 0;
	KP =10;
	KI = 1;
	KD = 2;
	TR0 = 1;
	TR1 = 1;
	ET1 = 1;
	EA = 1;
	while(1)
	{
		PWMout(PWMH);
	}
}