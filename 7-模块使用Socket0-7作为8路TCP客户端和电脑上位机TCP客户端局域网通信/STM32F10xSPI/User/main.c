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
#include "CH395INC.H"
#include "CH395CMD.H"
/*��ʾ:(ֻ����ʾ!���ʹ�õ�Socket0ͨ��,��û����Ϊ���仺����)
оƬ����48�黺����,ÿ��������512�ֽ�
оƬ����8��Socket,Ĭ�ϰ�48�黺�����ָ���Socket0,Socket1,Socket2,Socket3
���ĸ�Socket,ÿ�� Socket ʹ��8�黺������Ϊ����,4�黺������Ϊ����,
��Socket0,Socket1,Socket2,Socket3�Ľ�������Ϊ512*8 = 4KB
��Socket0,Socket1,Socket2,Socket3�ķ�������Ϊ512*4 = 2KB
���Ҫʹ��Socket4,Socket5,Socket6,Socket7��Ҫ���·��仺����
*/


/*�洢������յ�����*/
#define recv_buff_len 1500
unsigned char recv_buff[recv_buff_len];

char ch395_version=0;//��ȡ�汾��

unsigned char buf[20];
int ch395_status=0;//��ȡ�ж��¼�

/* socket ��ض���*/
UINT8  SocketDesIP[4] = {192,168,0,103}; /* Socket Ŀ��IP��ַ */
UINT16 SocketDesPort   = 8888;           /* Socket Ŀ�Ķ˿� */

UINT8	SocketStatus[4]={0,0,0,0};/*Socket0-3״̬ 0:δ���ӷ�����;1:�����Ϸ����� */


/* Socket ���ض˿�,��ʼ��Ĭ�϶˿ں� */
UINT16 SocketLocalPort = 1000;           
/*��̬��ȡ���ض˿ں�,ÿ�λ�ȡ�˿ں��ۼ�*/
UINT16 ch395_socket_tcp_client_port(void)
{
	SocketLocalPort++;
	if(SocketLocalPort>65535) SocketLocalPort = 1000;
	return SocketLocalPort;
}

/**
* @brief   ��ʼ��socket
* @param   sockindex  Socket����(0,1,2,3,4,5,6,7)
* @param   ipaddr  Ŀ�ĵ�ַ
* @param   desprot Ŀ�Ķ˿ں�
* @param   surprot ���ض˿ں�
* @retval  0:��ʼ���ɹ�; others:��ʼ��ʧ��
* @warning None
* @example 
**/
char ch395_socket_tcp_client_init(UINT8 sockindex,UINT8 *ipaddr,UINT16 desprot,UINT16 surprot)
{
    CH395SetSocketDesIP(sockindex,ipaddr);         		/* Ŀ�ĵ�ַ */         
    CH395SetSocketProtType(sockindex,PROTO_TYPE_TCP); /* Э������ */
    CH395SetSocketDesPort(sockindex,desprot);     		/* Ŀ�Ķ˿ں� */
    CH395SetSocketSourPort(sockindex,surprot);  			/* ���ض˿ں� */
		if(CH395OpenSocket(sockindex) !=0)                /* ��Socket */
		{
			return 1;
		}
		return 0;
}


/**
* @brief   socket������(�Ѵ˺����ŵ�ȫ��socket�ж�����)
* @param   sockindex  Socket����(0,1,2,3,4,5,6,7)
* @param   None
* @param   None
* @param   None
* @retval  None
* @warning None
* @example 
**/
void ch395_socket_tcp_client_interrupt(UINT8 sockindex)
{
	UINT8  sock_int_socket;
	UINT16 len;
	
	/* ��ȡsocket ���ж�״̬ */
	sock_int_socket = CH395GetSocketInt(sockindex);
	
	/* ���ͻ��������У����Լ���д��Ҫ���͵����� */
	if(sock_int_socket & SINT_STAT_SENBUF_FREE)
	{
	}
	
	/* ��������ж� */
	if(sock_int_socket & SINT_STAT_SEND_OK)                           
	{
	}
	
	/* ���������ж� */
	if(sock_int_socket & SINT_STAT_RECV)                              
	{
		len = CH395GetRecvLength(sockindex);/* ��ȡ��ǰ�����������ݳ��� */
		printf("\r\nsocket%d receive len = %d\r\n",sockindex,len);
		if(len == 0)return;
		if(len > recv_buff_len)len = recv_buff_len;
		CH395GetRecvData(sockindex,len,recv_buff);/* ��ȡ���� */
		
		/*�ѽ��յ����ݷ��͸�������*/
		CH395SendData(sockindex,recv_buff,len);
		
		/*ʹ�ô��ڴ�ӡ���յ�����*/
		PutData(&rb_t_usart1_send,recv_buff,len);
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	}
	
	/* �����жϣ�����TCPģʽ����Ч*/
	if(sock_int_socket & SINT_STAT_CONNECT)                          
	{
		SocketStatus[sockindex] = 1;//��������״̬Ϊ����
		printf("socket%d SINT_STAT_CONNECT\r\n",sockindex);
	}
	
	/* �Ͽ��жϣ�����TCPģʽ����Ч */
	if(sock_int_socket & SINT_STAT_DISCONNECT)   
	{
		printf("socket%d SINT_STAT_DISCONNECT \r\n",sockindex);
		SocketStatus[sockindex] = 0;//��������״̬Ϊδ����
	}
 
	/* ��ʱ�жϣ�����TCPģʽ����Ч ,TCP CLIENT�޷�˳�����ӷ������˻������ж�*/
	if(sock_int_socket & SINT_STAT_TIM_OUT)      
	{/*��ʱ���԰�SocketԴ�˿ںŽ����ԼӴ������µĶ˿�ȥ���ӷ�����*/ 
		printf("socket%d SINT_STAT_TIM_OUT\n",sockindex);
		SocketStatus[sockindex] = 0;//��������״̬Ϊδ����
	}
}




