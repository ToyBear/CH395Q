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
#include "CH395CMD.H"

char ch395_version=0;//��ȡ�汾��
int ch395_status=0;//��ȡ�ж��¼�
UINT8  buf[20];//����DHCP����
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
	
	/*��ȡоƬ�汾*/
	while((ch395_version = CH395CMDGetVer()) < 0x40)
	{
		printf("CH395CMDGetVer ERR\r\n");
		delay_ms(100);
	}
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

		//INT���Ų����͵�ƽ�ж��Ժ��ȥ�ж�
		if(Query395Interrupt())
		{
			/*��ȡ�ж��¼�(��ͬ�汾��ȡ�ĺ�����һ��)*/
			if(ch395_version>=0x44)
			{
				ch395_status = CH395CMDGetGlobIntStatus_ALL();
			}				
			else
			{
				ch395_status = CH395CMDGetGlobIntStatus();
			}

			/* ����PHY�ı��ж�*/
			if(ch395_status & GINT_STAT_PHY_CHANGE)                           
			{
				if(CH395CMDGetPHYStatus() == PHY_DISCONN)//���߶Ͽ�
				{
					printf("\r\nPHY_DISCONN\r\n");
				}
				else//��������
				{
					printf("\r\nPHY_CONNECTED\r\n");
					CH395DHCPEnable(1);//����DHCP
				}
			}
			
			
			/* ����DHCP/PPPOE�ж� */
			if(ch395_status & GINT_STAT_DHCP)
			{
				if(CH395GetDHCPStatus() == 0)//DHCP��OK��
				{
					CH395GetIPInf(buf);//��ȡIP��������������ص�ַ

					printf("IP:%d.%d.%d.%d\r\n",buf[0],buf[1],buf[2],buf[3]);
					printf("GWIP:%d.%d.%d.%d\r\n",buf[4],buf[5],buf[6],buf[7]);
					printf("Mask:%d.%d.%d.%d\r\n",buf[8],buf[9],buf[10],buf[11]);
					printf("DNS1:%d.%d.%d.%d\r\n",buf[12],buf[13],buf[14],buf[15]);
					printf("DNS2:%d.%d.%d.%d\r\n",buf[16],buf[17],buf[18],buf[19]);
				}
			}
			
			/* �����ɴ��жϣ���ȡ���ɴ���Ϣ */
			if(ch395_status & GINT_STAT_UNREACH)                              
			{
				CH395CMDGetUnreachIPPT(buf);                                
			}
			
			/* ����IP��ͻ�жϣ����������޸�CH395�� IP������ʼ��CH395*/
			if(ch395_status & GINT_STAT_IP_CONFLI){
				
			}
			/* ���� SOCK0 �ж� */
			if(ch395_status & GINT_STAT_SOCK0){
				
			}
			/* ���� SOCK1 �ж� */
			if(ch395_status & GINT_STAT_SOCK1){
				
			}
			/* ���� SOCK2 �ж� */
			if(ch395_status & GINT_STAT_SOCK2){
				
			}
			/* ���� SOCK3 �ж� */
			if(ch395_status & GINT_STAT_SOCK3){
				
			}
			
			if(ch395_version>=0x44)
			{
				/* ���� SOCK4 �ж� */
				if(ch395_status & GINT_STAT_SOCK4){

				}
				/* ���� SOCK5 �ж� */
				if(ch395_status & GINT_STAT_SOCK5){

				}
				/* ���� SOCK6 �ж� */
				if(ch395_status & GINT_STAT_SOCK6){

				}
				/* ���� SOCK7 �ж� */
				if(ch395_status & GINT_STAT_SOCK7){

				}
			}
		}
	}
}









