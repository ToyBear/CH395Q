/********************************** (C) COPYRIGHT *******************************
* File Name          : SPI_HW.C
* Author             : WXF
* Version            : V1.0
* Date               : 2013/12/19
* Description        : CH395оƬ CH395оƬ Ӳ����׼SPI�������ӵ�Ӳ������� V1.0
*                      �ṩI/O�ӿ��ӳ���
*******************************************************************************/



/*******************************************************************************/

/*******************************************************************************/
/* Ӳ����غ궨�� */
/* �����е�Ӳ�����ӷ�ʽ����(ʵ��Ӧ�õ�·���Բ����޸��������弰�ӳ���) */
/* ��Ƭ��������               CH395оƬ������
    SPI1_NSS-----PB0            	SCS
    SPI1_SCK-----PA5            	SCK      
    SPI1_MISO----PA6            	SDO
    SPI1_MOSI----PA7            	SDI
    INT#---------PA1                INT#
*/
#define CH395_SPI_SCS_LOW( )       ( GPIOB->BRR  = GPIO_Pin_0 )   /* SPIƬѡ��������͵�ƽ */
#define CH395_SPI_SCS_HIGH( )      ( GPIOB->BSRR = GPIO_Pin_0 )   /* SPIƬѡ��������ߵ�ƽ */
#define	CH395_SPI_SDO_PIN( )	   GPIO_ReadInputDataBit( GPIOA, GPIO_Pin_6 ) /* ��ȡCH395��SPI����������ŵ�ƽ */

#define	CH395_INT_PIN_WIRE( )	   GPIOA->IDR & GPIO_Pin_1 		  /* �ٶ�CH395��INT#����,���δ������ôҲ����ͨ����ѯ�����ж������SDO����״̬ʵ��                                                                                                                                                                                                                                */


/*******************************************************************************
* Function Name  : Delay_uS
* Description    : ΢�뼶��ʱ����(����׼ȷ)
* Input          : delay---��ʱֵ
* Output         : None
* Return         : None
*******************************************************************************/
void mDelayuS( UINT8 delay )
{
	UINT8 i, j;

	for( i = delay; i != 0; i -- ) 
	{
		for( j = 5; j != 0; j -- )
		{
		}		
	}
}

/*******************************************************************************
* Function Name  : Delay_mS
* Description    : ���뼶��ʱ����(����׼ȷ)
* Input          : delay---��ʱֵ
* Output         : None
* Return         : None
*******************************************************************************/
void mDelaymS( UINT8 delay )
{
	UINT8 i;
  UINT16 j;
	for( i = delay; i != 0; i -- ) 
	{
		for( j = 200; j != 0; j -- )
		{
			mDelayuS( 5 );
		}		
	}
}				
				   
/*******************************************************************************
* Function Name  : CH395_Port_Init
* Description    : CH395�˿ڳ�ʼ��
*                  ����ʹ��SPI��дʱ��,���Խ��г�ʼ��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CH395_PORT_INIT( void )  
{
	SPI_InitTypeDef  SPI_InitStructure;
 	GPIO_InitTypeDef GPIO_InitStructure;

	/* ��ʼ��SPI�ӿ� */   
	/* Enable SPI1 and GPIOA clocks */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_SPI1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE );/* ʵ������֮ǰ�Ѿ�ʹ�ܹ� */
  
	/* Configure SPI1 pins: NSS, SCK, MISO and MOSI */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF_PP;	  		 /* ����������ù��� */
	GPIO_Init( GPIOA, &GPIO_InitStructure );

  	/* Configure PA.4 as Output push-pull, used as Flash Chip select */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  			 /* ������� */
  	GPIO_Init( GPIOB, &GPIO_InitStructure );

  	/* Deselect the SD: Chip Select high */
  	CH395_SPI_SCS_HIGH( );
 
  	/* SPI1 configuration */ 
  	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; /* SPI���ó����ߵĵ���ȫ˫��ͨ�� */
  	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;	 				   /* SPI���� */
  	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;				   /* SPI8λ���ݸ�ʽ���� */
  	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;	  					   /* ʱ�ӵ�ʱ� */  				//	�ĳɸ�����?????????????????
  	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;					   /* ������ʱ�ӵڶ�������ʱ���� */
  	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		 				   /* �ڲ�NSS�ź���SSI���� */
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4; /* ������Ԥ��Ƶ��Ϊ4 */
  	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;		   		   /* ����ʱ��λ��ǰ */
  	SPI_InitStructure.SPI_CRCPolynomial = 7;			  			  	    
  	SPI_Init( SPI1, &SPI_InitStructure );
  
  	/* Enable SPI1  */
  	SPI_Cmd( SPI1, ENABLE );   							
	
	/* ��ʼ���ж����� */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  				 /* �������� */
    GPIO_Init( GPIOA, &GPIO_InitStructure );				
}

