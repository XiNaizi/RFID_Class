

#include "stm32f10x.h"
#include "systick.h"
#include "beep.h"
#include "usart.h"
#include "rfid1356M.h"  

/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int main(void)
{
	uint8_t Sector1[20],Sector2[20],Sector3[20];
	SystemCoreClockUpdate();
	SysTick_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	/*初始化USART 配置模式为 115200 8-N-1，中断接收*/
  USART_Config();
	/* 蜂鸣器 端口配置 */ 
	Beep_Gpio_Init();
	/* 定时器初始化 */
	RFID1356M_init();

	
	while(1)
	{
			if(IC_read(Sector1,Sector2,Sector3)==USERCARD_OK)
			{
				Usart_SendArray(USART1,Sector1,16);
				
			}			
	}
}
/*********************************************END OF FILE**********************/
