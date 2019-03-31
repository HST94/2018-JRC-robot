
#include "receive.h"
#include "GPS.h"
#include "string.h"
#include "misc.h"
#include "lcd.h"
#include "touch.h"
#include "distant.h"
#include "time_test.h"
#include "speech.h"

char  warning[]={"���棡��"};
char  warn_msg[]={"ǰ��    ����ͣ��"};
char  warn_hlp[]={"ǰ��    �����æ"};
char  warn_msg_sp[]={"ǰ����ͣ����"};
char  warn_hlp_sp[]={"ǰ���������æ��"};
char  warn_msg_near[]={"С����ͣ����"};
char  warn_hlp_near[]={"�����������æ��"};
char  warn_msg_cross[]={"С�ĺ�����"};

extern char  init4[];
extern GPS_INFO   GPS;  //��display.c�ж��壬ʹ��ʱҪ��extern
extern char rev_buf[80];
extern char rev_lolt[26];
extern uchar gps_flag;      //GPS�����־
extern uchar volatile rev_GPS_stop;     //����GPSֹͣ��־
extern uchar volatile rev_ZigBee_stop;	//����ZigBee������־
extern uchar volatile rev_ZigBee_start;
extern uchar rev_ZigBee_flag;		//����ZigBee����
extern uchar volatile Park_rq;
extern uchar volatile GPS_num;
//extern uchar volatile time_warn;
extern uchar volatile Help_rq;
extern uchar volatile cross_wr;
extern uchar volatile ZigBee_num;

extern double speed,longitude,latitude,direct;
extern char direction[];
extern char direction_ch[];
extern int flag_CE,flag_CL;
extern uchar NS,EW;


uchar  GPS_Valid=0;			//��Ч����GPS��־
uchar  ZigBee_Valid=0;		//��Ч����ZigBee��־
extern u8 warn_flag;
extern u8 speed_show;
extern u8 flag_flag;
int GPS_TIME=0;

void delay2(int x)
{
 int i,j;
 for(i=0;i<1000;i++)
 for(j=0;j<x;j++);
}



void receive_RMC()
{
	
		GPS_Valid= GPS_RMC_Parse(rev_buf, &GPS);
	
		if (GPS_Valid) //����GPRMC
		{
			speed=GPS.speed;
			longitude=GPS.longitude/100;	//ȱ������
			latitude=GPS.latitude/100;
			direct=GPS.direction;
			NS=GPS.NS;
			EW=GPS.EW;
			
			GPS_TIME++;	  
		
		}
		
	gps_flag = 0;
	rev_GPS_stop  = 0;	
}


void receive_ZigBee()		   //����һ֡����Ҫ1.04ms
{

 }
