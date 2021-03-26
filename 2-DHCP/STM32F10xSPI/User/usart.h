#ifndef __USART_H
#define __USART_H
#include <stm32f10x.h>
#ifndef USART_C_//���û�ж���
#define USART_Ex_ extern
#else
#define USART_Ex_
#endif

#include "BufferManage.h"
#include "LoopList.h"

/****************************����1****************************/
/*
����1��������:�������
����1��������:���ζ���
*/
USART_Ex_ buff_manage_struct buff_manage_struct_usart1_recv;//���ڽ��ջ������
USART_Ex_ rb_t rb_t_usart1_send;//����1���ͻ��滷�ζ���

#define Usart1RecvBuffLen 1024  //����1�������������С
#define Usart1RecvBufferMemoryLen 1024 //���ڽ��ջ�������С
#define Usart1RecvBufferMemoryCopyLen 500 //��ȡ�����������ݵ������С
#define Usart1RecvBufferMemoryManageLen 50 //���ڽ��ջ���������
#define Usart1SendBufflen 1024  //����1�������ݻ����С

USART_Ex_ unsigned char Usart1RecvBuff[Usart1RecvBuffLen];  //�������ݻ���
USART_Ex_ u32  Usart1RecvCnt;//����1���յ������ݸ���

USART_Ex_ u8   Usart1RecvBufferMemory[Usart1RecvBufferMemoryLen];//�������ݻ�����
USART_Ex_ u8   Usart1RecvBufferMemoryCopy[Usart1RecvBufferMemoryCopyLen];//��ȡ�����������ݵ�����
USART_Ex_ u32  Usart1RecvBufferMemoryManage[Usart1RecvBufferMemoryManageLen];//��������

USART_Ex_ unsigned char Usart1SendBuff[Usart1SendBufflen];  //����1�������ݻ���

/****************************����2****************************/
/*
����2��������:�������
����2��������:��ͨ�ж�
*/
USART_Ex_ buff_manage_struct buff_manage_struct_usart2_recv;//���ڽ��ջ������

#define Usart2RecvBuffLen 1024 //�������ݻ���
#define Usart2RecvBufferMemoryLen 1024 //�������ݻ�����
#define Usart2RecvBufferMemoryCopyLen 500 //��ȡ������������
#define Usart2RecvBufferMemoryManageLen 50 //��������

USART_Ex_ unsigned char Usart2RecvBuff[Usart2RecvBuffLen];  //�������ݻ���
USART_Ex_ u32  Usart2RecvCnt;//���ڽ��յ������ݸ���

USART_Ex_ u8   Usart2RecvBufferMemory[Usart2RecvBufferMemoryLen];//�������ݻ�����
USART_Ex_ u8   Usart2RecvBufferMemoryCopy[Usart2RecvBufferMemoryCopyLen];//��ȡ������������
USART_Ex_ u32  Usart2RecvBufferMemoryManage[Usart2RecvBufferMemoryManageLen];//��������

USART_Ex_ unsigned char *Usart2SendData;//�����жϷ������ݵĵ�ַ
USART_Ex_ u32   Usart2SendDataCnt;//�����жϷ������ݵĸ���

/****************************����3****************************/
/*
����3��������:�������
����3��������:�������+��ͨ�ж�
*/
USART_Ex_ buff_manage_struct buff_manage_struct_usart3_recv;//���ڽ��ջ������

#define Usart3RecvBuffLen 1024 //�������ݻ���
#define Usart3RecvBufferMemoryLen 1024 //�������ݻ�����
#define Usart3RecvBufferMemoryCopyLen 500 //��ȡ������������
#define Usart3RecvBufferMemoryManageLen 50 //��������

USART_Ex_ unsigned char Usart3RecvBuff[Usart3RecvBuffLen];  //�������ݻ���
USART_Ex_ u32  Usart3RecvCnt;//���ڽ��յ������ݸ���

USART_Ex_ u8   Usart3RecvBufferMemory[Usart3RecvBufferMemoryLen];//�������ݻ�����
USART_Ex_ u8   Usart3RecvBufferMemoryCopy[Usart3RecvBufferMemoryCopyLen];//��ȡ������������
USART_Ex_ u32  Usart3RecvBufferMemoryManage[Usart3RecvBufferMemoryManageLen];//��������

USART_Ex_ unsigned char *Usart3SendData;//�����жϷ������ݵĵ�ַ
USART_Ex_ u32   Usart3SendDataCnt;//�����жϷ������ݵĸ���

/*����3��������*/
USART_Ex_ buff_manage_struct buff_manage_struct_usart3_send;//���ڷ��ͻ������

#define Usart3SendBufferMemoryLen 1024 //���ݻ�����
#define Usart3SendBufferMemoryCopyLen 500 //��ȡ������������
#define Usart3SendBufferMemoryManageLen 50 //��������
USART_Ex_ u8   Usart3SendBufferMemory[Usart3SendBufferMemoryLen];//���ݻ�����
USART_Ex_ u8   Usart3SendBufferMemoryCopy[Usart3SendBufferMemoryCopyLen];//��ȡ������������
USART_Ex_ u32  Usart3SendBufferMemoryManage[Usart3SendBufferMemoryManageLen];//��������


void UsartOutStrIT(USART_TypeDef* USARTx,unsigned char *c,uint32_t cnt);
void uart_init(u32 bound1,u32 bound2,u32 bound3);//���ڳ�ʼ��
void UsartOutStr(USART_TypeDef* USARTx,unsigned char *c,uint32_t cnt);//���ڷ����ַ�������
void UsartOutChar(USART_TypeDef* USARTx,unsigned char c);//���ڷ���һ���ֽ�
#endif


