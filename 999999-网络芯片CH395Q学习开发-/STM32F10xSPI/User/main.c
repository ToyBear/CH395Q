/**
  ******************************************************************************
  * @author  yang feng wu 
  * @version V1.0.0
  * @date    2019/10/12
  * @brief   
  ******************************************************************************
	
  ******************************************************************************/
	
#include "main.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "mcu_api.h"
#include "usart.h"
#include "delay.h"
#include "timer.h"
#include "iwdg.h"


#include "CH395SPI.H"
#include "CH395CMD.C"


int main(void)
{
  NVIC_Configuration();
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
	delay_init();
	//��ʼ��CH395ʹ�õ�GPIO
	CH395_PORT_INIT();
	//��λ CH395
	CH395_RST();
	
	IWDG_Init(IWDG_Prescaler_256,156*10);
	printf("\r\nstart\r\n");
	
	//��ӡоƬ�汾
	printf("CH395VER : %2x\n",CH395CMDGetVer());
	//���������λȡ������˵������ͨ��
	while(CH395CMDCheckExist(0x55) == 0xaa)
	{
		printf("\r\nCH395CMDCheck OK\r\n");
		break;
	}
	//��ʼ��ģ��:�ɹ����� 0 
	while(!CH395CMDInitCH395())
	{
		printf("\r\nCH395CMDInitCH395 OK\r\n");
		break;
	}
	
	printf("\r\nstart\r\n");
	while(1)
	{
		IWDG_Feed();//ι��

		/* ��ѯCH395�Ƿ����� */
		if(CH395CMDGetPHYStatus() == PHY_DISCONN)
		{
			
			printf("CH395 DisConnect Ethernet\n");
		}
		else /* CH395оƬ���ӵ���̫������ʱ������ж� */
		{   
			printf("CH395 Connect Ethernet\n");
		}
		
		delay_ms(300);
	}
}









