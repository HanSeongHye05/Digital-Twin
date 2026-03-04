#ifndef INC_CON_MOTOR_H_
#define INC_CON_MOTOR_H_

#include "main.h"

/* 속도 범위: 0 ~ ARR 값 */
void Conveyor_Init(void);
void Conveyor_Start_Slow(void);
void Conveyor_Stop(void);
void Conveyor_SetDuty(uint16_t duty);

#endif /* INC_CON_MOTOR_H_ */
