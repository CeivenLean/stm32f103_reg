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

  u8 Buff[10] = {0x95,0x05,0x02,0x03,0x04,0x05,0x06,0x07,0x0D,0x0A};
  u8 count = 0;

  while(count<10)
  {
    while((USART1->SR &0X40) == 0);//等待发送结束
    USART1->DR = Buff[count];
    count ++;
  }
    /* Loop forever */
	while(1)
	{
	  if(Tim3->b100ms == true)
	  {
	    Tim3->b100ms = false;

	  }

	}
}
