/*
 * Const.h
 *
 *  Created on: Mar 5, 2021
 *      Author: xxxl
 */

#ifndef CONST_H_
#define CONST_H_


#ifdef HSESystemClk
  #define SYSCLK 72  //外部晶振72M
#else
  #define SYSCLK 64  //内部时钟64M
#endif


#endif /* CONST_H_ */
