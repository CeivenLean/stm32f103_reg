/*
 * TimeBase.c
 *
 *  Created on: 2021年3月1日
 *      Author: xxxl
 */
/* Includes ------------------------------------------------------------------*/
#include "Includes.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
STim3  sTim3_;
pSTim3 Tim3 = &sTim3_;
STim4  sTim4_;
pSTim4 Tim4 = &sTim4_;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void TIM1_UP_IRQHandler(void)
{
  if(TIM1->SR & 0x0001)//溢出中断
  {
    ;
  }
  TIM1->SR = 0;
}


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
      if((PB0_O!=0) && (PB1_O!=0) && (PB5_O!=0))    //初始状态
      {
        PB0_L;    //PB0
      }
      else if((PB0_O==0) && (PB1_O!=0) && (PB5_O!=0))
      {
        PB1_L;PB0_H;    //PB1
      }
      else if((PB0_O!=0) && (PB1_O==0) && (PB5_O!=0))
      {
        PB5_L;PB1_H;    //PB5
      }
      else if((PB0_O!=0) && (PB1_O!=0) && (PB5_O==0))
      {
        PB0_L;PB5_H;    //PB0
      }
    }
  }
  TIM4->SR &= ~(1<<0);//清除中断标志位
}

/*
 *
 */
void TIM1_Config(void)
{
  RCC->APB2ENR|=1<<11;   //TIM1时钟使能
  RCC->APB2ENR|=1<<2;     //使能PORTA时钟
  RCC->APB2ENR|=1<<3;     //使能PORTB时钟

  GPIOA->CRH&=0XFFFFF000;   //PA8,9,10清除之前的设置
  GPIOA->CRH|=0X00000BBB;   //PA8,9,10复用功能输出
  GPIOB->CRH&=0X000FFFFF;  //PB13,14,15清除之前的设置
  GPIOB->CRH|=0XBBB00000;  //PB13,14,15复用功能输出

  TIM1->CR1 |= 1<<8;     //分频2
  TIM1->CR1 |= 1<<5;     //中央对齐模式1

  TIM1->DIER |= 1;

  TIM1->CR2 |= 1<<13;
  TIM1->CR2 |= 1<<11;
  TIM1->CR2 |= 1<<9;
  TIM1->CR2 |= 7<<4;

  TIM1->ARR = 900;           //设定计数器自动重装值     ①1
  TIM1->PSC = 0;           //预分频器设置             ②2
  TIM1->CR1 |= 0x80;       //ARPE使能,开始所有输出通道,默认向上计数   ⑦7
  TIM1->RCR = 1;
  TIM1->EGR = 1;

  TIM1->CCER |= 1<<2;      //输入/捕获1互补输出使能 开启－ OC1N信号输出到PB13
  TIM1->CCER |= 1<<3;      //输入/捕获1互补输出极性 OC1N低电平有效
  TIM1->CCER |= 1<<6;      //输入/捕获2互补输出使能
  TIM1->CCER |= 1<<7;      //输入/捕获2互补输出极性
  TIM1->CCER |= 1<<10;      //输入/捕获3互补输出使能
  TIM1->CCER |= 1<<11;      //输入/捕获3互补输出极性

  TIM1->CCER |= 1;     //输入/捕获1输出使能
  TIM1->CCER |= 1<<4;  //输入/捕获2输出使能
  TIM1->CCER |= 1<<8;  //输入/捕获3输出使能
  TIM1->CCER |= 1<<12;     //输入/捕获4输出使能

  TIM1->CCMR1 |= 6<<4;     //CH1 PWM1模式           ④4
  TIM1->CCMR1 |= 1<<3;     //CH1预装载使能
  TIM1->CCMR1 |= 6<<12;    //CH2 PWM1模式
  TIM1->CCMR1 |= 1<<11;    //CH2预装载使能
  TIM1->CCMR2 |= 6<<4;     //CH3 PWM1模式
  TIM1->CCMR2 |= 1<<3;     //CH3预装载使能
  ////    TIM1->BDTR |= 0x19;     //DTG[7:5] = 000, 死区持续时间DT=DTG[7:0] × Tdtg，Tdtg = TDTS ，死区时间= 0x19 * (1/32)= 0.781 即781us
  ////    TIM1->BDTR |= 1<<8;     //锁定级别 1
  ////    TIM1->BDTR |= 1<<10;    //运行模式下“空闲状态”选择
  ////    TIM1->BDTR |= 1<<11;    //运行模式下“关闭状态”选择
  ////    TIM1->BDTR |= 1<<13;    //刹车输入极性:刹车输入高电平有效
  TIM1->BDTR = 0x2D19;       //同上一起设置
  TIM1->BDTR |= 1<<15;     //MOE 主输出使能



  TIM1->DIER |= 0x01;
  TIM1->SR = 0;
  TIM1->CR1 |= 0x01;       //使能计数器

  NVIC->ISER[TIM1_UP_IRQn /32] |= (1<<(TIM1_UP_IRQn %32));//使能TIM1中断位
  NVIC->IP[TIM1_UP_IRQn] |= 0xA0; //TIM1抢占级1，响应级0;

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
