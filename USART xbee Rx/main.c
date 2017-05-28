#include<stm32f4xx.h>
#include<stm32f4xx_gpio.h>
#include<stm32f4xx_rcc.h>
#include<stm32f4xx_usart.h>

uint16_t value;

void InitializeGPIO()
{
	GPIO_InitTypeDef PortD;

    RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOD , ENABLE);
	  PortD.GPIO_Pin  = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	  PortD.GPIO_Mode = GPIO_Mode_OUT;
	  PortD.GPIO_Speed = GPIO_Speed_25MHz;
	  PortD.GPIO_OType = GPIO_OType_PP;
	  PortD.GPIO_PuPd = GPIO_PuPd_NOPULL;

	 GPIO_Init(GPIOD,&PortD);

	 GPIO_InitTypeDef PortB;

	 RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOB , ENABLE);
	 GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //USART3_RX
	 GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); //USART3_TX
	   PortB.GPIO_Pin  = GPIO_Pin_11| GPIO_Pin_10;
	   PortB.GPIO_Mode = GPIO_Mode_AF;
	   PortB.GPIO_Speed = GPIO_Speed_25MHz;
	   PortB.GPIO_OType = GPIO_OType_PP;
	   PortB.GPIO_PuPd = GPIO_PuPd_UP;
	 GPIO_Init(GPIOB,&PortB);
}

void USART_Initialize()
{
	USART_InitTypeDef usart;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	usart.USART_BaudRate=9600;
    usart.USART_WordLength=USART_WordLength_8b;
    usart.USART_StopBits=USART_StopBits_1;
    usart.USART_Parity=USART_Parity_No;
    usart.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
    usart.USART_HardwareFlowControl=USART_HardwareFlowControl_None;

    USART_Init(USART3,&usart);
    USART_Cmd(USART3,ENABLE) ;
}
int main(void)
{
     InitializeGPIO();
     GPIO_SetBits (GPIOD,GPIO_Pin_13);
     USART_Initialize();

    while(1)
    {
    	if(USART_GetFlagStatus(USART3, USART_FLAG_RXNE)!=RESET){
            USART_ClearFlag(USART3, USART_FLAG_RXNE);
  			GPIO_ToggleBits(GPIOD, GPIO_Pin_14);
  			USART_ReceiveData(USART3);
    	  }
     }
}
