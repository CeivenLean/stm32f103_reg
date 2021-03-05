/*
 * TimeBase.c
 *
 *  Created on: 2021年3月1日
 *      Author: xxxl
 */

#include "Includes.h"


STim3  sTim3_;
pSTim3 Tim3 = &sTim3_;
STim4  sTim4_;
pSTim4 Tim4 = &sTim4_;

void TIM3_IRQHandler(void)
{
  if(TIM3->SR & 0x0001)//溢出中断
  {
    Tim3->t1ms ++;
    if(Tim3->t1ms ==2)
    {
      Tim3->t1ms = 0;Tim3->b1ms = true;
      Tim3->t5ms ++;
      if(Tim3->t5ms ==5)
      {
        Tim3->t5ms = 0;Tim3->b5ms = true;
        Tim3->t10ms ++;
        if(Tim3->t10ms ==2)
        {
          Tim3->t10ms = 0;Tim3->b10ms = true;
          Tim3->t20ms ++;
          Tim3->t50ms ++;
          if(Tim3->t20ms ==2){Tim3->t20ms = 0;Tim3->b20ms = true;}
          if(Tim3->t50ms ==5)
          {
            Tim3->t50ms = 0;Tim3->b50ms = true;
            Tim3->t100ms ++;
            if(Tim3->t100ms ==2)
            {
              Tim3->t100ms = 0;Tim3->b100ms = true;
              Tim3->t200ms ++;
              Tim3->t500ms ++;
              if(Tim3->t200ms ==2){Tim3->t200ms = 0;Tim3->b200ms = true;}
              if(Tim3->t500ms ==5){Tim3->t500ms = 0;Tim3->b500ms = true;}
            }
          }
        }
      }
    }
  }
  TIM3->SR &= ~(1<<0);//清除中断标志位
}

/*
 *
 */
void TIM4_IRQHandler(void)
{
  if(TIM4->SR & 0x0001)//溢出中断
  {
    Tim4->t5ms ++;
    if(Tim4->t5ms == 60)
    {
      Tim4->t5ms = 0;
      PB0_R;
      PB1_R;
      PB5_R;
    }
  }
  TIM4->SR &= ~(1<<0);//清除中断标志位
}

/*
 *
 */
void TIM3_Config(void)
{
  RCC->APB1ENR |= 1<<1;    //使用TIM3时钟

  /* Tout = ( (arr+1) * (psc+1) ) / Tclk
   * 则每 ((4+1) * (6399+1)) /64 = 500us 产生一次中断
   */
  TIM3->ARR = 4;    //设定计数器自动重装值
  TIM3->PSC = (SYSCLK*100 -1);    //预分频器6400,得到10Khz的计数时钟
  TIM3->DIER |= 1<<0;   //允许更新中断
  TIM3->CR1 |= 0x01;    //使能定时器3

  NVIC->ISER[TIM3_IRQn /32] |= (1<<(TIM3_IRQn %32));    //使能中断
  NVIC->IP[TIM3_IRQn] |= 0xC0; // 6,0
}

/*
 *
 */
void TIM4_Config(void)
{
  RCC->APB1ENR |= 1<<2;    //使用TIM4时钟
  /*
   * 每 ((49+1) * (6399+1)) /64 = 5000us, 5ms产生一次中断
   */
  TIM4->ARR = 49;    //设定计数器自动重装值
  TIM4->PSC = (SYSCLK*100 -1);    //预分频器6400,得到10Khz的计数时钟
  TIM4->DIER |= 1<<0;   //允许更新中断
  TIM4->CR1 |= 0x01;    //使能定时器3

  NVIC->ISER[TIM4_IRQn /32] |= (1<<(TIM4_IRQn %32));    //使能中断
  NVIC->IP[TIM4_IRQn] |= 0xD0; // 6,1
}
