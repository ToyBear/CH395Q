/**
  ******************************************************************************
  * @author  yang feng wu 
  * @version V1.0.0
  * @date    2019/10/12
  * @brief   
  ******************************************************************************
	
  ******************************************************************************
  */
/* ��Ƭ��������               CH395оƬ������
UART2_TXD----PA2                RXD
UART2_RXD----PA3         	      TXD 
             PB2                RST
						 PB5                SEL
INT#---------PA1                INT#
*/
	
#include "main.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "System.h"
#include "usart.h"
#include "gpio.h"
#include "delay.h"
#include "timer.h"
#include "iwdg.h"
#include "cString.h"
#include "key.h"


#include "CH395inc.h"
#include "CH395SPI.h"
#include "CH395CMD.C"

//��������ʹ��
unsigned char MainBuffer[MainBufferLen];//��������,ȫ��ͨ��
u32  MainLen=0;      //ȫ��ͨ�ñ���
unsigned char *MainString;    //ȫ��ͨ�ñ���


int main(void)
{
  NVIC_Configuration();
	uart_init(115200,9600,9600);	 //���ڳ�ʼ��Ϊ115200
	GpioInit();
	CH395_PORT_INIT();
	DelayInit();
	Timer2_Config();
	IWDG_Init(IWDG_Prescaler_256,156*10);
	PBout(2)=0;
	delay_ms(1000);
	PBout(2)=1;
	
	printf("\r\nstart\r\n");
	while(1)
	{
		IWDG_Feed();//ι��
		
		if(Timer2Cnt>1000){
			Timer2Cnt=0;
			
			printf("CH395VER : %2x\n",CH395CMDGetVer());
		}
		
		
		
		//��ȡ����1���յ�����
		BufferManageRead(&buff_manage_struct_usart1_recv,Usart1RecvBufferMemoryCopy,&buff_manage_struct_usart1_recv.ReadLen);
		if(buff_manage_struct_usart1_recv.ReadLen>0){//������
			
		}
		//��ȡ����2���յ�����
		BufferManageRead(&buff_manage_struct_usart2_recv,Usart2RecvBufferMemoryCopy,&buff_manage_struct_usart2_recv.ReadLen);
		if(buff_manage_struct_usart2_recv.ReadLen>0){//������
			
			printf("%x\r\n",Usart2RecvBufferMemoryCopy[0]);
			
		}
		
		//��ȡ����3���յ�����
		BufferManageRead(&buff_manage_struct_usart3_recv,Usart3RecvBufferMemoryCopy,&buff_manage_struct_usart3_recv.ReadLen);
		if(buff_manage_struct_usart3_recv.ReadLen>0){//������
			
		}

		if(Usart3SendDataCnt<=0)
		{
			BufferManageRead(&buff_manage_struct_usart3_send,Usart3SendBufferMemoryCopy,&buff_manage_struct_usart3_send.ReadLen);
			if(buff_manage_struct_usart3_send.ReadLen>0)
			{
				UsartOutStrIT(USART3,Usart3SendBufferMemoryCopy,buff_manage_struct_usart3_send.ReadLen);
			}
		}
	}
}
