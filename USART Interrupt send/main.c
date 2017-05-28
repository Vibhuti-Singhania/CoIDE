#include<stm32f4xx.h>
#include<stm32f4xx_gpio.h>
#include<stm32f4xx_rcc.h>
#include<stm32f4xx_usart.h>
#include<misc.h>
#include<stm32f4xx_spi.h>
#include<tm_stm32f4_gpio.h>
#include<tm_stm32f4_nrf24l01.h>
#include<tm_stm32f4_spi.h>
#include<tm_stm32f4_usart.h>

uint8_t MyAddress[] = { 0xE7,0xE7,0xE7,0xE7,0xE7};
uint8_t TxAddress[] = { 0x7E,0x7E,0x7E,0x7E,0x7E};
uint8_t dataS[32];
int i,j,flag=0;
TM_NRF24L01_Transmit_Status_t transmissionStatus;

void InitializeGPIO()
{
	GPIO_InitTypeDef PortD;

	 RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOD , ENABLE);
	  PortD.GPIO_Pin  = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_12;
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
}
void InitializeNVIC()
{
	 NVIC_InitTypeDef A;
	 A.NVIC_IRQChannel=USART3_IRQn;
	 A.NVIC_IRQChannelPreemptionPriority=0;
	 A.NVIC_IRQChannelSubPriority=0;
	 A.NVIC_IRQChannelCmd=ENABLE;
	 NVIC_Init(&A);
	 USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
}
void Transmission()
{
	TM_NRF24L01_Init(20,32)	;
	TM_NRF24L01_SetRF(TM_NRF24L01_DataRate_250k,TM_NRF24L01_OutputPower_0dBm);
	TM_NRF24L01_SetMyAddress(MyAddress);
	TM_NRF24L01_SetTxAddress(TxAddress);
}
void sending()
{
	do{
	transmissionStatus = TM_NRF24L01_GetTransmissionStatus();
	  }
while ( transmissionStatus == TM_NRF24L01_Transmit_Status_Sending );
}


void datainit()
{ for(j=1;j<32;j++)
    dataS[j]=1 ;
}

int main(void)

{
	InitializeGPIO();
	//SystemInit();
	USART_Initialize();
    InitializeNVIC();
    USART_Cmd (USART3,ENABLE);
    Transmission();
    datainit();
    while(1)
    {	  GPIO_SetBits (GPIOD , GPIO_Pin_12);
        if(flag==1){
                     flag=0;
                     TM_NRF24L01_Transmit(dataS);
                     GPIO_SetBits (GPIOD , GPIO_Pin_13);
                     sending();
                     GPIO_ResetBits (GPIOD , GPIO_Pin_13);
//                   for(i=0;i<1000000;i++);   //delay}
        }
    }
}

void USART3_IRQHandler()
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {
//	    USART_ClearFlag(USART3, USART_FLAG_RXNE);
		 USART_ClearITPendingBit (USART3,USART_IT_RXNE);
	    dataS[0] = (uint8_t)USART_ReceiveData(USART3);
	    flag=1;
	}

}


