#ifndef __ADC_H
#define __ADC_H	
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//Mini STM32������
//ADC ��������			   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2010/6/7 
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved					   
//////////////////////////////////////////////////////////////////////////////////	 

#include "stm32f10x.h"

extern float Az,gyroscope_rate,ZAdMax,ZAdMin,AD_gyro,Gyro_b,anglez,g_nCarGyroVal,angle_kalman,angle_dot_kalman;
extern int Gyro_zero,z;

#define ADC_CH0  0 //ͨ��0
#define ADC_CH1  1 //ͨ��1
#define ADC_CH2  2 //ͨ��2
#define ADC_CH3  3 //ͨ��3	   
 
void Adc_Init(void);
u16  Get_Adc(u8 ch);
void acc_z(void);  
 
#endif 















