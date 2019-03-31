/***********************************************************
**
** ׷��ʵ���� SYN6288 ���������ϳɿ�������Գ�����ʾ��
**
** ���������� AT89S51 ��Ƭ����������������Ƭ���������޸�
**
** ��    �ߣ�CIKY & SPP From Dreamon-II Labs.
**
** ��Ȩ���У�׷�μ���������Ӽ���ʵ����
**
** ����޸ģ�2012-8-21
**
***********************************************************/
#include "string.h"
#include "stm32f10x_usart.h"
#include "speech.h"



#define HEADLEN       5  //���ݰ�ͷ�ĳ���
#define BKM_OFFSET    4  //������������ƫ��
#define LEN_OFFSET    2  //�����ֽڵ�ƫ������һ�㲻�ᳬ��255�ֽڣ����ֻʹ��1�ֽڳ��ȣ�
#define BKM_MAX      15	 //������������

//*****************************************************
//sbit BUSY = P1^7;        //��������SYN6288��BUSY���Ź̶����ӵ�CPU��P3.7�˿�
uchar nBkm = 0x00;

uchar send_speech[255]=" ";
//*****************************************************



//*****************************************************

//���ݰ�ͷ��0xFD + 2�ֽڳ��� + 1�ֽ������� + 1�ֽ��������)

uchar head[] = {0xfd,0x00,0x00,0x01,0x00};
void Speech(char *buf)
{
	u8 i=0;
	uchar len = 0x00;
	while(buf[++len]);
		for(i = 0; i < HEADLEN; i++)
	{
		if(i == BKM_OFFSET)
			send_speech[i]= nBkm << 3; //д�뱳������
		else if(i == LEN_OFFSET)
			send_speech[i] = len + 3;
		else
			send_speech[i] = head[i];
   	}
	strcpy(&send_speech[5],buf);
	USART_ITConfig(USART3,USART_IT_TXE,ENABLE);
	

	//�������ݰ�ͷ(0xFD + 2�ֽڳ��� + 1�ֽ������� + 1�ֽ��������)
}


/**************оƬ��������*********************/
uint8_t SYN_StopCom[]={0xFD,0X00,0X02,0X02,0XFD};//ֹͣ�ϳ�
uint8_t SYN_SuspendCom[]={0XFD,0X00,0X02,0X03,0XFC};//��ͣ�ϳ�
uint8_t SYN_RecoverCom[]={0XFD,0X00,0X02,0X04,0XFB};//�ָ��ϳ�
uint8_t SYN_ChackCom[]={0XFD,0X00,0X02,0X21,0XDE};//״̬��ѯ
uint8_t SYN_PowerDownCom[]={0XFD,0X00,0X02,0X88,0X77};//����POWER DOWN ״̬����

/***********************************************/



/***********************************************************
* ��    �ƣ�  YS-SYN6288 �ı��ϳɺ���
* ��    �ܣ�  ���ͺϳ��ı���SYN6288оƬ���кϳɲ���
* ��ڲ�����Music(��������ѡ��):0�ޱ������֡�1-15����ر�������
            *HZdata:�ı�ָ����� 
* ���ڲ�����
* ˵    ���� ������ֻ�����ı��ϳɣ��߱���������ѡ��Ĭ�ϲ�����9600bps��					 
* ���÷��������� SYN_FrameInfo��0�����������ӿƼ�������
**********************************************************/
void SYN_FrameInfo(uint8_t Music,uint8_t *HZdata)
{
/****************��Ҫ���͵��ı�**********************************/ 
		 unsigned  char  Frame_Info[5];
         unsigned  char  HZ_Length;  
		 unsigned  char  ecc  = 0;  			//����У���ֽ�
	     unsigned  int i=0; 
		 HZ_Length =strlen(HZdata); 			//��Ҫ�����ı��ĳ���
 
/*****************֡�̶�������Ϣ**************************************/           
		 Frame_Info[0] = 0xFD ; 			//����֡ͷFD
		 Frame_Info[1] = 0x00 ; 			//�������������ȵĸ��ֽ�
		 Frame_Info[2] = HZ_Length + 3; 		//�������������ȵĵ��ֽ�
		 Frame_Info[3] = 0x01 ; 			//���������֣��ϳɲ�������		 		 
		 Frame_Info[4] = 0x01 | Music<<4 ;  //����������������������趨

/*******************У�������***************************************/		 
		 for(i = 0; i<5; i++)   				//���η��͹���õ�5��֡ͷ�ֽ�
	     {  
	         ecc=ecc^(Frame_Info[i]);		//�Է��͵��ֽڽ������У��
		    USART_SendData(USART3,Frame_Info[i]);
		    while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET){}//�ȴ����ͽ���	
	     }

	   	 for(i= 0; i<HZ_Length; i++)   		//���η��ʹ��ϳɵ��ı�����
	     {  
	         ecc=ecc^(HZdata[i]); 				//�Է��͵��ֽڽ������У��
		    USART_SendData(USART3,HZdata[i]);
		    while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET){}//�ȴ����ͽ���		
	     }		 
/*******************����֡��Ϣ***************************************/		  
		 //PrintCom_len(Frame_Info,5); //����֡����
		 //PrintCom_len(HZdata,HZ_Length); //�����ı�
		 //UARTSendByte(ecc);    //���;�����
		 USART_SendData(USART3,ecc);
		 while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET){}//�ȴ����ͽ���
	
}


/***********************************************************
* ��    �ƣ� void  main(void)
* ��    �ܣ� ������	�������
* ��ڲ����� *Info_data:�̶���������Ϣ���� 
* ���ڲ�����
* ˵    �����������������ã�ֹͣ�ϳɡ���ͣ�ϳɵ����� ��Ĭ�ϲ�����9600bps��					 
* ���÷�����ͨ�������Ѿ�������������������á� 
**********************************************************/
void YS_SYN_Set(uint8_t *Info_data)
{
	uint8_t Com_Len;
	Com_Len =strlen(Info_data);
	//PrintCom_len(Info_data,Com_Len);
}



