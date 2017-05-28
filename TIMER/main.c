
#include<stm32f4xx.h>
#include<stm32f4xx_gpio.h>
#include<stm32f4xx_rcc.h>
#include<stm32f4xx_tim.h>

int main(void)
{
	GPIO_InitTypeDef code;

	 RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOD , ENABLE);
	  code.GPIO_Pin  = GPIO_Pin_All;
	  code.GPIO_Mode = GPIO_Mode_OUT;
	  code.GPIO_Speed = GPIO_Speed_25MHz;
	  code.GPIO_OType = GPIO_OType_PP;
	  code.GPIO_PuPd = GPIO_PuPd_NOPULL;

	  GPIO_Init(GPIOD,&code);

	  TIM_TimeBaseInitTypeDef timer;
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	    timer.TIM_Period = 39210;
	    timer.TIM_Prescaler = 0x00FF;
	    timer.TIM_ClockDivision = 0;
	    timer.TIM_CounterMode = TIM_CounterMode_Up;
	    timer.TIM_RepetitionCounter = 0x0000;

	    TIM_TimeBaseInit(TIM2,&timer);
	    TIM_Cmd(TIM2,ENABLE);



    while(1)
    {
    	if(TIM_GetFlagStatus(TIM2, TIM_FLAG_Update)!=RESET)
    	    {
    	      TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    	      GPIO_ToggleBits(GPIOD, GPIO_Pin_All);
    	    }


    }
}
