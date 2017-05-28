#include<stm32f4xx.h>
#include<stm32f4xx_gpio.h>
#include<stm32f4xx_rcc.h>
#include<stm32f4xx_tim.h>
#include<misc.h>

int main(void)
{
	GPIO_InitTypeDef code;

	 RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOD , ENABLE);
	  code.GPIO_Pin  =  GPIO_Pin_13;
	  code.GPIO_Mode =  GPIO_Mode_OUT;
	  code.GPIO_Speed = GPIO_Speed_25MHz;
	  code.GPIO_OType = GPIO_OType_PP;
	  code.GPIO_PuPd =  GPIO_PuPd_NOPULL;

	 GPIO_Init(GPIOD,&code);

	  TIM_TimeBaseInitTypeDef timer;
	 	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	 	    timer.TIM_Period = 1000000-1;
	 	    timer.TIM_Prescaler =16;
	 	    timer.TIM_ClockDivision = 0;
	 	    timer.TIM_CounterMode = TIM_CounterMode_Up;
	 	    timer.TIM_RepetitionCounter = 0x0000;

	 	    TIM_TimeBaseInit(TIM2,&timer);
	 	    TIM_Cmd(TIM2,ENABLE);

	   NVIC_InitTypeDef interrupt;
	 	  interrupt.NVIC_IRQChannel=TIM2_IRQn;
	 	  interrupt.NVIC_IRQChannelPreemptionPriority=0;
	   	  interrupt.NVIC_IRQChannelSubPriority=1;
	 	  interrupt.NVIC_IRQChannelCmd=ENABLE;

	 	   NVIC_Init(&interrupt);

    TIM_ITConfig (TIM2,TIM_IT_Update,ENABLE);


    while(1)
    {
    }
}

void TIM2_IRQHandler()
{

	  if (TIM_GetITStatus(TIM2, TIM_IT_Update)!= RESET)
	    	  {
	    	    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	    	    GPIO_ToggleBits(GPIOD,GPIO_Pin_13);
	    	  }

}
