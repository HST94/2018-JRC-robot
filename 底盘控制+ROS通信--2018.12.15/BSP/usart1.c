#include "FIFO.h"
#include "main.h"

#define LEG_BUF_SIZE 15
#define Front_Leg_Middle 	168 //2048
#define Back_Leg_Middle 	169 //2048

//��ʼ��ADC															   
void  Adc_Init(void)
{    
  GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//ʹ��GPIOAʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //ʹ��ADC1ʱ��

  //�ȳ�ʼ��ADC1ͨ��5 IO��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;//PA5 ͨ��5
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
  GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��  
 
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1��λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//��λ����	 
 
	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//���������׶�֮����ӳ�5��ʱ��
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMAʧ��
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//��ʼ��
	
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λģʽ
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;//��ɨ��ģʽ	
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//�ر�����ת��
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ������⣬ʹ���������
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���	
  ADC_InitStructure.ADC_NbrOfConversion = 1;//1��ת���ڹ��������� Ҳ����ֻת����������1 
  ADC_Init(ADC1, &ADC_InitStructure);//ADC��ʼ��
	
 
	ADC_Cmd(ADC1, ENABLE);//����ADת����	

}				  
//���ADCֵ
//ch: @ref ADC_channels 
//ͨ��ֵ 0~16ȡֵ��ΧΪ��ADC_Channel_0~ADC_Channel_16
//����ֵ:ת�����
u16 Get_Adc(u8 ch)   
{
	  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles );	//ADC1,ADCͨ��,480������,��߲���ʱ�������߾�ȷ��			    
  
	ADC_SoftwareStartConv(ADC1);		//ʹ��ָ����ADC1�����ת����������	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
}
//��ȡͨ��ch��ת��ֵ��ȡtimes��,Ȼ��ƽ�� 
//ch:ͨ�����
//times:��ȡ����
//����ֵ:ͨ��ch��times��ת�����ƽ��ֵ
u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 

extern float forward_leg_angle,back_leg_angle;

void get_leg_angle(void)
{
	static u16 forward_leg_angle_buf[30],back_leg_angle_buf[30];
	static u8 forward_leg_count,back_leg_count;
	u8 i;
	u32 tem_sum=0;
	
	forward_leg_angle = Get_Adc(12) ; //max:AC min:46  change mode angle:5A  leave ground angle: 8A
	back_leg_angle = Get_Adc(13) ;  //max:A8  min:47  change mode angle:5A  leave ground angle: 8E
	
	forward_leg_angle_buf[forward_leg_count++] = forward_leg_angle;
	back_leg_angle_buf[back_leg_count++] = back_leg_angle;
	
	if(forward_leg_count>=LEG_BUF_SIZE) forward_leg_count=0;
	if(back_leg_count>=LEG_BUF_SIZE) back_leg_count=0;
	
	tem_sum=0;
	for(i=0;i<LEG_BUF_SIZE;i++)
	{
		tem_sum += forward_leg_angle_buf[i];
	}
	forward_leg_angle = tem_sum/LEG_BUF_SIZE;
	
	tem_sum=0;
	for(i=0;i<LEG_BUF_SIZE;i++)
	{
		tem_sum += back_leg_angle_buf[i];
	}
	back_leg_angle = tem_sum/LEG_BUF_SIZE;
	
	forward_leg_angle = 1.0*forward_leg_angle/4096*360*3.3/5;
	back_leg_angle = 1.0*back_leg_angle/4096*360*3.3/5;
	
	forward_leg_angle = Front_Leg_Middle - forward_leg_angle;
	back_leg_angle = Back_Leg_Middle - back_leg_angle;
	
//	if((forward_leg_angle>1400&&forward_leg_angle<2800) || (back_leg_angle>1400&&back_leg_angle<2800))
//	{
//		robot_mode = 2;
//	}
//	else
//	if((forward_leg_angle>1000&&forward_leg_angle<1300) && (back_leg_angle>1000&&back_leg_angle<1300))
//	{
//		robot_mode = 1;
//	}
//	else
//	{
//		robot_mode = 2;
//	}
	
//	if((forward_leg_angle>1400) || (back_leg_angle>1400))
//	{
//		robot_mode = 2;
//	}
//	else
//	{
//		robot_mode = 1;
//	}
	
}

