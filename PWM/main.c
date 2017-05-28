#include<stm32f4xx.h>
#include<stm32f4xx_gpio.h>
#include<stm32f4xx_rcc.h>
#include<stm32f4xx_tim.h>


void InitializeGPIO()
	{
	 GPIO_InitTypeDef code;
	 RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOD , ENABLE);
	  code.GPIO_Pin  = GPIO_Pin_All;
	  code.GPIO_Mode = GPIO_Mode_AF;
	  code.GPIO_Speed = GPIO_Speed_25MHz;
	  code.GPIO_OType = GPIO_OType_PP;
	  code.GPIO_PuPd = GPIO_PuPd_NOPULL;

	  GPIO_Init(GPIOD,&code);
	  GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
	  GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
	  GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);
	  GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4);

	}
void InitializeTimer()
   {
	  TIM_TimeBaseInitTypeDef timer;
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	    timer.TIM_Period = 100;
	    timer.TIM_Prescaler = 16;
	    timer.TIM_ClockDivision = 0;
	    timer.TIM_CounterMode = TIM_CounterMode_Up;
	    timer.TIM_RepetitionCounter = 0x0000;

	    TIM_TimeBaseInit(TIM4,&timer);
	    TIM_Cmd(TIM4,ENABLE);
   }

void InitializePWMChannel(int i)
{
    TIM_OCInitTypeDef pwm;
    pwm.TIM_OCMode = TIM_OCMode_PWM1;
    pwm.TIM_Pulse = i;
    pwm.TIM_OutputState = TIM_OutputState_Enable;
    pwm.TIM_OCPolarity = TIM_OCPolarity_High;

    TIM_OC1Init(TIM4,&pwm);
    TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable);

}
int main(void)
{
	int a,b;
	InitializeGPIO();
	InitializeTimer();

    while(1)

    {
    	for(a=0;a<=100;a++)
    	{
    		InitializePWMChannel(a);
    		for(b=0;b<=100000;b++);   //delay

    	}
    }
}
