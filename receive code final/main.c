#include<stm32f4xx_gpio.h>
#include<stm32f4xx_rcc.h>
#include <stm32f4xx_tim.h>
#include<stm32f4xx_spi.h>
#include<tm_stm32f4_gpio.h>
#include<tm_stm32f4_nrf24l01.h>
#include<tm_stm32f4_spi.h>

uint8_t MyAddress[] = { 0xE7,0xE7,0xE7,0xE7,0xE7};
uint8_t TxAddress[] = { 0x7E,0x7E,0x7E,0x7E,0x7E};
uint8_t dataR[32];
int data1,data2,data3,data4,i;

void InitializeGPIO()
{
	GPIO_InitTypeDef PortE;

	 RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOE , ENABLE);
	  PortE.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1;
	  PortE.GPIO_Mode = GPIO_Mode_OUT;
	  PortE.GPIO_Speed = GPIO_Speed_25MHz;
	  PortE.GPIO_OType = GPIO_OType_PP;
	  PortE.GPIO_PuPd = GPIO_PuPd_NOPULL;

	  GPIO_Init(GPIOE,&PortE);
}
void enableGPIO(){

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef PORTB;

		PORTB.GPIO_Pin= GPIO_Pin_0 | GPIO_Pin_1;
		PORTB.GPIO_Mode=GPIO_Mode_OUT;
		PORTB.GPIO_Speed=GPIO_Speed_50MHz;
		PORTB.GPIO_PuPd=GPIO_PuPd_NOPULL;
		PORTB.GPIO_OType=GPIO_OType_PP;

		GPIO_Init(GPIOB, &PORTB);
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
void GPIOfortimer(){
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

		GPIO_InitTypeDef PORTB2;

		PORTB2.GPIO_Pin= GPIO_Pin_10;
		PORTB2.GPIO_Mode=GPIO_Mode_AF;
		PORTB2.GPIO_Speed=GPIO_Speed_50MHz;
		PORTB2.GPIO_PuPd=GPIO_PuPd_NOPULL;
		PORTB2.GPIO_OType=GPIO_OType_PP;
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_TIM2);

		GPIO_Init(GPIOB, &PORTB2); //PWM for dribbler
	}
void enabletimer4and2(){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	TIM_TimeBaseInitTypeDef Timer4;
		Timer4.TIM_Prescaler=131;
		Timer4.TIM_CounterMode=TIM_CounterMode_Up;
		Timer4.TIM_Period=127;
		Timer4.TIM_ClockDivision=TIM_CKD_DIV1;
		Timer4.TIM_RepetitionCounter = 0;

		TIM_OC1PreloadConfig (TIM4,TIM_OCPreload_Enable);
		TIM_TimeBaseInit(TIM4, &Timer4);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_TimeBaseInitTypeDef Timer2;
		Timer2.TIM_Prescaler=131;
		Timer2.TIM_CounterMode=TIM_CounterMode_Up;
		Timer2.TIM_Period=127;
		Timer2.TIM_ClockDivision=TIM_CKD_DIV1;
		Timer2.TIM_RepetitionCounter = 0;

		TIM_OC1PreloadConfig (TIM2,TIM_OCPreload_Enable);
		TIM_TimeBaseInit(TIM2, &Timer2);

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

	TIM_Cmd(TIM4, ENABLE);

	     TIM_SetCompare1(TIM4 , 0);
		 TIM_SetCompare2(TIM4 , 0);
		 TIM_SetCompare3(TIM4 , 0);
		 TIM_SetCompare4(TIM4 , 0);

		 TIM_OCInitTypeDef pwm2;
		 	pwm2.TIM_OCMode=TIM_OCMode_PWM2;
		 	pwm2.TIM_OCPolarity=TIM_OCPolarity_Low;
		 	pwm2.TIM_OutputState=TIM_OutputState_Enable;
		 	pwm2.TIM_Pulse=0;

		 	TIM_OC3Init ( TIM2 , &pwm2 );
		 	TIM_CCxCmd(TIM2 , TIM_Channel_3, TIM_CCx_Enable);
		 	TIM_Cmd(TIM2, ENABLE);

		 	 GPIO_SetBits ( GPIOE , GPIO_Pin_13 );
		 	 GPIO_ResetBits ( GPIOE , GPIO_Pin_14 );
		 	 TIM_SetCompare3(TIM2 , 50);
	}
