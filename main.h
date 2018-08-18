#ifndef MAIN_H_
#define MAIN_H_

#include "./Core/arm_cortex_m4.h"
#include "./Core/arm_cortex_nvic.h"

#include "./Boot/peripherals.h"
#include "./Boot/print.h"

void USART3_IRQHandler(void);

#endif