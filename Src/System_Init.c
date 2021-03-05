/*
 * System_Init.c
 *
 *  Created on: Mar 5, 2021
 *      Author: xxxl
 */

#include "includes.h"


void SystemInit(void)
{
  SystemClock_Config();
  delay_init();
  AFIO_Config();
  NVIC_Config();
  TIM3_Config();
  TIM4_Config();
  USART1_Config();
  USART2_Config();
//  uart_init1(64,9600);
//  uart_init2(32,9600);

}

void SystemClock_Config(void)
{
#ifdef HSESystemClk
  unsigned char temp=0;
  //MYRCC_DeInit();     //复位并配置向量表
  RCC->CR|=0x00010000;  //外部高速时钟使能HSEON
  while(!(RCC->CR>>17));//等待外部时钟就绪
  RCC->CFGR=0X00000400; //APB1=DIV2;APB2=DIV1;AHB=DIV1;
  RCC->CFGR|= 7<<18;   //设置PLL值 2~16
  RCC->CFGR|=1<<16;   //PLLSRC ON
  FLASH->ACR|=0x32;   //FLASH 2个延时周期
  RCC->CR|=0x01000000;  //PLLON
  while(!(RCC->CR>>25));//等待PLL锁定
  RCC->CFGR|=0x00000002;//PLL作为系统时钟
  while(temp!=0x02)     //等待PLL作为系统时钟设置成功
  {
    temp=RCC->CFGR>>2;
    temp&=0x03;
  }
#else
  unsigned char temp=0;
  //MYRCC_DeInit();     //复位并配置向量表
  RCC->CR|=0x00000001;  //内部时钟使能HSION
  while(!(RCC->CR>>1));//等待内部时钟就绪
  RCC->CFGR=0X00000400; //APB1=DIV2;APB2=DIV1;AHB=DIV1;
  RCC->CFGR|= 15<<18;   //设置PLL值 2~16
  FLASH->ACR|=0x32;   //FLASH 2个延时周期
  RCC->CR|=0x01000000;  //PLLON
  while(!(RCC->CR>>25));//等待PLL锁定
  RCC->CFGR|=0x00000002;//PLL作为系统时钟
  while(temp!=0x02)     //等待PLL作为系统时钟设置成功
  {
    temp=RCC->CFGR>>2;
    temp&=0x03;
  }
#endif
}

/*
 *
 */
void AFIO_Config(void)
{
  AFIO->MAPR |= 2<<24;
}

/*
 *
 */
void NVIC_Config(void)
{
  /* SCB->AIRCR 在设置时需将高8位填为0x05FA，第8-10字节为分组
   * 0 111 抢占：0bit，响应：4bit
   * 1 110 抢占：1bit，响应：3bit
   * 2 101 抢占：2bit，响应：2bit
   * 3 100 抢占：3bit，响应：1bit
   * 4 011 抢占：4bit，响应：0bit
   * NVIC->ISER为中断使能，
   * NVIC->IP为中断优先级,低四位保留，高四位配置抢占级与响应级
   */
  SCB->AIRCR &= 0x0000F8FF; //清空原分组
  SCB->AIRCR |= 0x05FA0400;  //设置分组3 8-10:100

//  NVIC->ISER[USART1_IRQn /32] |= (1<<(USART1_IRQn %32));//使能串口1中断位
//  NVIC->IP[USART1_IRQn] |= 0xE0; //串口1抢占级7，响应级0;
}

static u8  fac_us=0;              //us延时倍乘数
static u16 fac_ms=0;              //ms延时倍乘数

//初始化延迟函数
//当使用OS的时候,此函数会初始化OS的时钟节拍
//SYSTICK的时钟固定为HCLK时钟的1/8
//SYSCLK:系统时钟
void delay_init(void)
{
  SysTick->CTRL&=~(1<<2);         //SYSTICK使用外部时钟源
  fac_us=SysClk/8;            //
  fac_ms=(u16)fac_us*1000;        //代表每个ms需要的systick时钟数
}

//延时nus
//nus为要延时的us数.
void delay_us(u32 nus)
{
  u32 temp;
  SysTick->LOAD=nus*fac_us;         //时间加载
  SysTick->VAL=0x00;                //清空计数器
  SysTick->CTRL=0x01 ;              //开始倒数
  do
  {
    temp=SysTick->CTRL;
  }while((temp&0x01)&&!(temp&(1<<16))); //等待时间到达
  SysTick->CTRL=0x00;             //关闭计数器
  SysTick->VAL =0X00;               //清空计数器
}

//延时nms
//注意nms的范围
//SysTick->LOAD为24位寄存器,所以,最大延时为:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK单位为Hz,nms单位为ms
//对72M条件下,nms<=1864
void delay_ms(u16 nms)
{
  u32 temp;
  SysTick->LOAD=(u32)nms*fac_ms;      //时间加载(SysTick->LOAD为24bit)
  SysTick->VAL =0x00;                 //清空计数器
  SysTick->CTRL=0x01 ;                //开始倒数
  do
  {
    temp=SysTick->CTRL;
  }while((temp&0x01)&&!(temp&(1<<16))); //等待时间到达
  SysTick->CTRL=0x00;             //关闭计数器
  SysTick->VAL =0X00;               //清空计数器
}
