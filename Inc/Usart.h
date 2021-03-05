/*
 * Usart.h
 *
 *  Created on: Feb 27, 2021
 *      Author: xxxl
 */

#ifndef INC_USART_H_
#define INC_USART_H_

typedef struct
{
  bool Success;
  u16 Number;
  u8  Data[10];
  u16 Count;

}STx, *pSTx;

typedef struct
{
  bool Success;
  u16 Number;
  u8  Data[10];
  u16 Count;

}SRx, *pSRx;

extern STx  sTx_;
extern pSTx Tx;

extern SRx  sRx_;
extern pSRx Rx;

void USART1_Config(void);
void USART2_Config(void);
void USART1_Send(void);
void USART2_Send(u8 *buff, u8 len);
void uart_init1(u32 pclk2,u32 bound);
void uart_init2(u32 pclk2,u32 bound);

#endif /* INC_USART_H_ */
