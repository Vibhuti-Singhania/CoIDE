#include<stm32f4xx_gpio.h>
#include<stm32f4xx_rcc.h>
#include<stm32f4xx_spi.h>
#include<tm_stm32f4_gpio.h>
#include<tm_stm32f4_nrf24l01.h>
#include<tm_stm32f4_spi.h>

uint8_t MyAddress[] = { 0xE7,0xE7,0xE7,0xE7,0xE7};
uint8_t TxAddress[] = { 0x7E,0x7E,0x7E,0x7E,0x7E};
uint8_t data[32];
int i,j;
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
void Transmission()
{
	TM_NRF24L01_Init(15,32)	;
	TM_NRF24L01_SetRF(TM_NRF24L01_DataRate_250k,TM_NRF24L01_OutputPower_0dBm);
	TM_NRF24L01_SetMyAddress(MyAddress);
	TM_NRF24L01_SetTxAddress(TxAddress);
}
void dowhile()
{ do { transmissionStatus = TM_NRF24L01_GetTransmissionStatus();
} while (transmissionStatus == TM_NRF24L01_Transmit_Status_Sending);}

void datainit()
{
	data[0]=1;     // this is bot id
for(j=1;j<5;j++)
    {data[j]=25;}  //25 is pwm dutycycle
for(j=5;j<32;j++)
    {data[j]=1;}
}

int main(void)
{
	SystemInit();
	InitializeGPIO();
    datainit();
    Transmission();
    while(1)
    {
    	 GPIO_SetBits (GPIOD , GPIO_Pin_12);
         TM_NRF24L01_Transmit(data);
         GPIO_SetBits (GPIOD , GPIO_Pin_13);
         dowhile();
         GPIO_ResetBits (GPIOD , GPIO_Pin_13);
         for(i=0;i<1000;i++);   //delay
     }
}
