/**
  ******************************************************************************
  * @file    usart.c
  * @author  fengwu yang
  * @version V1.0.0
  * @date    2019/10/12
  * @brief   �������ú���
  ******************************************************************************
  ******************************************************************************
  */
#define USART_C_
#include "usart.h"
#include <stdio.h>
#include <stdlib.h>

#include "BufferManage.h"

/****************************����1****************************/
unsigned char Usart1RecvBuff[Usart1RecvBuffLen]={0};  //�������ݻ���
u32  Usart1RecvCnt = 0;//���ڽ��յ������ݸ���

u8   Usart1RecvBufferMemory[Usart1RecvBufferMemoryLen];//�������ݻ�����
u8   Usart1RecvBufferMemoryCopy[Usart1RecvBufferMemoryCopyLen];//��ȡ������������
u32  Usart1RecvBufferMemoryManage[Usart1RecvBufferMemoryManageLen];//��������

unsigned char Usart1SendBuff[Usart1SendBufflen];  //���ڷ������ݻ���
u8 Usart1SendBits=0;//������ȡ���ζ���1���ֽ�

unsigned char *Usart1SendData;//�����жϷ������ݵĵ�ַ
u32   Usart1SendDataCnt=0;//�����жϷ������ݵĸ���


/****************************����2****************************/
unsigned char Usart2RecvBuff[Usart2RecvBuffLen]={0};  //�������ݻ���
u32  Usart2RecvCnt = 0;//���ڽ��յ������ݸ���

u8   Usart2RecvBufferMemory[Usart2RecvBufferMemoryLen];//�������ݻ�����
u8   Usart2RecvBufferMemoryCopy[Usart2RecvBufferMemoryCopyLen];//��ȡ������������
u32  Usart2RecvBufferMemoryManage[Usart2RecvBufferMemoryManageLen];//��������

unsigned char *Usart2SendData;//�����жϷ������ݵĵ�ַ
u32   Usart2SendDataCnt=0;//�����жϷ������ݵĸ���



/****************************����3****************************/
unsigned char Usart3RecvBuff[Usart3RecvBuffLen]={0};  //�������ݻ���
u32  Usart3RecvCnt = 0;//���ڽ��յ������ݸ���

u8   Usart3RecvBufferMemory[Usart3RecvBufferMemoryLen];//�������ݻ�����
u8   Usart3RecvBufferMemoryCopy[Usart3RecvBufferMemoryCopyLen];//��ȡ������������
u32  Usart3RecvBufferMemoryManage[Usart3RecvBufferMemoryManageLen];//��������

unsigned char *Usart3SendData;//�����жϷ������ݵĵ�ַ
u32   Usart3SendDataCnt=0;//�����жϷ������ݵĸ���

u8   Usart3SendBufferMemory[Usart3SendBufferMemoryLen];//�������ݻ�����
u8   Usart3SendBufferMemoryCopy[Usart3SendBufferMemoryCopyLen];//��ȡ������������
u32  Usart3SendBufferMemoryManage[Usart3SendBufferMemoryManageLen];//��������

