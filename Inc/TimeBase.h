/*
 * TimeBase.h
 *
 *  Created on: 2021年3月1日
 *      Author: xxxl
 */

#ifndef INC_TIMEBASE_H_
#define INC_TIMEBASE_H_

typedef struct
{
  bool b1ms;
  bool b4ms;
  bool b5ms;
  bool b10ms;
  bool b20ms;
  bool b50ms;
  bool b100ms;
  bool b200ms;
  bool b500ms;
  u16 t1ms;
  u16 t4ms;
  u16 t5ms;
  u16 t10ms;
  u16 t20ms;
  u16 t50ms;
  u16 t100ms;
  u16 t200ms;
  u16 t500ms;
}STim3, *pSTim3;
extern STim3  sTim3_;
extern pSTim3 Tim3;

typedef struct
{
  bool b5ms;
  bool b10ms;
  bool b20ms;
  bool b50ms;
  bool b100ms;
  bool b125ms;
  bool b150ms;
  bool b200ms;
  bool b250ms;
  bool b300ms;
  bool b500ms;
  u16 t5ms;
  u16 t10ms;
  u16 t20ms;
  u16 t50ms;
  u16 t100ms;
  u16 t125ms;
  u16 t150ms;
  u16 t200ms;
  u16 t250ms;
  u16 t300ms;
  u16 t500ms;
}STim4, *pSTim4;
extern STim4  sTim4_;
extern pSTim4 Tim4;

void TIM1_PWM_Init(u16 arr,u16 psc);
void TIM1_Config(void);
void TIM3_Config(void);
void TIM4_Config(void);

#endif /* INC_TIMEBASE_H_ */
