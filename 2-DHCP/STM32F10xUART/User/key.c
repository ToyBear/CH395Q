/**
  ******************************************************************************
  * @file    key.c
  * @author  fengwu yang
  * @version V1.0.0
  * @date    2019/10/12
  * @brief   �������ģ��
  ******************************************************************************
  һ,ʹ��˵��:�������(PB5)
		1,�����³������1ms��ʱ���ж���
			if(Key1Value[1])
				Key1Value[5]++;
			else
				Key1Value[5]=0;

			if(Key1Value[2])
				Key1Value[6]++;
			else
				Key1Value[6]=0;
		
		2,��ѭ������ʹ��
			while(1)
			{
				Key1Function();
				
				//��1:��⵽��������ִ��ĳ�δ���
				if(Key1Value[3] == 1)
				{
					Key1Value[3] = 0;//�����Ժ�,ֻ�а����ɿ��ٰ��µ�ʱ��Ż����
					//ִ�еĴ���
				}

				//��2:��⵽��������,Ȼ����ݰ���ʱ��ִ��ĳ�δ���
				if(Key1Value[3] == 1)
				{
					if(Key1Value[5]>=3000)//����ʱ�����3S
					{
						Key1Value[3] = 0;//�����Ժ�,ֻ�а����ɿ��ٰ��µ�ʱ��Ż����
						//ִ�еĴ���
					}
				}
				
				//��3:��⵽�����ɿ�ִ��ĳ�δ���
				if(Key1Value[4] == 1)
				{
					Key1Value[4] = 0;//�����Ժ�,ֻ�а�������Ȼ���ɿ���ʱ��Ż����
					//ִ�еĴ���
				}
				
				//��4:��⵽�����ɿ�,Ȼ������ɿ�ʱ��ִ��ĳ�δ���
				if(Key1Value[4] == 1)
				{
					if(Key1Value[6]>=3000)//�ɿ�ʱ�����3S
					{
						Key1Value[4] = 0;//�����Ժ�,ֻ�а����������ɿ���ʱ��Ż����
						//ִ�еĴ���
					}
				}
			}
  ******************************************************************************
  */

#define __KEY_C_
#include "key.h"

int Key1Value[7]={0};//�������� ���±�� �ɿ���� ���±�ǿ���  �ɿ���ǿ��� ����ʱ��(Ms) �ɿ�ʱ��(Ms)
	
/**
* @brief  ��������
* @param  
* @param  None
* @param  None
* @retval None
* @example 
**/
void Key1Function(void)
{
  if(Key1PinIn == Key1Down)
	{
		if(Key1Value[1] == 0)
		{
			Key1Value[0]++;
			if(Key1Value[0]>=500)//�����Ժ����
			{
				Key1Value[0] = 0;//Cnt
				Key1Value[2] = 0;//UP
				Key1Value[1] = 1;//Down
				Key1Value[3] = 1;//DownCopy
				Key1Value[4] = 0;//UpCopy
			}
		}
	}
	else
	{
		Key1Value[0]++;
		if(Key1Value[0]>2000)
		{
			Key1Value[0]=0;
			if(Key1Value[1]==1)//���º��ɿ������
			{
				Key1Value[2] = 1;//UP
				Key1Value[1] = 0;//Down
				Key1Value[3] = 0;//DownCopy
				Key1Value[4] = 1;//UpCopy
			}
		}
	}
}
