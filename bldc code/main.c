#include<stm32f4xx.h>
#include<stm32f4xx_gpio.h>
#include<stm32f4xx_rcc.h>
#include<stm32f4xx_tim.h>

//This is the code for PWM, varying the brightness of LEDS

void enableGPIO(){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);


	GPIO_InitTypeDef PORTD;

	PORTD.GPIO_Pin= GPIO_Pin_12;
	PORTD.GPIO_Mode=GPIO_Mode_AF;
	PORTD.GPIO_Speed=GPIO_Speed_50MHz;
	PORTD.GPIO_PuPd=GPIO_PuPd_NOPULL;
	PORTD.GPIO_OType=GPIO_OType_PP;
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_TIM4);
	GPIO_Init(GPIOD, &PORTD); //FOR PWM

	GPIO_InitTypeDef PORTB;

		PORTB.GPIO_Pin= GPIO_Pin_0| GPIO_Pin_1;
		PORTB.GPIO_Mode=GPIO_Mode_OUT;
		PORTB.GPIO_Speed=GPIO_Speed_50MHz;
		PORTB.GPIO_PuPd=GPIO_PuPd_NOPULL;
		PORTB.GPIO_OType=GPIO_OType_PP;

		GPIO_Init(GPIOB, &PORTB); //FOR CW(pin 0 high) or CCW(pin 1 high)
}
void enabletimer4(){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

		TIM_TimeBaseInitTypeDef Timer4;
		Timer4.TIM_Prescaler=131;
		Timer4.TIM_CounterMode=TIM_CounterMode_Up;
		Timer4.TIM_Period=127;
		Timer4.TIM_ClockDivision=TIM_CKD_DIV1;
		Timer4.TIM_RepetitionCounter = 0;

		TIM_OC1PreloadConfig (TIM4,TIM_OCPreload_Enable);
		TIM_TimeBaseInit(TIM4, &Timer4);
}

void pwminit(){
	//PAGE 529 of register description
	TIM_OCInitTypeDef pwm;
	pwm.TIM_OCMode=TIM_OCMode_PWM2;
	pwm.TIM_OCPolarity=TIM_OCPolarity_Low;
	pwm.TIM_OutputState=TIM_OutputState_Enable;
	pwm.TIM_Pulse=0;
	TIM_OC1Init(TIM4,&pwm);
	TIM_CCxCmd(TIM4, TIM_Channel_1, TIM_CCx_Enable);

}

int main(void)
{
	enableGPIO();
	enabletimer4();
	pwminit();

	//This is for CW. For CCW, invert
	GPIO_SetBits ( GPIOB , GPIO_Pin_0 );
	GPIO_ResetBits ( GPIOB , GPIO_Pin_1 );

	//Frequency of timer is 1600Hz
	//The LEDS are on timer4

	//x is percentage duty cycle, set compare = 1000x
	//x to be inputed between 10% and 90%
	//int x=100;
	TIM_Cmd(TIM4, ENABLE);
	TIM_SetCompare1(TIM4 , 63 );
    while(1){}
}
