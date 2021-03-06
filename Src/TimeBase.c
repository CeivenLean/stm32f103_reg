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

void TIM1_PWM_Init(u16 arr,u16 psc)
{
  RCC->APB2ENR |= 1<<11;    //使用TIM1时钟
  RCC->APB2ENR |= 1;
  RCC->APB2ENR |= 1<<6;    //使用GPIOE时钟
  AFIO->MAPR |= 3<<6;
  GPIOE->CRH &= 0xFF000000;    //PE8-13
  GPIOE->CRH |= 0x00BBBBBB;    //


    TIM1->ARR=arr;           //设定计数器自动重装值     ①1
    TIM1->PSC=psc;           //预分频器设置             ②2

    TIM1->CCER|=1<<0;      //TIM1CH1 输出使能,高电平有效      ③3
    TIM1->CCER|=1<<4;      //TIM1CH2 输出使能
    TIM1->CCER|=1<<8;      //TIM1CH3 输出使能
    TIM1->CCER|=1<<12;      //TIM1CH4 输出使能
    TIM1->CCER|=1<<2;      //TIM1CH1N 互补输出使能
    TIM1->CCER|=1<<6;      //TIM1CH2N 互补输出使能
    TIM1->CCER|=1<<10;      //TIM1CH3N 互补输出使能

    TIM1->CCMR1|=7<<4;     //CH1 PWM2模式           ④4
    TIM1->CCMR1|=1<<3;     //CH1预装载使能
    TIM1->CCMR1|=7<<12;    //CH2 PWM2模式
    TIM1->CCMR1|=1<<11;    //CH2预装载使能
    TIM1->CCMR2|=7<<4;     //CH3 PWM2模式
    TIM1->CCMR2|=1<<3;     //CH3预装载使能

    TIM1->CCMR2|=7<<12;    //CH4 PWM2模式
    TIM1->CCMR2|=1<<11;    //CH4预装载使能

        TIM1->BDTR|=0x14;       //死区时间设置          ⑤5
    TIM1->BDTR|=1<<15;     //MOE 主输出使能        ⑥6
    TIM1->CR1 |= 0x80;       //ARPE使能,开始所有输出通道,默认向上计数   ⑦7
    TIM1->CR1 |= 0x01;       //使能计数器
}

/*
 *
 */
void TIM1_Config(void)
{
  RCC->APB2ENR |= 1<<11;    //使用TIM1时钟
  RCC->APB2ENR |= 1<<6;    //使用GPIOE时钟
  GPIOE->CRH &= 0xFF000000;    //PE8-13
  GPIOE->CRH |= 0x00BBBBBB;    //

  TIM1->CR1 |= 1<<7;      //允许自动重装载预装载

  TIM1->ARR = 999;        //计数值
  TIM1->PSC = 0;

  TIM1->CCMR1 |= 6<<4;     //PWM1模式
  TIM1->CCER |= 8;
  TIM1->CCMR1 |= 8;        //预装载使能

  TIM1->CCMR1 |= 6<<12;
  TIM1->CCER |= 80;

  TIM1->CCMR1 |= 8<<8;

  TIM1->CCMR2 |= 6<<4;
  TIM1->CCER |= 800;
  TIM1->CCMR2 |= 8;

  TIM1->BDTR |= 1<<13;

  TIM1->CCER |= 1;
  TIM1->BDTR |= 1<<15;

  TIM1->CR1 |= 1;

  TIM1->CCER |= 1;
  TIM1->CCER |= 1<<4;
  TIM1->CCER |= 1<<8;

  TIM1->CCER |= 1<<2;
  TIM1->CCER |= 1<<6;
  TIM1->CCER |= 1<<10;

//  TIM1->CR1 |= 0<<4;    //向上计数

 // TIM1->CR1 |= 0<<5;        //CMS为边沿对齐模式

 // TIM1->CR1 |= 0<<8;    //时钟分频因子0
 // TIM1->RCR = 0;           //重复计数0





//  TIM1->EGR |= 1;

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
