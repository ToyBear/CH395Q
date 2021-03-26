#ifndef __GPIO_H_
#define __GPIO_H_
#include "stm32f10x.h"
#include "System.h"



#define realy1_pin      GPIO_Pin_11		/*����ܽ�*/
#define realy1_port     GPIOC					/*GPIO�˿�*/
#define realy1_set      PCout(11)=1
#define realy1_reset    PCout(11)=0
#define realy1_in       PCin(11)

#define realy2_pin      GPIO_Pin_12		/*����ܽ�*/
#define realy2_port     GPIOC					/*GPIO�˿�*/
#define realy2_set      PCout(12)=1
#define realy2_reset    PCout(12)=0
#define realy2_in       PCin(12)


void GpioInit(void);

#endif
