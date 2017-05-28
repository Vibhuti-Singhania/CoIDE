#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_spi.h>
#include <stm32f4xx_usart.h>
#include <tm_stm32f4_gpio.h>
#include <tm_stm32f4_spi.h>
#include <tm_stm32f4_nrf24l01.h>
#include <misc.h>

int flag = 0;

uint8_t MyAddress[] = {0xE7,0xE7,0xE7,0xE7,0xE7};
uint8_t TxAddress[] = {0x7E,0x7E,0x7E,0x7E,0x7E};
uint8_t dataOut[32];

TM_NRF24L01_Transmit_Status_t transmissionStatus;

void USART1_GPIO_Init();
void USART1_Initialize();
void NVIC_Initialize();
void Enable_USART1();
void nRF_Init();
void waitforsend();
void LEDinit();

void USART1_IRQHandler(){
    static int i = 0;
    if(USART_GetITStatus  (USART1,USART_IT_RXNE)==SET){
        USART_ClearITPendingBit (USART1
        		,USART_IT_RXNE);

        dataOut[i] = (uint8_t) USART_ReceiveData ( USART1 );
        i++;
        if(i==32){
            i = 0;
            flag = 1;
        }

        //while(USART_GetFlagStatus (USART3,USART_FLAG_TXE)==RESET);
        //USART_SendData (USART3, data);
    }
}


int main(void)
{
    int j;
    //SystemInit();
    USART1_GPIO_Init();
    USART1_Initialize();
    NVIC_Initialize();
    Enable_USART1();
    LEDinit();
    nRF_Init();

    while(1)
    {
        if(flag == 1){
            flag = 0;

            TM_NRF24L01_Transmit(dataOut);
            GPIO_SetBits(GPIOD,GPIO_Pin_12);
            waitforsend();
            GPIO_ResetBits(GPIOD,GPIO_Pin_12);
            //for(j=0;j<100000;j++);
        }
    }
}
USART1_GPIO_Init(){

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_USART1); //TX
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_USART1); //RX

    GPIO_InitTypeDef gpio;
        gpio.GPIO_Mode = GPIO_Mode_AF;
        gpio.GPIO_OType = GPIO_OType_PP;
        gpio.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
        gpio.GPIO_PuPd = GPIO_PuPd_UP;
        gpio.GPIO_Speed = GPIO_Speed_50MHz;

        GPIO_Init(GPIOB, &gpio);
}
void USART1_Initialize(){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

        USART_InitTypeDef usart1;
        usart1.USART_BaudRate = 9600;
        usart1.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
        usart1.USART_Parity = USART_Parity_No;
        usart1.USART_StopBits = USART_StopBits_1;
        usart1.USART_WordLength = USART_WordLength_8b;
        usart1.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

        USART_Init(USART1, &usart1);
}
void NVIC_Initialize(){

     NVIC_InitTypeDef interrupt;
     interrupt.NVIC_IRQChannel = USART1_IRQn; //In file stm32f4xx.h, line 140
     interrupt.NVIC_IRQChannelPreemptionPriority = 0;
     interrupt.NVIC_IRQChannelSubPriority = 0;
     interrupt.NVIC_IRQChannelCmd = ENABLE;

     NVIC_Init (&interrupt);

     USART_ITConfig (USART1, USART_IT_RXNE, ENABLE);
}
void Enable_USART1(){
    USART_Cmd (USART1,ENABLE);
}
void nRF_Init(){
     TM_NRF24L01_Init(15, 32);
     TM_NRF24L01_SetRF(TM_NRF24L01_DataRate_2M, TM_NRF24L01_OutputPower_M18dBm);
     TM_NRF24L01_SetMyAddress(MyAddress);
     TM_NRF24L01_SetTxAddress(TxAddress);
}
void waitforsend(){
    do {
        transmissionStatus = TM_NRF24L01_GetTransmissionStatus();
    } while (transmissionStatus == TM_NRF24L01_Transmit_Status_Sending);
}
void LEDinit(){
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
        GPIO_InitTypeDef led;

        led.GPIO_Mode = GPIO_Mode_OUT;
        led.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
        led.GPIO_PuPd = GPIO_PuPd_DOWN;
        led.GPIO_OType = GPIO_OType_PP;
        led.GPIO_Speed = GPIO_Speed_50MHz;

        GPIO_Init ( GPIOD , &led );
}


