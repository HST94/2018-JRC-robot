#ifndef _LCD5110_H
#define _LCD5110_H
//#define    sce  PORTA_PA1  //Ƭѡ���ţ�оƬʹ��
//#define    res  PORTA_PA0  //��λ���ţ��ⲿ��λ�����0��λ
//#define    dc   PORTA_PA2  //����/�����л��ţ�1д���ݣ�0дָ��
//#define    sdin PORTA_PA3  //�������룺�������������
//#define    sclk PORTA_PA4  //ʱ�����ţ�����ʱ�������
/*
#define    res=1    GPIO_WriteBit(GPIOE, GPIO_Pin_2, Bit_SET);//��λ,0��λ
#define    res=0    GPIO_WriteBit(GPIOE, GPIO_Pin_2, Bit_RESET);//��λ,0��λ

#define    sce=1    GPIO_WriteBit(GPIOE, GPIO_Pin_3, Bit_SET);//Ƭѡ
#define    sce=0    GPIO_WriteBit(GPIOE, GPIO_Pin_3, Bit_RESET);//Ƭѡ

#define    dc=1     GPIO_WriteBit(GPIOE, GPIO_Pin_4, Bit_SET);//1д���ݣ�0дָ��
#define    dc=0     GPIO_WriteBit(GPIOE, GPIO_Pin_4, Bit_RESET);//1д���ݣ�0дָ��

#define    sdin=1   GPIO_WriteBit(GPIOE, GPIO_Pin_5, Bit_SET);//����
#define    sdin=0   GPIO_WriteBit(GPIOE, GPIO_Pin_5, Bit_RESET);//����

#define    sclk=1   GPIO_WriteBit(GPIOE, GPIO_Pin_6, Bit_SET);//ʱ��
#define    sclk=0   GPIO_WriteBit(GPIOE, GPIO_Pin_6, Bit_RESET);//ʱ��
//sbit P27=P2^7;
 */

    

//6*16�ַ�
//#pragma CODE_SEG DEFAULT
extern unsigned char shuzi[];
extern unsigned char  hanzi[];
extern unsigned char  zimu1[] ;
extern unsigned char  zimu2[] ;
extern unsigned char  fuhao[] ;
extern unsigned char F6x8[][6];

//void delayms(unsigned  int ii);
void LCD_write_byte(unsigned char dt, unsigned char command);
void LCD_init(void); //LCD��ʼ��
void LCD_set_XY(unsigned char X, unsigned char Y);
void LCD_clear(void); //LCD����
void LCD_clearxy(unsigned char xLcd,unsigned char yLcd);
void LCD_write_shu(unsigned char* shuzi,unsigned char row, unsigned char page,unsigned char c);
void LCD_write_hanzi(unsigned char row, unsigned char page,unsigned char c);
 
 void LCD_tou(void);
 
 void LCD_P6x8Str(unsigned char xLcd,unsigned char yLcd,unsigned char ch[]);
 void LCD_P6x8num_i(unsigned char xLcd,unsigned char yLcd,int num);
 
//void LCD_WrDat(byte data);
// void LCD_WrCmd(byte cmd);
// void LCD_Set_Pos(byte x, byte y);
 
#endif