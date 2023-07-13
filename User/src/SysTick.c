  
#include "SysTick.h"
#include "core_cm3.h"
#include "misc.h"
extern void RFID_TimeOut_Count(void);
static __IO u32 TimingDelay,SYS_Timer=0;
/**
  * @brief  ����ϵͳ�δ�ʱ�� SysTick
  * @param  ��
  * @retval ��
  */
void SysTick_Init(void)
{
	/* SystemFrequency / 1000    1ms�ж�һ��
	 * SystemFrequency / 100000	 10us�ж�һ��
	 * SystemFrequency / 1000000 1us�ж�һ��
	 */
//	if (SysTick_Config(SystemFrequency / 100000))	// ST3.0.0��汾
	if (SysTick_Config(SystemCoreClock / 1000))	// ST3.5.0��汾
	{ 
		/* Capture error */ 
		while (1);
	}
}
/**
  * @brief   ms��ʱ����,1msΪһ����λ
  * @param  
  *	@arg nTime: Delay_ms( 1 ) ��ʵ�ֵ���ʱΪ 1ms
  * @retval  ��
  */
void Delay_ms(__IO uint32_t nTime)
{ 
	TimingDelay = nTime;	

	while(TimingDelay != 0);
}


/**
  * @brief  ��ȡ���ĳ���
  * @param  ��
  * @retval ��
  * @attention  �� SysTick �жϺ��� SysTick_Handler()����
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
