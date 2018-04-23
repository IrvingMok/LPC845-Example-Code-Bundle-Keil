/*
 * SCT_Event_Counter.h
 *
 *  Created on: Apr 19, 2016
 *      Author:
 */

#ifndef SCT_EVENT_COUNTER_H_
#define SCT_EVENT_COUNTER_H_

#define OUT_PORT PORT0
#define OUT_BIT  Bit25
#define IN_PORT  P0_24

void setup_debug_uart(void);
void WKT_Config(void);
void MRT_Config(void);

#endif /* SCT_EVENT_COUNTER_H_ */
