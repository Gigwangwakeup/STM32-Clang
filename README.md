# STM32-Clang

**NUCLEO-F411RE** 보드와 **STM32 HAL**로 진행한 임베디드 C 실습 모음입니다.
GPIO 제어부터 시작해서 타이머 인터럽트, I2C(RTC·LCD), 센서 드라이버, 스텝모터, PWM, 이벤트 큐, ADC와 DMA를 단계별로 다루고, 마지막에는 팀 프로젝트로 UART 프로토콜 엘리베이터와 FreeRTOS RC카를 만들었습니다.

## 개발 환경

| 항목 | 내용 |
|---|---|
| 보드 | NUCLEO-F411RE (STM32F411, Cortex-M4) |
| 시스템 클럭 | 100 MHz (HSE + PLL) |
| 개발 도구 | STM32CubeIDE / STM32CubeMX |
| 라이브러리 | STM32 HAL, FreeRTOS (12번, CMSIS-RTOS v1) |
| 언어 | C, Python (11번 검증 GUI) |

> 각 폴더에는 직접 작성한 소스 파일(`main.c`, `ap/`, `driver/` 등)만 들어 있습니다.
> 빌드하려면 CubeMX로 같은 핀/주변장치 설정의 프로젝트를 만든 뒤 소스를 넣어야 합니다.

## 프로젝트 목록

각 프로젝트의 동작, 핀 구성, 구현 내용은 폴더 안의 README에 정리되어 있습니다.

| # | 프로젝트 | 내용 | 주요 기술 |
|---|---|---|---|
| 01 | [LedButton](01.%20LedButton) | 버튼 3개로 LED 켜기 / 끄기 / 깜빡이기 | GPIO 입출력 |
| 02 | [fnd_upcount](02.%20fnd_upcount) | 4자리 FND에 0000~9999 카운트 | FND 다이나믹 구동 |
| 03 | [upcount_btn](03.%20upcount_btn) | 버튼으로 Run / Stop / Clear 하는 카운터 | 상태 머신, 비트 시프트 디바운싱 |
| 04 | [watch_stopwatch](04.%20watch_stopwatch) | FND 시계 + 스톱워치 (모드 전환) | 타이머 인터럽트, ap/driver 계층 구조 |
| 05 | [LCD_RTC_I2C](05.%20LCD_RTC_I2C) | RTC 시계 + 스톱워치를 LCD와 FND에 표시 | I2C, DS3231 RTC, I2C LCD |
| 06 | [analogClock_motor](06.%20analogClock_motor) | 스텝모터 3개로 만든 아날로그 시계 | 스텝모터, 스텝 오차 보정 |
| 07 | [dist_temp_humid_sensor](07.dist_temp_humid_sensor) | 거리·온습도를 LCD에 표시 | HC-SR04, DHT11, EXTI, DWT us 딜레이 |
| 08 | [PWM](08.PWM) | 버튼으로 PWM 주파수·듀티비 조절 | 타이머 PWM, ARR / CCR |
| 09 | [smartWatch](09.smartWatch) | 달력·온습도·거리·스톱워치를 버튼으로 전환하는 스마트워치 | 버튼 이벤트 큐(링 버퍼), 상태 머신, 디버깅 기록 |
| 10 | [joystick_count](10.joystick_count) | 조이스틱 기울기에 따라 X·Y 카운트를 올리고 내림 | ADC Scan + DMA, I2C LCD |
| 11 | [elevator_teamproject](11.elevator_teamproject) | **2인 팀**. 3층 카드 권한 엘리베이터와 Python 모델 검증 GUI | 2-process FSM, 스텝모터, 포토인터럽터, UART 8바이트 프레임 |
| 12 | [minicar](12.minicar) | **2인 팀**. BLE 앱 조종과 초음파 자율 주행 RC카 | FreeRTOS Task 3개, Queue, Mutex, HC-SR04 3개, L298N PWM |

## 학습 흐름

```
01 GPIO ─▶ 02 FND ─▶ 03 상태 머신·디바운싱 ─▶ 04 타이머 인터럽트·계층 구조
                                                       │
08 PWM ◀─ 07 센서(EXTI, us 딜레이) ◀─ 06 스텝모터 ◀─ 05 I2C (RTC, LCD)

09 스마트워치: 05~07 기능 통합 + 버튼 이벤트 큐
10 조이스틱: ADC + DMA

11 엘리베이터 (팀): 스텝모터, 센서, FSM + PC와 UART 프로토콜
12 RC카 (팀): 슈퍼루프에서 FreeRTOS Task 구조로 전환
```

## 코드 구조 (04번 이후)

```
main.c          CubeMX 생성 코드 + ap_main() 호출
ap/             애플리케이션: 모드 전환, 각 기능의 task / ISR
  service/      (07~) 센서 값 처리, 화면 표시 같은 기능 단위
driver/         하드웨어 드라이버: button, led, fnd, lcd, DS3231, stepmotor, DHT11, HCSR04
common/         (07~) 공통 유틸리티 (us 딜레이, 09~ 링 버퍼 큐)
```

- 메인 루프(super loop)에서 각 모듈의 `xxx_task()`를 반복 호출 (12번은 FreeRTOS Task로 나누고 Queue와 Mutex로 연결)
- 시간이 정확해야 하는 일은 타이머 인터럽트 콜백(`HAL_TIM_PeriodElapsedCallback`)에서 처리
- 함수 이름 규칙: `모듈_동사목적어` (예: `fnd_show_number`, `button_getState`)
