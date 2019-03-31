//��·ADC���ݲɼ�C�ļ�
#include"ADCS.h"
extern __IO uint16_t ADCConvertedValue[10];
//=========================================================================================
//��������


//ADC ��������ݼĴ���
#define ADC1_DR_Address    ((uint32_t)0x4001244C)
//ADC_DR(ADC�������ݼĴ���),ƫ����=0x4c  ADC1(0x40012400-0x400127ff)
//so ADC1_DR_Address=0x40012400+0x4c


//==========================================================================================
//ADC1_GPIO����
void ADC1_GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  //CH_10->PC0	CH_11->PC1	CH_12->PC2	CH_13->PC3
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	//adcģʽ������ģ������
  GPIO_Init(GPIOC, &GPIO_InitStructure);//ִ������Ĳ���

  //CH_0->PA0	CH_1->PA1	CH_2->PA2	CH_3->PA3
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	//adcģʽ������ģ������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//ִ������Ĳ���
  //ע�⣬�ڲ��¶ȶ�Ӧ 16ͨ���������ţ�ֻ�迪��adcʱ�Ӽ��ɡ�
  //�ڲ��ο���ѹ����Ӧ17 ͨ���������š�ֻ�迪��ʱ��
}

//============================================================================================
//����ADC��DMAʱ�ӿ���
 void ADC1_RCC_Configuration(void)
{
  /* Enable ADC1 and GPIOC clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA, ENABLE);
}

//===============================================================================================
//ADC1 
void ADC1_Init(void)
{
ADC_InitTypeDef ADC_InitStructure;

  /* ADC1 configuration ------------------------------------------------------*/
  //ADC����ģʽ	 �����˫��ģʽ
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  //ɨ��ģʽ���ڶ�ͨ���ɼ�
  //�������ȻҲ�Ƕ�ͨ�������ǲ��÷�DMAģʽ������ɨ��ģʽ���ȡ���ݲ���ʱ�������һ��ͨ����
  //���ݸ��Ǳ������ݣ����Բ��õ���ת��ģʽ�������ɼ���ɱ���󣬲������ݱ����ߣ��ſ�ʼ��һ��
  //ͨ���Ĺ���
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  //��������ת��ģʽ   ��ת���걾�飨������һ�����������¿�ʼִ��
  //����ڵ���ģʽ��ת��һ�κ�ͽ���
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  //��ʹ���ⲿ����ת��
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  //�ɼ������Ҷ���
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  //ת�����ͨ����Ŀ
  ADC_InitStructure.ADC_NbrOfChannel =1;
  //ִ��
  ADC_Init(ADC1, &ADC_InitStructure);
  
  //����ADCʱ�ӣ�ΪPCLK2��8��Ƶ����9Hz
  RCC_ADCCLKConfig(RCC_PCLK2_Div8);
  /* ADC1 regular channel11 configuration */ 
  //����ADC1��ͨ��11Ϊ55.5����������
  //Ĭ���飬adc1 ��ͨ��11������Ϊ1,55.5����
  
  
  

  //----------------------ʹ���¶ȴ�����----------------------------
  ADC_TempSensorVrefintCmd(ENABLE);

  
   ADC_Cmd(ADC1, ENABLE);
  // ADC_SoftwareStartConvCmd(ADC1, DISABLE);
  /* Enable ADC1 reset calibration register */ 
  //ʹ��ADC1�ĸ�λУ׼�Ĵ���  
  ADC_ResetCalibration(ADC1);
  /* Check the end of ADC1 reset calibration register */
  //�ȴ�У׼���
  while(ADC_GetResetCalibrationStatus(ADC1));

  /* Start ADC1 calibration */
  //ʹ��ADC1�Ŀ�ʼУ׼�Ĵ���
  ADC_StartCalibration(ADC1);
  /* Check the end of ADC1 calibration */
  //�ȴ����
  while(ADC_GetCalibrationStatus(ADC1));
  

}
//===============================================================================================
//˵����3��ͨ�������ݣ��ֱ𰴵���ת������ɣ���������ȫ�ֱ���ADCConvertedValue�����У�������������ʾ
//													  
void GetADCSConvertedValues()
{
	uint8_t i;
	for(i=0;i<3;i++)
	{
		switch(i)
		{
		
		case 0:	 //ͨ��10���ⲿ�ɱ�����ѹ�������������1��������
		ADC_RegularChannelConfig(ADC1, ADC_Channel_10,1, ADC_SampleTime_239Cycles5);break;
		case 1:	 //ͨ��11���ⲿ�ɱ�����ѹ�������������1��������
		ADC_RegularChannelConfig(ADC1, ADC_Channel_11,1, ADC_SampleTime_239Cycles5);break;
		case 2:	 //ͨ��12���ⲿ�ɱ�����ѹ�������������1��������
		ADC_RegularChannelConfig(ADC1, ADC_Channel_12,1, ADC_SampleTime_239Cycles5);break;
		
		
		}
		
  		ADC_Cmd(ADC1, ENABLE); //ʹ��ADC1
  		ADC_SoftwareStartConvCmd(ADC1, ENABLE);	//�����������úõ�һ��ͨ��������ת��	
		while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET);//�ȴ�EOC��λ
		ADCConvertedValue[i]=ADC_GetConversionValue (ADC1);	//�����ݼĴ�����ֵ����

		ADC_ClearFlag(ADC1, ADC_FLAG_EOC);	   //���EOC��DMAʱ�����ݣ�Ӳ���Զ����
        ADC_SoftwareStartConvCmd(ADC1, DISABLE);
        ADC_Cmd(ADC1, DISABLE);
	}
}
