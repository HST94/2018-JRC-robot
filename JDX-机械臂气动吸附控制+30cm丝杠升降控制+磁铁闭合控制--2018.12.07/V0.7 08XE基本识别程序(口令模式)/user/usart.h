#ifndef __USART_H
#define __USART_H


void UartIni(void);//���ڳ�ʼ��
void UARTSendByte(uint8_t DAT);	//���ڷ���һ�ֽ�����
void PrintCom(uint8_t *DAT); //��ӡ�����ַ�������

void SYN_FrameInfo(uint8_t Music,uint8_t *HZdata);
void YS_SYN_Set(uint8_t *Info_data);
void PrintCom_len(uint8_t *DAT,uint8_t len);


#endif