/**
* @brief   ���ڳ�ʼ��
* @param   bound  ������
* @param   None
* @param   None
* @retval  None
* @warning None
* @example 
**/
void uart_init(u32 bound1,u32 bound2,u32 bound3){
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	rbCreate(&rb_t_usart1_send,Usart1SendBuff,Usart1SendBufflen);//�������ζ���(���ڷ�������)
	BufferManageCreate(&buff_manage_struct_usart1_recv,Usart1RecvBufferMemory,Usart1RecvBufferMemoryLen,Usart1RecvBufferMemoryManage,Usart1RecvBufferMemoryManageLen*4);/*���ڽ��ջ���*/
	
	BufferManageCreate(&buff_manage_struct_usart2_recv,Usart2RecvBufferMemory,Usart2RecvBufferMemoryLen,Usart2RecvBufferMemoryManage,Usart2RecvBufferMemoryManageLen*4);/*���ڽ��ջ���*/
	
	BufferManageCreate(&buff_manage_struct_usart3_recv,Usart3RecvBufferMemory,Usart3RecvBufferMemoryLen,Usart3RecvBufferMemoryManage,Usart3RecvBufferMemoryManageLen*4);/*���ڽ��ջ���*/
	BufferManageCreate(&buff_manage_struct_usart3_send,Usart3SendBufferMemory,Usart3SendBufferMemoryLen,Usart3SendBufferMemoryManage,Usart3SendBufferMemoryManageLen*4);/*���ڷ��ͻ���*/
	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2|RCC_APB1Periph_USART3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_USART1|RCC_APB2Periph_AFIO , ENABLE);	//??USART1,GPIOA??
	
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9
  
  //USART1_RX	  GPIOA.10��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10 
	
	
	
	//USART2_TX   GPIOA.2
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  //USART2_RX	  GPIOA.3
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA3
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	 //USART3_TX   GPIOB.10
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  //USART3_RX	  GPIOB.11
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB11
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
  
	
  //USART ��ʼ������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	
	
	USART_InitStructure.USART_BaudRate = bound1;//���ڲ�����
  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	
	USART_InitStructure.USART_BaudRate = bound2;//���ڲ�����
	USART_Init(USART2, &USART_InitStructure); //��ʼ������2
	
	USART_InitStructure.USART_BaudRate = bound3;//���ڲ�����
	USART_Init(USART3, &USART_InitStructure); //��ʼ������3
	
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);//�����ж�
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);//�����ж�
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);//�����ж�
  USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���
	USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ���
	USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ���
}


/**
* @brief  �����жϷ�������
* @param  c:���ݵ��׵�ַ  cnt:���͵����ݸ���
* @param  None
* @param  None
* @retval None
* @example 
**/
void UsartOutStrIT(USART_TypeDef* USARTx,unsigned char *c,uint32_t cnt)
{
	if(USARTx == USART1){
		PutData(&rb_t_usart1_send,c,cnt);
		USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);
	}
	else if(USARTx == USART2){
		Usart2SendData=c;
		Usart2SendDataCnt=cnt;
		USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);
	}
	else if(USARTx == USART3){
		Usart3SendData=c;
		Usart3SendDataCnt=cnt;
		USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);
	}
}


/**
* @brief  ���ڷ����ַ�������
* @param  *c:���͵�����ָ��  cnt:���ݸ���
* @param  None
* @param  None
* @retval None
* @example 
**/
void UsartOutStr(USART_TypeDef* USARTx,unsigned char *c,uint32_t cnt)
{
	while(cnt--)
	{
		USART_SendData(USARTx, *c++);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
	}
}


/**
* @brief  ���ڷ���һ���ֽ�
* @param  *c:���͵�����ָ��
* @param  None
* @param  None
* @retval None
* @example 
**/
void UsartOutChar(USART_TypeDef* USARTx,unsigned char c)
{
	USART_SendData(USARTx,c);
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
}



void USART1_IRQHandler(void)//����1�жϷ������
{
	u8 Res;
	int value;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		Res =USART_ReceiveData(USART1);	//��ȡ���յ�������
		
		Usart1RecvBuff[Usart1RecvCnt] = Res;	//���յ����ݴ�������
		Usart1RecvCnt++;
		if(Usart1RecvCnt > Usart1RecvBuffLen -10)//��ֹ�������
		{
			Usart1RecvCnt = 0;
		}
	} 
	else if(USART_GetITStatus(USART1,USART_IT_IDLE) == SET)//�����ж�
	{
		USART1->DR; //���USART_IT_IDLE��־
		//�����ݴ��뻺��
	  BufferManageWrite(&buff_manage_struct_usart1_recv,Usart1RecvBuff,Usart1RecvCnt,&value);
		Usart1RecvCnt=0;
  }  
	
	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
  {
    if(rbCanRead(&rb_t_usart1_send)>0)//�����������ݸ�������0
    {
			rbRead(&rb_t_usart1_send,&Usart1SendBits,1);
			USART_SendData(USART1, Usart1SendBits);
    }
    else
    {
      //�����ֽڽ���			
      USART_ClearITPendingBit(USART1,USART_IT_TXE);
      USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
      USART_ITConfig(USART1, USART_IT_TC, ENABLE);
    }
  }
	//�������
  if (USART_GetITStatus(USART1, USART_IT_TC) != RESET)
  {
    USART_ClearITPendingBit(USART1,USART_IT_TC);
    USART_ITConfig(USART1, USART_IT_TC, DISABLE);
  }
} 


