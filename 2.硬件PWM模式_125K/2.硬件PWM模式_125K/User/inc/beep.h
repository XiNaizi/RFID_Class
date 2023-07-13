#ifndef __BEEP_H
#define	__BEEP_H


#include "stm32f10x.h"


/* ����BEEP���ӵ�GPIO�˿� */
#define BEEP_PORT    	GPIOB		              /* GPIO�˿� */
#define BEEP_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO�˿�ʱ�� */
#define BEEP_PIN			GPIO_Pin_1			        

/* ʹ�ñ�׼�Ĺ̼������IO*/
#define open_beep(n)	if (n)	\
					GPIO_SetBits(BEEP_PORT,BEEP_PIN);\
					else		\
					GPIO_ResetBits(BEEP_PORT,BEEP_PIN)


void Beep_Gpio_Init(void);

#endif /* __BEEP_H */
