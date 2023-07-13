/**
  ******************************************************************************
  * @file    beep.c
  * @author  mafeng
  * @version V1.0
  * @date    2023-xx-xx
  * @brief   beep应用函数接口
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */
  
#include "beep.h"   

 /**
  * @brief  初始化控制beep的IO
  * @param  无
  * @retval 无
  */
void Beep_Gpio_Init(void)
{		
		/*定义一个GPIO_InitTypeDef类型的结构体*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*开启蜂鸣器相关的GPIO外设时钟*/
		RCC_APB2PeriphClockCmd( BEEP_CLK , ENABLE);
		/*选择要控制的GPIO引脚*/
		GPIO_InitStructure.GPIO_Pin = BEEP_PIN;	

		/*设置引脚模式为通用推挽输出*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		/*设置引脚速率为50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*调用库函数，初始化GPIO*/
		GPIO_Init(BEEP_PORT, &GPIO_InitStructure);	
		//关闭蜂鸣器
    
}

/*********************************************END OF FILE**********************/
