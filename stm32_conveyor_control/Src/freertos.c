/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "con_motor.h"     //컨베이어 벨트 모터
#include "btn_on_off.h"    //전원 버튼
#include "btn_speed.h"     //속도 버튼


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

//전역 명령어 설정
#define CMD_POWER_TOGGLE 1
#define CMD_SPEED_UP 2
#define CMD_SPEED_DOWN 3

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for ButtonTask */
osThreadId_t ButtonTaskHandle;
const osThreadAttr_t ButtonTask_attributes = {
  .name = "ButtonTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for MotorTask */
osThreadId_t MotorTaskHandle;
const osThreadAttr_t MotorTask_attributes = {
  .name = "MotorTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for motorQueue */
osMessageQueueId_t motorQueueHandle;
const osMessageQueueAttr_t motorQueue_attributes = {
  .name = "motorQueue"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartTask02(void *argument);
void StartTask03(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of motorQueue */
  motorQueueHandle = osMessageQueueNew (16, sizeof(uint8_t), &motorQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of ButtonTask */
  ButtonTaskHandle = osThreadNew(StartTask02, NULL, &ButtonTask_attributes);

  /* creation of MotorTask */
  MotorTaskHandle = osThreadNew(StartTask03, NULL, &MotorTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)     //시스템 기본 작업용
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTask02 */
/**
* @brief Function implementing the ButtonTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask02 */
void StartTask02(void *argument)
{
/* USER CODE BEGIN StartTask02 */
	  uint8_t msg;
 /* USER CODE BEGIN StartTask02 */
	  for(;;)
	  {
	      // 버튼 함수가 눌림 감지 시 1(True)을 반환한다고 가정
	      if (Check_Power_Button() == 1) {
	          msg = CMD_POWER_TOGGLE;
	          osMessageQueuePut(motorQueueHandle, &msg, 0, 0);
	      }

	      int speedBtn = Check_Speed_Buttons();
	      if (speedBtn == 1) { // Up
	          msg = CMD_SPEED_UP;
	          osMessageQueuePut(motorQueueHandle, &msg, 0, 0);
	      } else if (speedBtn == 2) { // Down
	          msg = CMD_SPEED_DOWN;
	          osMessageQueuePut(motorQueueHandle, &msg, 0, 0);
	      }

	      osDelay(20);
	  }
  /* USER CODE END StartTask02 */
}

/* USER CODE BEGIN Header_StartTask03 */
/**
* @brief Function implementing the MotorTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask03 */
void StartTask03(void *argument)
{
  uint8_t receiveMsg;
  uint16_t target_speed = 0;
  uint16_t actual_speed = 0;

  // [중요] 시작 시 모터 물리적 정지 강제
  Conveyor_Stop();
  Conveyor_SetDuty(0);

  for(;;)
  {
    // 버튼 눌림 메시지가 오면 목표 속도 갱신
    if(osMessageQueueGet(motorQueueHandle, &receiveMsg, NULL, 0) == osOK)
    {
      target_speed = Get_Current_Speed();
    }

    if (Get_Power_Status() == 1) // 전원이 켜진 경우에만
    {
      // 전원을 막 켰을 때 target_speed가 0이면 초기값(100) 로드
      if (target_speed == 0) target_speed = Get_Current_Speed();

      // 실제 속도를 목표 속도까지 부드럽게 가속/감속 (슬로프 제어)
      if (actual_speed < target_speed) actual_speed++;
      else if (actual_speed > target_speed) actual_speed--;

      Conveyor_SetDuty(actual_speed);
    }
    else // 전원이 꺼진 경우
    {
      actual_speed = 0;
      target_speed = 0;
      Conveyor_SetDuty(0);
      Conveyor_Stop();
    }

    // 가속의 부드러움을 결정 (30ms 주기가 가장 실감납니다)
    osDelay(30);
  }
  /* USER CODE END StartTask03 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

