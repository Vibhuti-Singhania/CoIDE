#include <stm32f4xx.h>
#include <stm32f4xx_tim.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_gpio.h>

//This is the code for PWM, varying the brightness of LEDS

void enableGPIO(){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_InitTypeDef PORTD;

	PORTD.GPIO_Pin= GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 |GPIO_Pin_15 ;
	PORTD.GPIO_Mode=GPIO_Mode_AF;
	PORTD.GPIO_Speed=GPIO_Speed_50MHz;
	PORTD.GPIO_PuPd=GPIO_PuPd_NOPULL;
	PORTD.GPIO_OType=GPIO_OType_PP;
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource13,GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource14,GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource15,GPIO_AF_TIM4);

	GPIO_Init(GPIOD, &PORTD); //FOR PWM

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef PORTD2;

		PORTD2.GPIO_Pin= GPIO_Pin_0 | GPIO_Pin_1;
		PORTD2.GPIO_Mode=GPIO_Mode_OUT;
		PORTD2.GPIO_Speed=GPIO_Speed_50MHz;
		PORTD2.GPIO_PuPd=GPIO_PuPd_NOPULL;
		PORTD2.GPIO_OType=GPIO_OType_PP;

		GPIO_Init(GPIOB, &PORTD2);
		//FOR MOTOR1
		//FOR CW(pin 0 high) or CCW(pin 1 high)

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	GPIO_InitTypeDef PORTE2;

		PORTE2.GPIO_Pin= GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12| GPIO_Pin_13 |GPIO_Pin_14;
		PORTE2.GPIO_Mode=GPIO_Mode_OUT;
		PORTE2.GPIO_Speed=GPIO_Speed_50MHz;
		PORTE2.GPIO_PuPd=GPIO_PuPd_NOPULL;
		PORTE2.GPIO_OType=GPIO_OType_PP;

		GPIO_Init(GPIOE, &PORTE2);
		//FOR MOTOR 2
		//FOR CW(pin 7 high) or CCW(pin 8 high)

		//FOR MOTOR 3
		//FOR CW(pin 9 high) or CCW(pin 10 high)

		//FOR MOTOR 4
		//FOR CW(pin 11 high) or CCW(pin 12 high)

		//FOR MOTOR 5
		//FOR CW(pin 13 high) or CCW(pin 14 high)
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

	TIM_OC1Init ( TIM4 , &pwm );
	TIM_OC2Init ( TIM4 , &pwm );
	TIM_OC3Init ( TIM4 , &pwm );
	TIM_OC4Init ( TIM4 , &pwm );

	TIM_CCxCmd(TIM4, TIM_Channel_1, TIM_CCx_Enable);
	TIM_CCxCmd(TIM4, TIM_Channel_2, TIM_CCx_Enable);
	TIM_CCxCmd(TIM4, TIM_Channel_3, TIM_CCx_Enable);
	TIM_CCxCmd(TIM4, TIM_Channel_4, TIM_CCx_Enable);

}

int main(void)
{
	enableGPIO();
	enabletimer4();
	pwminit();

	//This is for CW. For CCW, invert
	GPIO_SetBits ( GPIOB , GPIO_Pin_0 );
	GPIO_ResetBits ( GPIOB , GPIO_Pin_1 );

	GPIO_SetBits ( GPIOE , GPIO_Pin_7 );
	GPIO_ResetBits ( GPIOE , GPIO_Pin_8 );

	GPIO_SetBits ( GPIOE , GPIO_Pin_9 );
	GPIO_ResetBits ( GPIOE , GPIO_Pin_10);

	GPIO_SetBits ( GPIOE , GPIO_Pin_11 );
	GPIO_ResetBits ( GPIOE , GPIO_Pin_12 );

	GPIO_SetBits ( GPIOE , GPIO_Pin_13 );
	GPIO_ResetBits ( GPIOE , GPIO_Pin_14 );

	//Frequency of timer is 1600Hz
	//The LEDS are on timer4

	//x is percentage duty cycle, set compare = 1000x
	//x to be inputed between 10% and 90%
//	int x=100;
	TIM_Cmd(TIM4, ENABLE);

	TIM_SetCompare1(TIM4 , 0 );
	TIM_SetCompare2(TIM4 , 0);
	TIM_SetCompare3(TIM4 , 0);
	TIM_SetCompare4(TIM4 , 0);
    while(1){
    	TIM_SetCompare1(TIM4 , 40 );
    		TIM_SetCompare2(TIM4 , 40);
    		TIM_SetCompare3(TIM4 , 40);
    		TIM_SetCompare4(TIM4 , 40);
    }
}
