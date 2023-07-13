#ifndef __BEEP_H
#define	__BEEP_H


#include "stm32f10x.h"


/* 定义BEEP连接的GPIO端口 */
#define BEEP_PORT    	GPIOB		              /* GPIO端口 */
#define BEEP_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define BEEP_PIN			GPIO_Pin_1			        

/* 使用标准的固件库控制IO*/
#define open_beep(n)	if (n)	\
					GPIO_SetBits(BEEP_PORT,BEEP_PIN);\
					else		\
					GPIO_ResetBits(BEEP_PORT,BEEP_PIN)


void Beep_Gpio_Init(void);

#endif /* __BEEP_H */
