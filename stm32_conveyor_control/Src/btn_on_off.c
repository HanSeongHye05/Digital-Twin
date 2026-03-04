#include "btn_on_off.h"
#include "main.h"
#include <stdio.h>

 //[변수 초기화]
 //static: 이 파일 안에서만 사용하는 전역 변수
 // power_on = 0: 런(Run) 했을 때 모터가 즉시 도는 것을 막기 위해 '꺼짐' 상태로 시작함

static uint8_t power_on = 0;

 // @brief 전원 버튼(PF3) 상태를 체크하여 ON/OFF를 전환하는 함수
 // @return int: 버튼이 눌려 상태가 변했다면 1, 아니면 0 반환

int Check_Power_Button(void) {
    // 1. PF3 버튼이 눌렸는지 확인 (Pull-up 회로이므로 눌리면 RESET/0 상태가 됨)
    if (HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_3) == GPIO_PIN_RESET) {

        // 2. 채터링(떨림) 방지: 40ms 대기 후에도 계속 눌려 있는지 확인
        HAL_Delay(40);

        if (HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_3) == GPIO_PIN_RESET) {
            // 3. 현재 상태 반전 (0이면 1로, 1이면 0으로)
            power_on = !power_on;

            // 4. 상태 변화를 시리얼 터미널(UART)로 출력하여 사용자에게 알림
            if (power_on) {
                printf("\r\n[SYSTEM] POWER: ON - Conveyor Ready\r\n");
            } else {
                printf("\r\n[SYSTEM] POWER: OFF - Conveyor Stop\r\n");
            }

            // 5. 버튼을 뗄 때까지 대기 (한 번 눌렀는데 여러 번 토글되는 것 방지)
            while (HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_3) == GPIO_PIN_RESET);

            return 1; // 상태 변경 성공
        }
    }
    return 0; // 버튼 안 눌림
}

//
// @brief 다른 소스 파일(btn_speed.c, freertos.c)에서 현재 전원 상태를 알 수 있게 해주는 함수
// @return uint8_t: 현재 전원 상태 (0: 꺼짐, 1: 켜짐)

uint8_t Get_Power_Status(void) {
    return power_on;
}

