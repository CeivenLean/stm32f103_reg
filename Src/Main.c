/**
 ******************************************************************************
 * @file           : main.c
 * @author         : xxxl
 * @brief          : Main program body
 ******************************************************************************
 */
/* Includes ------------------------------------------------------------------*/
#include "includes.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u16 pwmVal = 0;   //PWM占空比
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
int main(void)
{
  SystemInit();
  LED_Init();
  /* Loop */
  //重要：全部开启SWJ(JTAG-DP + SW-DP)
  AFIO->MAPR &= (u32)~(7<<24);
	while(1)
	{
	  UsartService();
    while(pwmVal < 900)
    {
      pwmVal +=1;
      TIM1->CCR1 = pwmVal;
      TIM1->CCR2 = pwmVal;
      TIM1->CCR3 = pwmVal;
      delay_ms(1);
    }
    while(pwmVal >0)
    {
      pwmVal -=1;
      TIM1->CCR1 = pwmVal;
      TIM1->CCR2 = pwmVal;
      TIM1->CCR3 = pwmVal;
      delay_ms(1);
    }
	}
}