void Receive()
{
	TM_NRF24L01_Init(15,32)	;
	TM_NRF24L01_SetRF(TM_NRF24L01_DataRate_2M, TM_NRF24L01_OutputPower_M18dBm );
	TM_NRF24L01_SetMyAddress(MyAddress);
	TM_NRF24L01_SetTxAddress(TxAddress);
}
void CW_CCW(){

	if(data1<=127){
		        GPIO_SetBits ( GPIOB , GPIO_Pin_0 );
				GPIO_ResetBits ( GPIOB , GPIO_Pin_1 );
				TIM_SetCompare1(TIM4 , (data1*0.8) + 12.7);

	}

	else if(data1>127)	{
				 GPIO_ResetBits ( GPIOB , GPIO_Pin_0 );
				 GPIO_SetBits ( GPIOB , GPIO_Pin_1 );
				 TIM_SetCompare1(TIM4 , ((0.8*(255-data1)) + 12.7));

				}
	if(data2<=127){

					GPIO_SetBits ( GPIOE , GPIO_Pin_7 );
					GPIO_ResetBits ( GPIOE , GPIO_Pin_8 );
					TIM_SetCompare2(TIM4 , (data2*0.8) + 12.7);

	}

					else if(data2>127)	{

						  GPIO_ResetBits ( GPIOE , GPIO_Pin_7 );
						  GPIO_SetBits ( GPIOE , GPIO_Pin_8 );
						  TIM_SetCompare2(TIM4 , ((0.8*(255-data2)) + 12.7));
					}
	if(data3<=127){

					GPIO_SetBits ( GPIOE , GPIO_Pin_9 );
					GPIO_ResetBits ( GPIOE , GPIO_Pin_10);
					TIM_SetCompare3(TIM4 , (data3*0.8) + 12.7);
		}

					else if(data3>127)	{

						  GPIO_ResetBits ( GPIOE , GPIO_Pin_9 );
						  GPIO_SetBits ( GPIOE , GPIO_Pin_10);
						  TIM_SetCompare3(TIM4 , ((0.8*(255-data3)) + 12.7));

					}

	if(data4<=127){
	                GPIO_SetBits ( GPIOE , GPIO_Pin_11 );
					GPIO_ResetBits ( GPIOE , GPIO_Pin_12 );
					TIM_SetCompare4(TIM4 , (data4*0.8) + 12.7);
		}

					else if(data4>127)	{

						  GPIO_ResetBits ( GPIOE , GPIO_Pin_11 );
						  GPIO_SetBits ( GPIOE , GPIO_Pin_12 );
						  TIM_SetCompare4(TIM4 , ((0.8*(255-data4)) + 12.7));

					}
}

int main(void)
{
	//SystemInit();
	InitializeGPIO();
	enableGPIO();
	GPIOfortimer();
	enabletimer4and2();
	pwminit();
	Receive();


    	 while(1)
    	    {
    		 if (TM_NRF24L01_DataReady())
    		     	{
    		     	           TM_NRF24L01_GetData(dataR);
    		     	           GPIO_SetBits ( GPIOE , GPIO_Pin_1 );
    		                        if(dataR[0]==80) {
    		                        	data1=(int)dataR[1];
    		                        	data2=(int)dataR[2];
    		                        	data3=(int)dataR[3];
    		                        	data4=(int)dataR[4];
    		                        	CW_CCW();

    		                        }
    	        }
    	}
}
/*
 mapping: pwm cycle = y(10-90%)
          x = compare value(0 - 127)
          80*x/127 + 10 = y
          compare value(set by us) = value send by coding team*0.8 + 12.7
          pwm value = 62.5*( % duty cycle(set by us))-625
 */
