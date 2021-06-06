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

int ch395_status=0;//��ȡ�ж��¼�
UINT8  buf[20];//����DHCP����
UINT8 dhcp_enable_flage = 1; //1:û��ʹ��DHCP, 0:ʹ��DHCP
UINT8 dhcp_success_flage = 0;//0:δ��ȡ��DHCP����ĵ�ַ��Ϣ,  1:��ȡ��ַ�ɹ�
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
		/* ��ѯCH395�Ƿ�������·���� */
		if(CH395CMDGetPHYStatus() == PHY_DISCONN)
		{
			printf("CH395 DisConnect Ethernet\r\n");//ģ��û��������·����
			delay_ms(1000);
			//���ñ���
			dhcp_enable_flage = 1;
			dhcp_success_flage = 0;
		}
		else /*�����������Ժ���ȥ����DHCP*/
		{
			if(dhcp_enable_flage !=0)//û������DHCP
			{
				dhcp_enable_flage =CH395DHCPEnable(1);//����DHCP,�ɹ�����0
				if(dhcp_enable_flage !=0){
					printf("CH395DHCPEnable ERR\r\n");
					delay_ms(100);
				}
			}
			else //�Ѿ�����DHCP,��ѵ�¼�
			{
				//INT���Ų����͵�ƽ�ж��Ժ��ȥ�ж�
				if(Query395Interrupt()) //Ҳ�������ε����,������ĳ���һֱ��ѵģ��
				{
					/*��ȡ�ж��¼�*/
					ch395_status = CH395CMDGetGlobIntStatus_ALL();

					/* �����ɴ��жϣ���ȡ���ɴ���Ϣ */
					if(ch395_status & GINT_STAT_UNREACH)                              
					{
						CH395CMDGetUnreachIPPT(buf);                                
					}
					
					/* ����PHY�ı��ж�*/
					if(ch395_status & GINT_STAT_PHY_CHANGE)                           
					{
						printf("Init status : GINT_STAT_PHY_CHANGE\r\n");
						//���ñ���
						dhcp_enable_flage = 1;//����DHCP
						dhcp_success_flage = 0;
					}
					
					/* ����DHCP/PPPOE�ж� */
					if(ch395_status & GINT_STAT_DHCP)
					{
						if(!dhcp_success_flage)//��û�л�ȡ��DHCP����ĵ�ַ��Ϣ
						{
							if(CH395GetDHCPStatus() == 0)//DHCP��OK��
							{
								
								CH395GetIPInf(buf);//��ȡIP��������������ص�ַ
								
								//ȷ����ȡ����
								if(buf[0]!=0 || buf[1]!=0 || buf[2]!=0 || buf[3]!=0)
								{
									dhcp_success_flage = 1;
									
									printf("IP:%d.%d.%d.%d\r\n",buf[0],buf[1],buf[2],buf[3]);
									printf("GWIP:%d.%d.%d.%d\r\n",buf[4],buf[5],buf[6],buf[7]);
									printf("Mask:%d.%d.%d.%d\r\n",buf[8],buf[9],buf[10],buf[11]);
									printf("DNS1:%d.%d.%d.%d\r\n",buf[12],buf[13],buf[14],buf[15]);
									printf("DNS2:%d.%d.%d.%d\r\n",buf[16],buf[17],buf[18],buf[19]);
								}
							}
						}
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
}









