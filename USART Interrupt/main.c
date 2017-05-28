#include<stm32f4xx.h>
#include<stm32f4xx_gpio.h>
#include<stm32f4xx_rcc.h>
#include<stm32f4xx_usart.h>
#include<misc.h>

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
}

void USART_Initialize()
{

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


	USART_InitTypeDef usart;


	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	usart.USART_BaudRate=9600;
    usart.USART_WordLength=USART_WordLength_8b;
    usart.USART_StopBits=USART_StopBits_1;
    usart.USART_Parity=USART_Parity_No;
    usart.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;;
    usart.USART_HardwareFlowControl=USART_HardwareFlowControl_None;

    USART_Init(USART3,&usart);

    USART_Cmd(USART3,ENABLE) ;

    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    USART_ITConfig(USART3, USART_IT_TC, ENABLE);
   // NVIC_EnableIRQ(USART3_IRQn);
   }

void InitializeNVIC()
{
	 NVIC_InitTypeDef A;
	 A.NVIC_IRQChannel=USART3_IRQn;
	 A.NVIC_IRQChannelPreemptionPriority=0;
	 A.NVIC_IRQChannelSubPriority=0;
	 A.NVIC_IRQChannelCmd=ENABLE;
	 NVIC_Init(&A);
}


int main(void)

{
	InitializeGPIO();
	//GPIO_SetBits (GPIOD,GPIO_Pin_13);
	USART_Initialize();
	InitializeNVIC();



    while(1)
    {
    }
}

void USART3_IRQHandler()
{
//char a;

    // RXNE handler
  //  if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
   // {
    	// USART_ClearITPendingBit (USART3 , USART_IT_RXNE);
        // If received 'a', toggle LED and transmit 'A'
        if(USART_ReceiveData(USART3) == 'a')
        {
        	GPIO_ResetBits (GPIOD,GPIO_Pin_13);

            USART_SendData(USART3, 'A');

        }
   // }


}

