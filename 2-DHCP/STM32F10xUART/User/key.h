#ifndef __KEY_H_
#define __KEY_H_

#ifndef __KEY_C_
#define __KEY_C_  extern
#else 
#define __KEY_C_
#endif
#include "stm32f10x.h"
#include "System.h"

#define Key1Down 0  //0- �͵�ƽ��Ϊ�ǰ���  1-�ߵ�ƽ��Ϊ�ǰ���
#define Key1PinIn PBin(5)  //���İ�������
__KEY_C_ int Key1Value[7]; //�������� ���±�� �ɿ���� ���±�ǿ���  �ɿ���ǿ��� ����ʱ��(Ms) �ɿ�ʱ��(Ms)
void Key1Function(void);
	
#endif
