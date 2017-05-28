#include<stm32f4xx_gpio.h>
#include<stm32f4xx_rcc.h>
#include<stm32f4xx_spi.h>
#include<tm_stm32f4_gpio.h>
#include<tm_stm32f4_nrf24l01.h>
#include<tm_stm32f4_spi.h>


	uint8_t TxAddress[] = {0xE7,0xE7,0xE7,0xE7,0xE7};
	uint8_t MyAddress[] = {0x7E,0x7E,0x7E,0x7E,0x7E};
	uint8_t dataR[32];
	int i;

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

void Receive()
{
	TM_NRF24L01_Init(20,32)	;
	TM_NRF24L01_SetRF(TM_NRF24L01_DataRate_250k,TM_NRF24L01_OutputPower_0dBm);
	TM_NRF24L01_SetMyAddress(MyAddress);
	TM_NRF24L01_SetTxAddress(TxAddress);
}
int main(void)
{
	InitializeGPIO();
	//SystemInit();
	Receive();
    while(1)
    {
    	if (TM_NRF24L01_DataReady())
    	{
    	            TM_NRF24L01_GetData(dataR);
    	            //GPIO_ToggleBits(GPIOE , GPIO_Pin_1);
    	            //for(i=0;i<100000;i++);
    	            if(dataR[0]==255)
                   {
    	            GPIO_SetBits(GPIOE , GPIO_Pin_1);
                   }
    	}

      }
}
