/*
 * System_Init.h
 *
 *  Created on: Mar 5, 2021
 *      Author: xxxl
 */

#ifndef SYSTEM_INIT_H_
#define SYSTEM_INIT_H_

void SystemInit(void);
void SystemClock_Config(void);
void AFIO_Config(void);
void NVIC_Config(void);
void delay_init(void);
void delay_us(u32 nus);
void delay_ms(u16 nms);

#endif /* SYSTEM_INIT_H_ */
