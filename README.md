
# 🏭 STM32 Conveyor Belt Control System (RTOS)

---

## 📌 프로젝트 개요

* OpenCV 기반 색상 인식 시스템과 연동된 컨베이어 벨트 제어
* STM32 기반 DC Motor 제어 구현
* FreeRTOS 기반 Task 구조를 활용한 제어 시스템 설계

👉 **입력에 따라 동작이 결정되는 RTOS 기반 모터 제어 시스템**

---

## ⚙️ 시스템 전체 흐름 (팀 프로젝트)

```plaintext
Vision (OpenCV)
        ↓
STM32 (Motor Control)
        ↓
Conveyor Belt 동작
        ↓
Robot Arm 분류
```

---

## 🔧 내가 구현한 부분

* 컨베이어 벨트 모터 제어 로직 구현
* PWM(Timer) 기반 모터 속도 제어
* 버튼 입력 기반 ON/OFF 및 속도 조절 기능 구현
* FreeRTOS 기반 Task 구조 설계 및 적용

---

## 🔄 제어 흐름 (코드 기반)

```plaintext
버튼 입력 (ON/OFF, 속도 증가/감소)
        ↓
상태 판단 (전원 상태 / 속도 변경)
        ↓
PWM Duty 값 변경
        ↓
Timer PWM 출력
        ↓
모터 속도 제어
```

---

## 🧠 제어 구조 특징

* 입력 / 제어 / 출력 구조 분리
* FreeRTOS Task 기반 주기적 제어 수행
* 기능별 파일 분리를 통한 구조 설계

👉 **RTOS 기반 안정적인 모터 제어 구조 구현**

---

## 🚨 문제 해결 경험

* 버튼 입력 시 연속 입력 발생
  → 소프트웨어 디바운싱 적용

* 입력 신호 불안정
  → 풀다운 저항 적용으로 신호 안정화

* UART 출력 불안정
  → 출력 주기 조정 및 버퍼 처리

---

## 📈 개선 방향

* 인터럽트 기반 입력 처리 적용
* RTOS Task 우선순위 최적화
* 다양한 모터 환경 대응 구조 확장

---

## 🧰 기술 스택

* STM32 HAL Driver
* FreeRTOS
* Timer PWM
* GPIO / UART

---

## 📁 코드 구조

```plaintext
stm32_conveyor_control/
├── Inc/
│   ├── main.h
│   ├── con_motor.h
│   ├── btn_on_off.h
│   ├── btn_speed.h
│
├── Src/
│   ├── main.c
│   ├── freertos.c
│   ├── con_motor.c
│   ├── btn_on_off.c
│   ├── btn_speed.c
```

---

### 🔧 주요 파일 역할

* `main.c`
  → 전체 시스템 흐름 및 초기화

* `freertos.c`
  → Task 생성 및 스케줄링

* `con_motor.c`
  → PWM 기반 컨베이어 모터 제어

* `btn_on_off.c`
  → 전원 ON/OFF 버튼 입력 처리

* `btn_speed.c`
  → 속도 증가 / 감소 제어

---

## 🎯 핵심 포인트

👉 **FreeRTOS 기반 구조에서 버튼 입력에 따라 동작하는 모터 제어 시스템 구현**
