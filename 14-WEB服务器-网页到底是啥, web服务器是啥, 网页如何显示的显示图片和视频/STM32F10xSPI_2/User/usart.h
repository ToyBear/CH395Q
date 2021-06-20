#ifndef __USART_H
#define __USART_H
#include <stm32f10x.h>
#ifndef USART_C_//���û�ж���
#define USART_Ex_ extern
#else
#define USART_Ex_
#endif


#include "LoopList.h"

/****************************����1****************************/
//���ջ��ζ���
USART_Ex_ rb_t rb_t_usart1_read;
#define rb_t_usart1_read_buff_len 1024
USART_Ex_ unsigned char rb_t_usart1_read_buff[rb_t_usart1_read_buff_len];
//�ӻ��濽������ʹ��
USART_Ex_ unsigned char usart1_read_buff_copy[rb_t_usart1_read_buff_len];
//���ռ���
USART_Ex_ int usart1_read_count;


//���ͻ��ζ���
USART_Ex_ rb_t rb_t_usart1_send;
#define rb_t_usart1_send_buff_len 1500
USART_Ex_ unsigned char rb_t_usart1_send_buff[rb_t_usart1_send_buff_len];
USART_Ex_ unsigned char rb_t_usart1_send_byte;//������ȡ���ζ���1���ֽ�

//�����жϱ�־
USART_Ex_ unsigned char usart1_idle_flag;//��־

void uart_init(u32 bound1);//���ڳ�ʼ��
void UsartOutStrIT(USART_TypeDef* USARTx, unsigned char *c,uint32_t cnt);
void UsartOutStr(USART_TypeDef* USARTx, unsigned char *c,uint32_t cnt);//���ڷ����ַ�������
void UsartOutChar(USART_TypeDef* USARTx, unsigned char c);//���ڷ���һ���ֽ�
void Usart2IdleLoop(int IdleCnt);
#endif


