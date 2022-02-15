


#include "stm32f10x.h"
#include "bsp_usart.h"
#include "bsp_adc.h"


extern __IO uint16_t ADC_ConvertedValue;

 	 
float ADC_ConvertedValueLocal;        


void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  
	
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);    
 
   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
 
	TIM_TimeBaseStructure.TIM_Period = arr; 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 
	
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OC2Init(TIM3, &TIM_OCInitStructure); 
	
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable); 
 
	TIM_Cmd(TIM3, ENABLE);  
	
}
void delay_ms(double nms)
{
	u32 temp;
	SysTick->LOAD=9000*nms;
	SysTick->VAL=0x00;
	SysTick->CTRL=0x01;
	do
	{
		temp=SysTick->CTRL;
	}
	while((temp&0x01)&&(!(temp&(1<<16))));
	SysTick->CTRL=0x00;
	SysTick->VAL=0x00;

}


int main(void)
{		int count=0;

	
	USART_Config();
	TIM3_PWM_Init(899,0);
	ADCx_Init();
	TIM_SetCompare2(TIM3, 390);
	while (1)
	{
		ADC_ConvertedValueLocal =(float) ADC_ConvertedValue/4096*3.3; 
		if(ADC_ConvertedValueLocal>3.0050)
    //阈值电压，通过万用表读取设定重量时的电压是多少，在这里可以更换设定值。default = 3.0250
		{
			count++;
				}
		if(count>70&&count<140)
		{
			
			TIM_SetCompare2(TIM3, 420);
		
		}
		if(count>140&&count<160)
		{
		TIM_SetCompare2(TIM3, 600);
		
		}
		if(count>200)
		{
		TIM_SetCompare2(TIM3, 750);
		return 0;
		}
		
	
		delay_ms(12.5);
	}
	
}

