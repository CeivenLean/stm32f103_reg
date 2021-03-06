/**
 ******************************************************************************
 * @file           : main.c
 * @author         : xxxl
 * @brief          : Main program body
 ******************************************************************************
 */

#include "includes.h"


int main(void)
{
  SystemInit();
  LED_Init();
  uint16_t pwmVal=0;   //PWM占空比
    /* Loop forever */
	while(1)
	{
	  //UsartService();
    if(pwmVal< 1000)
    {
      pwmVal++;
      TIM1->CCR1 = pwmVal;
      TIM1->CCR2 = pwmVal;
      TIM1->CCR3 = pwmVal;
    }
    else pwmVal = 0;
    delay_ms(1);
	}
}
