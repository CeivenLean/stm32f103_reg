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

  u8 Tx2Count = 0;
    /* Loop forever */
	while(1)
	{
	  if(Rx->Success == true)
	  {
	    Rx->Success = false;
	    Tx2Count = 0;
	    while(Tx2Count < 10)
	    {
	      USART2->DR = Rx->Data[Tx2Count];
	      while((USART2->SR &0X40) == 0);//等待发送结束
	      Tx2Count ++;
	    }
	    Tx2Count = 0;
	  }
    if(Rx2->Success == true)
    {
      Rx2->Success = false;
      Tx2Count = 0;
      while(Tx2Count < 10)
      {
        USART1->DR = Rx2->Data[Tx2Count];
        while((USART1->SR &0X40) == 0);//等待发送结束
        Tx2Count ++;
      }
      Tx2Count = 0;
    }
	}
}
