#include <string.h>
#include <stdio.h>
#include "rfid125k.h"


void RFID_CLK_Init(void)
{

    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    // ����Ƚ�ͨ�� GPIO ��ʼ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);

    // �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
    TIM_TimeBaseStructure.TIM_Period	=	8-1;
    // ����CNT��������ʱ�� = Fck_int/(psc+1)
    TIM_TimeBaseStructure.TIM_Prescaler	= 	48-1;
    // ʱ�ӷ�Ƶ���� ����������ʱ��ʱ��Ҫ�õ�
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
    // ����������ģʽ������Ϊ���ϼ���
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
    // �ظ���������ֵ��û�õ����ù�
    TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
    // ��ʼ����ʱ��
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    /*--------------------����ȽϽṹ���ʼ��-------------------*/
    // ռ�ձ�����
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    // ����ΪPWMģʽ1
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    // ���ʹ��
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    // ���ͨ����ƽ��������
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    // ����Ƚ�ͨ�� 2
    TIM_OCInitStructure.TIM_Pulse = 	4	;
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);

    // ʹ�ܼ�����
    TIM_Cmd(TIM2, ENABLE);

}



void RFID_DAT_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    /*��������GPIO�ڵ�ʱ��*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    /* ѡ�񰴼��õ���GPIO */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    /* ����Ϊ�������� */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    // ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    // �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
    TIM_TimeBaseStructure.TIM_Period=(1000-1);					//
    // ʱ��Ԥ��Ƶ��
    TIM_TimeBaseStructure.TIM_Prescaler= 48-1;					//1M
    // ʱ�ӷ�Ƶ���� ��û�õ����ù�
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
    // ����������ģʽ������Ϊ���ϼ���
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
    // �ظ���������ֵ��û�õ����ù�
    TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
    // ��ʼ����ʱ��
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    // ����������жϱ�־λ

//    TIM_ClearFlag(TIM3, TIM_FLAG_Update);
//
//		// �����������ж�
//    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);

    // ʹ�ܼ�����
//	TIM_Cmd(TIM3, ENABLE);
    TIM_Cmd(TIM3, DISABLE);
}

void RFID_Init(void)
{
    RFID_CLK_Init();
    RFID_DAT_Init();

}


/*********************************************END OF FILE**********************/
