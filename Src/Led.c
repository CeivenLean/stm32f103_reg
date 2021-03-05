/*
 * Led.c
 *
 *  Created on: Mar 5, 2021
 *      Author: xxxl
 */

#include "includes.h"

void LED_Init(void)
{
  RCC->APB2ENR|=1<<3;    //使能PORTB时钟

  GPIOB->CRL&=0XFF0FFF00;
  GPIOB->CRL|=0X00300033;//PB0,1,5 推挽输出
  GPIOB->ODR |= 1<<0;
  GPIOB->ODR |= 1<<1;
  GPIOB->ODR |= 1<<5;      //输出高

}
