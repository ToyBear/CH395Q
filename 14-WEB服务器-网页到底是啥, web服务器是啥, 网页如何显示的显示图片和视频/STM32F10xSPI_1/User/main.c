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


#include "http_help.h"


/*�洢������յ�����*/
#define recv_buff_len 500
unsigned char recv_buff[recv_buff_len];

char ch395_version=0;//��ȡ�汾��

unsigned char buf[20];
int ch395_status=0;//��ȡ�ж��¼�

/* socket ��ض���*/
UINT16 SocketServerPort   = 80;           /*���ؼ����� Socket �˿� */
char   SocketServerStatus = 0;//SocketServer״̬ 0:δ��������; 1:��������

//������������͵���Ӧͷ��������
char http_response_data[1024];

//������������͵���Ӧ������
char http_html_data[1024]=
"<html>\r\n \
<head>\r\n \
<title>������ʾ��ҳ����</title>\r\n \
</head> \
<body> \
<h1>������ô��?</h1> \
</body> \
</html>";


/**
* @brief   ��ʼ��socket
* @param   sockindex  Socket����(0,1,2,3,4,5,6,7)
* @param   None
* @param   None
* @param   surprot ���ض˿ں�
* @retval  0:��ʼ���ɹ�; others:��ʼ��ʧ��
* @warning None
* @example 
**/
char ch395_socket_tcp_server_init(UINT8 sockindex,UINT16 surprot)
{       
    CH395SetSocketProtType(sockindex,PROTO_TYPE_TCP); /* Э������ */
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
		if(len == 0)return;
		if(len > recv_buff_len)len = recv_buff_len;
			
		CH395GetRecvData(sockindex,len,recv_buff);/* ��ȡ���� */
		
		/*ʹ�ô��ڴ�ӡ���յ�����*/
		printf("socket%d receive data:\r\n",sockindex);
		PutData(&rb_t_usart1_send,recv_buff,len);
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
		
		/*��Ӧͷ��*/
		len = sprintf((char *)http_response_data,"HTTP/1.1 200 OK\r\n\
Content-Type: text/html\r\n\
Content-Length:%d\r\n\r\n",sizeof(http_html_data)
		);			
		//������Ӧͷ��
		CH395SendData(sockindex,(UINT8*)http_response_data,len);
		delay_ms(10);

		/*������Ӧ����*/
		CH395SendData(sockindex,(unsigned char*)http_html_data,sizeof(http_html_data));
	}
	
	/* �����жϣ�����TCPģʽ����Ч*/
	if(sock_int_socket & SINT_STAT_CONNECT)                          
	{
		printf("socket%d SINT_STAT_CONNECT\r\n",sockindex);
		
		CH395CMDGetRemoteIPP(sockindex,buf);//��ȡ���ӵ�TCP�ͻ��˵���Ϣ
		printf("IP address = %d.%d.%d.%d\r\n",(UINT16)buf[0],(UINT16)buf[1],(UINT16)buf[2],(UINT16)buf[3]);    
		printf("Port = %d\r\n",((buf[5]<<8) + buf[4]));
	}
	
	/* �Ͽ��жϣ�����TCPģʽ����Ч */
	if(sock_int_socket & SINT_STAT_DISCONNECT)   
	{
		printf("socket%d SINT_STAT_DISCONNECT \r\n",sockindex);
		SocketServerStatus = 0;//������״̬����Ϊδ��������
	}
 
	/* ��ʱ�жϣ�����TCPģʽ����Ч ,TCP CLIENT�޷�˳�����ӷ������˻������ж�*/
	if(sock_int_socket & SINT_STAT_TIM_OUT)      
	{/*��ʱ���԰�SocketԴ�˿ںŽ����ԼӴ������µĶ˿�ȥ���ӷ�����*/ 
		printf("socket%d SINT_STAT_TIM_OUT\r\n",sockindex);
		SocketServerStatus = 0;//������״̬����Ϊδ��������
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
	printf("CH395CMDGetVer =%2x\r\n",ch395_version);
	
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
		
		
		/*��⵽û������������,��ִ������������*/
		if(SocketServerStatus == 0)
		{
			if(ch395_socket_tcp_server_init(0,SocketServerPort) == 0)
			{
				if(CH395TCPListen(0) == 0) //Socke 0 ����TCP����
				{
					printf("\r\nCH395TCPListen\r\n");
					SocketServerStatus = 1;//������״̬����Ϊ��������
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
					CH395GetIPInf(buf);//��ȡIP��������������ص�ַ
					printf("IP:%d.%d.%d.%d\r\n",buf[0],buf[1],buf[2],buf[3]);
					printf("GWIP:%d.%d.%d.%d\r\n",buf[4],buf[5],buf[6],buf[7]);
					printf("Mask:%d.%d.%d.%d\r\n",buf[8],buf[9],buf[10],buf[11]);
					printf("DNS1:%d.%d.%d.%d\r\n",buf[12],buf[13],buf[14],buf[15]);
					printf("DNS2:%d.%d.%d.%d\r\n",buf[16],buf[17],buf[18],buf[19]);
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






