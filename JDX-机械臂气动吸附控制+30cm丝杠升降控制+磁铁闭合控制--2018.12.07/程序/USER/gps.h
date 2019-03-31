#ifndef __GPS_H_
#define __GPS_H_

#include "stm32f10x.h"

#define uchar unsigned char
#define uint  unsigned int

typedef struct{
	int year;  
	int month; 
	int  day;
	int hour;
	int minute;
	int second;
}DATE_TIME;

typedef struct{
	char	str_lng[12];
	char 	str_lat[11];
	double 	lat_other;
	double 	lng_other;
	double  latitude;  //γ��
	double  longitude; //����
	/*int     latitude_Degree;	//��
	int		latitude_Cent;		//��
	int   	latitude_Second;    //��
	int     longitude_Degree;	//��
	int		longitude_Cent;		//��
	int   	longitude_Second;   //��*/
	float 	speed;      //�ٶ�
	char 	ch_dir[7];
	float 	dir_other;
	float 	direction;  //����
	float 	height_ground;    //ˮƽ��߶�
	float 	height_sea;       //���θ߶�
	u16     satellite;
	uchar 	NS;
	uchar 	EW;
	DATE_TIME D;
}GPS_INFO;

//void GPS_Init(void);
int GPS_RMC_Parse(char *line,GPS_INFO *GPS);
//int GPS_GGA_Parse(char *line,GPS_INFO *GPS);
//int GPS_GSV_Parse(char *line,GPS_INFO *GPS);
int GPS_OTH_Parse(char *line,GPS_INFO *GPS);
void Int_To_Str(int x,char *Str);

#endif  //__GPS_H_
