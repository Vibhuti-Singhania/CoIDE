#include<stm32f4xx.h>
#include<stm32f4xx_gpio.h>
#include<stm32f4xx_rcc.h>
#include<stm32f4xx_exti.h>
#include<misc.h>
#include<stm32f4xx_syscfg.h>
void InitializeGPIO()
{
	GPIO_InitTypeDef PortD;

	 RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOD , ENABLE);
	  PortD.GPIO_Pin  = GPIO_Pin_13;
	  PortD.GPIO_Mode = GPIO_Mode_OUT;
	  PortD.GPIO_Speed = GPIO_Speed_25MHz;
	  PortD.GPIO_OType = GPIO_OType_PP;
	  PortD.GPIO_PuPd = GPIO_PuPd_NOPULL;

	  GPIO_Init(GPIOD,&PortD);

	  GPIO_InitTypeDef PortA;

	  	 RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOA , ENABLE);
	  	  PortA.GPIO_Pin  = GPIO_Pin_0;
	  	  PortA.GPIO_Mode = GPIO_Mode_IN;
	  	  PortA.GPIO_Speed = GPIO_Speed_25MHz;
	  	  PortA.GPIO_OType = GPIO_OType_PP;
	  	  PortA.GPIO_PuPd = GPIO_PuPd_NOPULL;

	  	  GPIO_Init(GPIOA,&PortA);

}
void InitializeExtin()
{
	EXTI_InitTypeDef extint;
	extint.EXTI_Line=EXTI_Line0;
	extint.EXTI_Mode=EXTI_Mode_Interrupt ;
	extint.EXTI_Trigger=EXTI_Trigger_Rising ;
	extint.EXTI_LineCmd=ENABLE;
	EXTI_Init (&extint);

}

void InitializeNVIC()
{
	 NVIC_InitTypeDef interr;
	 interr.NVIC_IRQChannel=EXTI0_IRQn;
	 interr.NVIC_IRQChannelPreemptionPriority=0;
	 interr.NVIC_IRQChannelSubPriority=0;
	 interr.NVIC_IRQChannelCmd=ENABLE;
	 NVIC_Init (&interr);
}

int main(void)
{
	 RCC_APB2PeriphClockCmd (RCC_APB2Periph_SYSCFG,ENABLE);
	 SYSCFG_EXTILineConfig (EXTI_PortSourceGPIOA,EXTI_PinSource0);
	 InitializeGPIO();
	 InitializeExtin();
	 InitializeNVIC();

    while(1)
    {
    }
}
void EXTI0_IRQHandler()
{
	if(EXTI_GetITStatus(EXTI_Line0)!=RESET)
	{
		 EXTI_ClearITPendingBit(EXTI_Line0) ;
		 GPIO_ToggleBits(GPIOD,GPIO_Pin_13);
	}

}
