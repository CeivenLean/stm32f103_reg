/*
 * USART.c
 *
 *  Created on: Feb 27, 2021
 *      Author: xxxl
 */

#include "Includes.h"


STx  sTx_;
pSTx Tx = &sTx_;
SRx  sRx_;
pSRx Rx = &sRx_;

STx2  sTx2_;
pSTx2 Tx2 = &sTx2_;
SRx2  sRx2_;
pSRx2 Rx2 = &sRx2_;


void USART1_IRQHandler(void)
{
  Rx->Number = 10;
  if(USART1->SR &(1<<5))
  {
    Rx->Data[Rx->Count] = USART1->DR;
    if(Rx->Data[0] == 0x95 && Rx->Count < Rx->Number)
    {
      Rx->Count ++;
    }
    else Rx->Count = 0;
    if(Rx->Count >= Rx->Number)
    {
      Rx->Success = true;
      Rx->Count = 0;
    }
  }
}

/*
 *
 */
void USART2_IRQHandler(void)
{
  Rx2->Number = 10;
  if(USART2->SR &(1<<5))
  {
    Rx2->Data[Rx2->Count] = USART2->DR;
    if(Rx2->Data[0] == 0x95 && Rx2->Count < Rx2->Number)
    {
      Rx2->Count ++;
    }
    else Rx2->Count = 0;
    if(Rx2->Count >= Rx2->Number)
    {
      Rx2->Success = true;
      Rx2->Count = 0;
    }
  }
}

void USART1_Send()
{
  Tx->Number = 10;
  Tx->Count = 0;
  Rx->Data[8] = 0x0D;
  Rx->Data[9] = 0x0A;
  while(Tx->Count < Tx->Number)
  {
    USART1->DR = Rx->Data[Tx->Count];
    while((USART1->SR &0X40) == 0);//等待发送结束
    Tx->Count ++;
  }
  Tx->Count = 0;
}

void USART2_Send(u8 *buff, u8 len)
{
  u8 count = 0;
  while(count < len)
  {
    USART2->DR = buff[count];
    while((USART2->SR &0X40) == 0);//等待发送结束
    count ++;
  }
}

/* USART1配置 */
void USART1_Config(void)
{
  /*
   * 波特率9600
   * UE、PEIE、RXNEIE、TE、RE使能
   */
  //RCC->APB2RSTR |= 1<<14;    //复位串口1
  //RCC->APB2RSTR &= ~(1<<14);//停止复位
  RCC->APB2ENR |= 1<<14;    //使能串口1时钟

  RCC->APB2ENR |= 1<<2;       //使能 PORTA 口时钟
  GPIOA->CRH &= 0xFFFFF00F;    // PA9:USART1_TX,  PA10:USART1_RX
  GPIOA->CRH |= 0x000008B0;    // PA9:复用推挽输出(50M)    PA10:下拉输入

  USART1->BRR = 0x1A0A;
  USART1->CR1 |= 0x212C;
  NVIC->ISER[USART1_IRQn /32] |= (1<<(USART1_IRQn %32));//使能串口1中断位
  NVIC->IP[USART1_IRQn] |= 0xE0; //串口1抢占级7，响应级0;
}

/* USART2配置 */
void USART2_Config(void)
{
  /*
   * 波特率9600
   * UE、PEIE、RXNEIE、TE、RE使能
   */
  RCC->APB1RSTR |= 1<<17;   //复位串口2
  RCC->APB1RSTR &= ~(1<<17);//停止复位
  RCC->APB1ENR |= 1<<17;   //使能串口2时钟

  RCC->APB2ENR |= 1<<2;       //使能 PORTA 口时钟
  GPIOA->CRL &= 0xFFFF00FF;    // PA2:USART1_TX,  PA3:USART1_RX
  GPIOA->CRL |= 0x00008B00;    // PA2:复用推挽输出(50M)    PA3:下拉输入

  USART2->BRR = 0x0D05;     //APB1 36M的波特率9600为 0D05
  USART2->CR1 |= 0x212C;
  NVIC->ISER[USART2_IRQn /32] |= (1<<(USART2_IRQn %32));//使能串口2中断位
  NVIC->IP[USART2_IRQn] |= 0xF0; //串口2抢占级7，响应级1;
}


void uart_init1(u32 pclk2,u32 bound)
{
  float temp;
  u16 mantissa;
  u16 fraction;
  temp=(float)(pclk2*1000000)/(bound*16);//得到USARTDIV
  mantissa=temp;         //得到整数部分
  fraction=(temp-mantissa)*16; //得到小数部分
    mantissa<<=4;
  mantissa+=fraction;
  RCC->APB2ENR|=1<<2;   //使能PORTA口时钟
  RCC->APB2ENR|=1<<14;  //使能串口时钟
  GPIOA->CRH&=0XFFFFF00F;//IO状态设置
  GPIOA->CRH|=0X000008B0;//IO状态设置
  RCC->APB2RSTR|=1<<14;   //复位串口1
  RCC->APB2RSTR&=~(1<<14);//停止复位
  //波特率设置
  USART1->BRR=mantissa; // 波特率设置
  USART1->CR1|=0X210C;  //1位停止,无校验位.
  //使能接收中断
  USART1->CR1|=1<<5;    //接收缓冲区非空中断使能
  NVIC->ISER[USART1_IRQn /32] |= (1<<(USART1_IRQn %32));//使能串口1中断位
  NVIC->IP[USART1_IRQn] |= 0xE0; //串口1抢占级7，响应级0;

}

void uart_init2(u32 pclk2,u32 bound)
{
  float temp;
  u16 mantissa;
  u16 fraction;
  temp=(float)(pclk2*1000000)/(bound*16);//得到USARTDIV
  mantissa=temp;         //得到整数部分
  fraction=(temp-mantissa)*16; //得到小数部分
    mantissa<<=4;
  mantissa+=fraction;
  RCC->APB2ENR|=1<<2;   //使能PORTA口时钟
  RCC->APB1ENR|=1<<17;  //使能串口时钟
  GPIOA->CRL &= 0xFFFF00FF;    // PA2:USART1_TX,  PA3:USART1_RX
  GPIOA->CRL |= 0x00008B00;    // PA2:复用推挽输出(50M)    PA3:下拉输入
  RCC->APB1RSTR|=1<<17;   //复位串口1
  RCC->APB1RSTR&=~(1<<17);//停止复位
  //波特率设置
  USART2->BRR=mantissa; // 波特率设置
  USART2->CR1|=0X200C;  //1位停止,无校验位.
  //使能接收中断
  USART2->CR1|=1<<5;    //接收缓冲区非空中断使能
  NVIC->ISER[USART2_IRQn /32] |= (1<<(USART2_IRQn %32));//使能串口2中断位
  NVIC->IP[USART2_IRQn] |= 0xF0; //串口2抢占级7，响应级1;
}
