#include "System.h"
#include <stdlib.h>
#include <stdio.h>

void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	
	/*����--1*/
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
		/*����--2*/
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
		/*����--3*/
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
		//��ʱ��2
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_Init(&NVIC_InitStructure);
}
/*��λоƬ*/
void Reset_MCU(void)
{
  __disable_fault_irq();   
	NVIC_SystemReset();
}


uint32_t idAddr[]={
	0x1FFFF7AC,/*STM32F0ΨһID��ʼ��ַ*/
	0x1FFFF7E8,/*STM32F1ΨһID��ʼ��ַ*/
	0x1FFF7A10,/*STM32F2ΨһID��ʼ��ַ*/
	0x1FFFF7AC,/*STM32F3ΨһID��ʼ��ַ*/
	0x1FFF7A10,/*STM32F4ΨһID��ʼ��ַ*/
	0x1FF0F420,/*STM32F7ΨһID��ʼ��ַ*/
	0x1FF80050,/*STM32L0ΨһID��ʼ��ַ*/
	0x1FF80050,/*STM32L1ΨһID��ʼ��ַ*/
	0x1FFF7590,/*STM32L4ΨһID��ʼ��ַ*/
	0x1FF0F420 /*STM32H7ΨһID��ʼ��ַ*/
}; 
 
/*��ȡMCUΨһID*/
void GetSTM32MCUID(uint32_t *MCUID,char AddrID)
{
	if(MCUID!=NULL)
	{
		MCUID[0]=*(uint32_t*)(idAddr[AddrID]);
		MCUID[1]=*(uint32_t*)(idAddr[AddrID]+4);
		MCUID[2]=*(uint32_t*)(idAddr[AddrID]+8);
		
		printf("\r\n оƬΨһID: %X-%X-%X\r\n",MCUID[0],MCUID[1],MCUID[2]);   
	}
}

