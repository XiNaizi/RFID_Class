/**
  ******************************************************************************
  * @file    beep.c
  * @author  mafeng
  * @version V1.0
  * @date    2023-xx-xx
  * @brief   beepӦ�ú����ӿ�
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */
  
#include "beep.h"   

 /**
  * @brief  ��ʼ������beep��IO
  * @param  ��
  * @retval ��
  */
void Beep_Gpio_Init(void)
{		
		/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*������������ص�GPIO����ʱ��*/
		RCC_APB2PeriphClockCmd( BEEP_CLK , ENABLE);
		/*ѡ��Ҫ���Ƶ�GPIO����*/
		GPIO_InitStructure.GPIO_Pin = BEEP_PIN;	

		/*��������ģʽΪͨ���������*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		/*������������Ϊ50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*���ÿ⺯������ʼ��GPIO*/
		GPIO_Init(BEEP_PORT, &GPIO_InitStructure);	
		//�رշ�����
    
}

/*********************************************END OF FILE**********************/
