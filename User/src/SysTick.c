  
#include "SysTick.h"
#include "core_cm3.h"
#include "misc.h"
extern void RFID_TimeOut_Count(void);
static __IO u32 TimingDelay,SYS_Timer=0;
/**
  * @brief  启动系统滴答定时器 SysTick
  * @param  无
  * @retval 无
  */
void SysTick_Init(void)
{
	/* SystemFrequency / 1000    1ms中断一次
	 * SystemFrequency / 100000	 10us中断一次
	 * SystemFrequency / 1000000 1us中断一次
	 */
//	if (SysTick_Config(SystemFrequency / 100000))	// ST3.0.0库版本
	if (SysTick_Config(SystemCoreClock / 1000))	// ST3.5.0库版本
	{ 
		/* Capture error */ 
		while (1);
	}
}
/**
  * @brief   ms延时程序,1ms为一个单位
  * @param  
  *	@arg nTime: Delay_ms( 1 ) 则实现的延时为 1ms
  * @retval  无
  */
void Delay_ms(__IO uint32_t nTime)
{ 
	TimingDelay = nTime;	

	while(TimingDelay != 0);
}


/**
  * @brief  获取节拍程序
  * @param  无
  * @retval 无
  * @attention  在 SysTick 中断函数 SysTick_Handler()调用
  */
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
	SYS_Timer++;
	
}




/*********************************************END OF FILE**********************/
