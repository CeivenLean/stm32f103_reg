/*
 * Const.h
 *
 *  Created on: Mar 5, 2021
 *      Author: xxxl
 */

#ifndef CONST_H_
#define CONST_H_


#ifdef HSESystemClk
  #define SysClk 72  //外部晶振72M
#else
  #define SysClk 64  //内部时钟64M
#endif


#endif /* CONST_H_ */