void USART2_IRQHandler(void)//����2�жϷ������
{
	u8 Res;
	int value;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		Res =USART_ReceiveData(USART2);	//��ȡ���յ�������
		
		Usart2RecvBuff[Usart2RecvCnt] = Res;	//���յ����ݴ�������
		Usart2RecvCnt++;
		if(Usart2RecvCnt > Usart2RecvBuffLen -10)//��ֹ�������
		{
			Usart2RecvCnt = 0;
		}
	} 
	else if(USART_GetITStatus(USART2,USART_IT_IDLE) == SET)//�����ж�
	{
		USART2->DR; //���USART_IT_IDLE��־
		//�����ݴ��뻺��
		/*��������ȡ���������С,Ԥ���������*/
		if(Usart2RecvCnt<= Usart2RecvBufferMemoryCopyLen)
		{
			BufferManageWrite(&buff_manage_struct_usart2_recv,Usart2RecvBuff,Usart2RecvCnt,&value);
		}
		
	  
		Usart2RecvCnt=0;
  }  
	
	if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET)
  {
    if(Usart2SendDataCnt>0)//�����������ݸ�������0
    {
			USART_SendData(USART2, *Usart2SendData);
			Usart2SendData++;
		  Usart2SendDataCnt--;
    }
    else
    {
      //�����ֽڽ���			
      USART_ClearITPendingBit(USART2,USART_IT_TXE);
      USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
      USART_ITConfig(USART2, USART_IT_TC, ENABLE);
    }
  }
	//�������
  if (USART_GetITStatus(USART2, USART_IT_TC) != RESET)
  {
    USART_ClearITPendingBit(USART2,USART_IT_TC);
    USART_ITConfig(USART2, USART_IT_TC, DISABLE);
  }
} 

void USART3_IRQHandler(void)//����3�жϷ������
{
	u8 Res;
	int value;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		Res =USART_ReceiveData(USART3);	//��ȡ���յ�������
		
		Usart3RecvBuff[Usart3RecvCnt] = Res;	//���յ����ݴ�������
		Usart3RecvCnt++;
		if(Usart3RecvCnt > Usart3RecvBuffLen -10)//��ֹ�������
		{
			Usart3RecvCnt = 0;
		}
	} 
	else if(USART_GetITStatus(USART3,USART_IT_IDLE) == SET)//�����ж�
	{
		USART3->DR; //���USART_IT_IDLE��־
		//�����ݴ��뻺��
		/*��������ȡ���������С,Ԥ���������*/
		if(Usart3RecvCnt<= Usart3RecvBufferMemoryCopyLen)
		{	
			BufferManageWrite(&buff_manage_struct_usart3_recv,Usart3RecvBuff,Usart3RecvCnt,&value);
		}
		Usart3RecvCnt=0;
  }  
	
	if(USART_GetITStatus(USART3, USART_IT_TXE) != RESET)
  {
    if(Usart3SendDataCnt>0)//�����������ݸ�������0
    {
			USART_SendData(USART3, *Usart3SendData);
			Usart3SendData++;
		  Usart3SendDataCnt--;
    }
    else
    {
      //�����ֽڽ���			
      USART_ClearITPendingBit(USART3,USART_IT_TXE);
      USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
      USART_ITConfig(USART3, USART_IT_TC, ENABLE);
    }
  }
	//�������
  if (USART_GetITStatus(USART3, USART_IT_TC) != RESET)
  {
    USART_ClearITPendingBit(USART3,USART_IT_TC);
    USART_ITConfig(USART3, USART_IT_TC, DISABLE);
  }
} 




/**
* @brief  ʹ��microLib�ķ���,ʹ��printf
* @warningg ��ѡ microLib
* @param  None
* @param  None
* @param  None
* @retval None
* @example 
**/

int fputc(int ch, FILE *f)
{
	static char flage=0;
	uint8_t data = ch;
	if(!flage)
	{
	  flage=1;
		USART_ClearITPendingBit(USART1,USART_IT_TC);//���printf��ʧ��һ���ֽڵ�����
	}
	
	PutData(&rb_t_usart1_send,&data,1);
	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);

  return ch;
}