/*******************************************************************************
* Function Name  : Spi395Exchange
* Description    : Ӳ��SPI���������8��λ����
* Input          : d---��Ҫ���뵽CH395������
* Output         : None
* Return         : None
*******************************************************************************/
UINT8 Spi395Exchange( UINT8 d )  
{  
	/* Loop while DR register in not emplty */
//	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);	
	while( ( SPI1->SR & SPI_I2S_FLAG_TXE ) == RESET );

	/* Send byte through the SPI1 peripheral */
//	SPI_I2S_SendData(SPI1, byte);
	SPI1->DR = d;

	/* Wait to receive a byte */
//	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	while( ( SPI1->SR & SPI_I2S_FLAG_RXNE ) == RESET );

	/* Return the byte read from the SPI bus */
//	return  SPI_I2S_ReceiveData(SPI1);
	return( SPI1->DR );	
} 

/*******************************************************************************
* Function Name  : xWriteCH395Cmd
* Description    : ��CH395д����
* Input          : mCmd---��Ҫд��CH395��������
* Output         : None
* Return         : None
*******************************************************************************/
void xWriteCH395Cmd( UINT8 mCmd )  
{
	CH395_SPI_SCS_HIGH( );  									 /* ��ֹ֮ǰδͨ��xEndCH395Cmd��ֹSPIƬѡ */
	CH395_SPI_SCS_HIGH( ); 
			
	/* ����˫��I/O����ģ��SPI�ӿ�,��ô����ȷ���Ѿ�����SPI_SCS,SPI_SCK,SPI_SDIΪ�������,SPI_SDOΪ���뷽�� */
	CH395_SPI_SCS_LOW( );  										 /* SPIƬѡ��Ч */

	/* ���������� */
	Spi395Exchange( mCmd );  									 /* ���������� */
	mDelayuS( 2 );  											 /* ��ʱ1.5uSȷ����д���ڴ���1.5uS */
}

/*******************************************************************************
* Function Name  : xWriteCH395Data
* Description    : ��CH395д����
* Input          : mData---��Ҫд��CH395������
* Output         : None
* Return         : None
*******************************************************************************/
void xWriteCH395Data( UINT8 mData ) 
{
	Spi395Exchange( mData );  									 /* �������� */
}

/*******************************************************************************
* Function Name  : xReadCH395Data
* Description    : ��CH395������
* Input          : None
* Output         : None
* Return         : ���ض�ȡ������
*******************************************************************************/
UINT8 xReadCH395Data( void ) 
{
	return( Spi395Exchange( 0xFF ) );  	
}

#define	xEndCH395Cmd( )	{ CH395_SPI_SCS_HIGH( ); }  			 /* SPIƬѡ��Ч,����CH395����,������SPI�ӿڷ�ʽ */

/*******************************************************************************
* Function Name  : Query395Interrupt
* Description    : ��ѯCH395�ж�(INT#�͵�ƽ)
* Input          : None
* Output         : None
* Return         : �����ж�״̬
*******************************************************************************/
UINT8 Query395Interrupt( void )
{
	return( CH395_INT_PIN_WIRE( ) ? FALSE : TRUE );  
}


