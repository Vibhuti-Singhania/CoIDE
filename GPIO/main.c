#include<stm32f4xx.h>
#include<stm32f4xx_gpio.h>
#include<stm32f4xx_rcc.h>
int main(void)
{
	GPIO_InitTypeDef code;

	 RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOD , ENABLE);
	  code.GPIO_Pin  = GPIO_Pin_13;
	  code.GPIO_Mode = GPIO_Mode_OUT;
	  code.GPIO_Speed = GPIO_Speed_25MHz;
	  code.GPIO_OType = GPIO_OType_PP;
	  code.GPIO_PuPd = GPIO_PuPd_NOPULL;

	  GPIO_Init(GPIOD,&code);



    while(1)
    {
     GPIO_SetBits (GPIOD,GPIO_Pin_13);
    }

}
