/*******************************************************************************
 * �ļ���  ��rtc.c
 * ����    ������STM32��RTCʵ��һ�����׵ĵ���ʱ�ӡ�
 *           ��ʾ��ʽΪ Time: XX : XX : XX(ʱ���֣���)��         
 * ʵ��ƽ̨��CHD1807-STM32������
 * ��汾  ��ST3.5.0
 
**********************************************************************************/
#include "rtc.h"
#include "stdio.h"

/* ���жϱ�־���������ж�ʱ��1����ʱ�䱻ˢ��֮����0 */
__IO uint32_t TimeDisplay;	

/*
 * ��������NVIC_Configuration
 * ����  ������RTC���жϵ����ж����ȼ�Ϊ1�������ȼ�Ϊ0
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* Configure one bit for preemption priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	/* ʹ�ܣңԣ��ж� */
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/*
 * ��������RTC_Configuration
 * ����  ������RTC
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void RTC_Configuration(void)
{
	/* ʹ��PWR��BKPʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	
	/* BKP ʹ�� */
	PWR_BackupAccessCmd(ENABLE);
	
	/* ����BKP�Ĵ���*/
	BKP_DeInit();
	
	/* ʹ�� LSE */
	RCC_LSEConfig(RCC_LSE_ON);
	/* �ȴ�LES׼�� */
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
	{}
	
	/* ѡ��LES��ΪRTCʱ�� */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	
	/* ʹ��RTCʱ��*/
	RCC_RTCCLKCmd(ENABLE);
	
	/* �ȴ�RTCͬ�� */
	RTC_WaitForSynchro();
	
	/* �ȴ�RTC����������� */
	RTC_WaitForLastTask();
	
	/* ʹ��RTC�� */
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
	

	RTC_WaitForLastTask();
	
	/* �趨�ңԣ�Ԥ�궨�� */
	RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */
	
	
	RTC_WaitForLastTask();
}


/*
 * ��������Time_Regulate
 * ����  �������û��ڴ��������������ʱ��ֵ������ֵ������
 *         RTC �����Ĵ����С�
 * ����  ����
 * ���  ���û��ڴ��������������ʱ��ֵ����λΪ s
 * ����  ���ڲ�����
 */
uint32_t Time_Regulate(void)
{
	uint32_t Tmp_HH = 0xFF, Tmp_MM = 0xFF, Tmp_SS = 0xFF;
	
	printf("\r\n****************����ʱ��*********************");
	printf("\r\n  ������Сʱ");
	
	while (Tmp_HH == 0xFF)
	{
		Tmp_HH = USART_Scanf(23);
	}
	printf(":  %d", Tmp_HH);
	printf("\r\n  �����÷���");
	while (Tmp_MM == 0xFF)
	{
		Tmp_MM = USART_Scanf(59);
	}
	printf(":  %d", Tmp_MM);
	printf("\r\n  ��������");
	while (Tmp_SS == 0xFF)
	{
		Tmp_SS = USART_Scanf(59);
	}
	printf(":  %d", Tmp_SS);
	printf("\r\n");
	/* Return the value to store in RTC counter register */
	return((Tmp_HH*3600 + Tmp_MM*60 + Tmp_SS));
}


/*
 * ��������Time_Adjust
 * ����  ��ʱ�����
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void Time_Adjust(void)
{
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
	/* Change the current time */
	RTC_SetCounter(Time_Regulate());
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
}


/*
 * ��������Time_Display
 * ����  ����ʾ��ǰʱ��ֵ
 * ����  ��-TimeVar RTC����ֵ����λΪ s
 * ���  ����
 * ����  ���ڲ�����
 */	
void Time_Display(uint32_t TimeVar)
{
	uint32_t THH = 0, TMM = 0, TSS = 0;
	
	/* Compute  hours */
	THH = TimeVar / 3600;
	/* Compute minutes */
	TMM = (TimeVar % 3600) / 60;
	/* Compute seconds */
	TSS = (TimeVar % 3600) % 60;
	
	printf(" Time: %0.2d:%0.2d:%0.2d\r", THH, TMM, TSS);
	printf("\n\r");
}


/*
 * ��������Time_Show
 * ����  ���ڴ�����������ʾ��ǰʱ��ֵ
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */   
void Time_Show(void)
{
	printf("\n\r");
	
	/* Infinite loop */
	while (1)
	{
		/* If 1s has paased */
		if (TimeDisplay == 1)
		{
			/* Display current time */
			Time_Display(RTC_GetCounter());
			TimeDisplay = 0;
		}
	}
}


/*
 * ��������USART_Scanf
 * ����  ���Ӵ������ֻ�ȡ��ֵ
 * ����  ��- value �û��ڴ����������������ֵ
 * ���  ����
 * ����  ���ڲ�����
 */ 
uint8_t USART_Scanf(uint32_t value)
{
	uint32_t index = 0;
	uint32_t tmp[2] = {0, 0};
	
	while (index < 2)
	{
		/* Loop until RXNE = 1 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
		{}
		tmp[index++] = (USART_ReceiveData(USART1));
		// �Ӵ��������������ȥ������ASCII��ֵ
		if ((tmp[index - 1] < 0x30) || (tmp[index - 1] > 0x39))
		{
			printf("\n\rPlease enter valid number between 0 and 9");
			index--;
		}
	}
	/* Calculate the Corresponding value */
	index = (tmp[1] - 0x30) + ((tmp[0] - 0x30) * 10);
	/* Checks */
	if (index > value)
	{
		printf("\n\rPlease enter valid number between 0 and %d", value);
		return 0xFF;
	}
	return index;
}

/*****************************END OF FILE******************************/