int main(void)
{
  NVIC_Configuration();
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
	delay_init();
	timer2_config();
	
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
	
	/*���������λȡ������˵������ͨ��*/
	while(CH395CMDCheckExist(0x55) != 0xaa)
	{
		printf("\r\nCH395CMDCheck ERR\r\n");
		delay_ms(100);
	}
	
	/*��ʼ��ģ��:�ɹ����� 0 */
	while(CH395CMDInitCH395() != 0)
	{
		printf("\r\nCH395CMDInitCH395 ERR\r\n");
		delay_ms(100);
	}
	
	printf("\r\nstart\r\n");
	while(1)
	{
		IWDG_Feed();//ι��
		
		/*ÿ��8S��ʼ��Socket��ִ�����Ӻ���*/
		/*оƬ�ڲ����ӳ�ʱʱ��Ϊ5S,������ڴ�ʱ��*/
		if(Timer2Cnt>8000)
		{
			Timer2Cnt = 0;
			/*Socket0*/
			if(!SocketStatus[0])
			{
				if(ch395_socket_tcp_client_init(0,SocketDesIP,SocketDesPort,ch395_socket_tcp_client_port()) == 0)
				{
					printf("CH395TCPConnect0 ... \r\n");
					CH395TCPConnect(0);//���ӷ�����
				}
			}
			/*Socket1*/
			if(!SocketStatus[1])
			{
				if(ch395_socket_tcp_client_init(1,SocketDesIP,SocketDesPort,ch395_socket_tcp_client_port()) == 0)
				{
					printf("CH395TCPConnect1 ... \r\n");
					CH395TCPConnect(1);//���ӷ�����
				}
			}
			/*Socket2*/
			if(!SocketStatus[2])
			{
				if(ch395_socket_tcp_client_init(2,SocketDesIP,SocketDesPort,ch395_socket_tcp_client_port()) == 0)
				{
					printf("CH395TCPConnect2 ... \r\n");
					CH395TCPConnect(2);//���ӷ�����
				}
			}
			/*Socket3*/
			if(!SocketStatus[3])
			{
				if(ch395_socket_tcp_client_init(3,SocketDesIP,SocketDesPort,ch395_socket_tcp_client_port()) == 0)
				{
					printf("CH395TCPConnect3 ... \r\n");
					CH395TCPConnect(3);//���ӷ�����
				}
			}
		}
		
		
		//INT���Ų����͵�ƽ�ж��Ժ��ȥ�ж�
		if(Query395Interrupt())
		{
			/*��ȡ�ж��¼�*/
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
				if(CH395GetDHCPStatus() == 0)//DHCP OK
				{
				}
			}

			/* �����ɴ��жϣ���ȡ���ɴ���Ϣ */
			if(ch395_status & GINT_STAT_UNREACH){
				CH395CMDGetUnreachIPPT(buf);
			}
			
			/* ����IP��ͻ�жϣ����������޸�CH395�� IP������ʼ��CH395*/
			if(ch395_status & GINT_STAT_IP_CONFLI){

			}
			/* ���� SOCK0 �ж� */
			if(ch395_status & GINT_STAT_SOCK0){
				ch395_socket_tcp_client_interrupt(0);
			}
			/* ���� SOCK1 �ж� */
			if(ch395_status & GINT_STAT_SOCK1){
				ch395_socket_tcp_client_interrupt(1);
			}
			/* ���� SOCK2 �ж� */
			if(ch395_status & GINT_STAT_SOCK2){
				ch395_socket_tcp_client_interrupt(2);
			}
			/* ���� SOCK3 �ж� */
			if(ch395_status & GINT_STAT_SOCK3){
				ch395_socket_tcp_client_interrupt(3);
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






