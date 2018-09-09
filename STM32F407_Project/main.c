#include "main.h"
#include <stdio.h>

static inline void Delay_1us(uint32_t nCnt_1us)
{
  volatile uint32_t nCnt;

  for (; nCnt_1us != 0; nCnt_1us--)
    for (nCnt = 13; nCnt != 0; nCnt--);
}

void RCC_Configuration(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD , ENABLE);
}

void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD ;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART3);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART3);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 |GPIO_Pin_13 | GPIO_Pin_14 |GPIO_Pin_15;  // LED is connected to PG13/PG14
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void USART3_Configuration(void)
{
    USART_InitTypeDef USART_InitStructure;

    USART_InitStructure.USART_BaudRate = 57600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART3, &USART_InitStructure);

    USART_Cmd(USART3, ENABLE);
    
    USART_ClearFlag(USART3, USART_FLAG_TC);

    USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

    NVIC_InitTypeDef NVIC_InitStruct = {
      .NVIC_IRQChannel = USART3_IRQn,
      .NVIC_IRQChannelPreemptionPriority = 0,
      .NVIC_IRQChannelSubPriority = 0,
      .NVIC_IRQChannelCmd = ENABLE
    };
    NVIC_Init(&NVIC_InitStruct);
}

void USART3_puts(char* s)
{
    while(*s) {
        while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
        USART_SendData(USART3, *s);
        s++;
    }
}

// void LED3_On(void){

//   GPIO_SetBits(GPIOD,GPIO_Pin_13);

// }

// void LED3_Off(void){

//   GPIO_ResetBits(GPIOD,GPIO_Pin_13);

// }

// void LED5_On(void){

//   GPIO_SetBits(GPIOD,GPIO_Pin_14);

// }

// void LED5_Off(void){

//   GPIO_ResetBits(GPIOD,GPIO_Pin_14);

// }

// void LED3_Toggle(void){


//   GPIO_ToggleBits(GPIOD,GPIO_Pin_13);

// }

// void LED5_Toggle(void){


//   GPIO_ToggleBits(GPIOD,GPIO_Pin_14);

// }

// uint8_t PushButton_Read(void){

//     return GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0);

// }
// USART input and output
// uint16_t a;

void vApplicationStackOverflowHook(void)
{
    while(1);
}
void vApplicationIdleHook(void)
{

}
void vApplicationMallocFailedHook(void)
{
    while(1);
}

int main(void)
{
    RCC_Configuration();
    GPIO_Configuration();
    USART3_Configuration();

    // char buff_transmit[100];
    USART3_puts("Hello World!\r\n");

    //sprintf((char *)buff_transmit, "%x, %x, %x, %x\r\n",*baud, *Reg1, *Reg2, *Reg3);
    // USART3_puts((char *)buff_transmit);

    xTaskCreate((TaskFunction_t)Task1, "Task1", 1024, NULL, 1, NULL);
    xTaskCreate((TaskFunction_t)Task2, "Task2", 1024, NULL, 1, NULL);

    vTaskStartScheduler();

    return 0;
}

void USART3_IRQHandler(void){
  
  if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) 
  {
    uart1_data = USART_ReceiveData(USART3);

    USART_SendData(USART3, uart1_data);
  }
}

void Task1(void)
{
    while(1)
    {
        USART3_puts("run1\r\n");
    }
}

void Task2(void)
{
    while(1)
    {
        USART3_puts("run2\r\n");
    }
}
