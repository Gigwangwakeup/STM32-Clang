# RTOS RC Car

STM32 NUCLEO-F411RE와 FreeRTOS로 만든 RC카입니다. 아이폰 앱의 조이스틱으로 조종하는 **수동 주행**과, 초음파 센서 3개로 벽 사이를 따라가는 **자율 주행**을 앱 버튼으로 바꿔 가며 쓸 수 있습니다.

- 팀: 15조 (이기광, 정명훈)
- 기간: 2026.11
- 발표 자료: [RTOS_RC_Car_Project.pptx](RTOS_RC_Car_Project.pptx)
- 시연 영상: [Video.mp4](Video.mp4)

## My Contribution (이기광)

| 맡은 부분 | 코드 | 설명 |
|---|---|---|
| 초음파 3개 순차 측정 | [distance_task.c](Src/Src/ap/task/distance_task/distance_task.c), [HCSR04.c](Src/Src/driver/HCSR04/HCSR04.c) | [초음파 거리 측정](#초음파-거리-측정-distance_task-hcsr04) |
| 자율 주행 적용 | [motor_task.c](Src/Src/ap/task/motor_task/motor_task.c)의 `auto_motor_controller()` | [자율 주행](#자율-주행-auto_motor_controller). 조향 알고리즘은 AI에게 물어 받은 방식 |
| 트러블슈팅 | `distance_task.c` | [오른쪽 거리가 계속 0](#2-오른쪽-거리가-계속-0): `&&` 단락 평가 때문에 오른쪽 결과를 읽는 함수가 건너뛰어진 버그 |

발표 자료에서는 9쪽(State Machine: Auto Mode)과 10쪽(Troubleshooting: Right Distance Always 0)이 제 파트입니다.

## 주요 기능

**수동 주행 (Stick Mode)**
- 앱 조이스틱의 각도를 6방향(전진, 좌/우 대각 전진, 좌회전, 우회전, 후진)으로 나누고, 기울기(0~1)를 속도(0~999)로 바꿉니다.
- 기울기가 0.2 미만이면 정지합니다.
- 후진 중에는 대각 전진으로 바로 넘어가지 않습니다.

**모드 전환**
- 앱 B0 버튼을 누를 때마다 AUTO와 MANUAL이 바뀝니다. T0 토글, `AUTO` / `MANUAL` 문자열로도 바꿀 수 있습니다.
- 모드를 바꿀 때는 모터를 먼저 세운 뒤 새 모드를 시작합니다.

**자율 주행 (Auto Mode)**
- 초음파 센서 3개를 정면, 오른쪽, 왼쪽 순서로 하나씩 측정합니다.
- 좌우 거리 차이에 비례해서 넓은 쪽으로 꺾고, 정면 벽이 가까워지면 더 꺾습니다.

**표시와 디버그**
- LCD 1행: 모터 상태 (`Motor State FW` 등)
- LCD 2행: 세 방향 거리 (`R:xxxC:xxxL:xxx`)
- USART2(ST-LINK 가상 COM)로 PC에서 `FORWARD*`, `SPD500*`처럼 `*`로 끝나는 명령을 보낼 수 있습니다.

## 하드웨어

| 부품 | 모델 | 수량 | 연결 |
|---|---|---|---|
| MCU 보드 | STM32 NUCLEO-F411RE (Cortex-M4, 100 MHz) | 1 | |
| 모터 드라이버 | L298N 듀얼 H-브리지 (HW-095 모듈) | 1 | 채널마다 PWM 1선 + 방향 2선 |
| DC 모터 | DC 기어드 모터 | 4 | 좌우 2채널로 구동 |
| 초음파 센서 | HC-SR04 | 3 | Trig 출력, Echo는 EXTI |
| LCD | 16x2 문자 LCD (I2C, 0x27) | 1 | I2C1 |
| 블루투스 | HM-10 (BLE) | 1 | USART6 |
| 앱 | iPhone Bluetooth Serial Connect | | 조이스틱, 모드 버튼 |
| PC | ComPort Master | | USART2 디버그 |

### 핀 배치

| 기능 | 신호 | 핀 | 설정 |
|---|---|---|---|
| LCD | SCL / SDA | PB8 / PB9 | I2C1 |
| PC 디버그 | TX / RX | PA2 / PA3 | USART2, 115200 8N1 |
| 블루투스 | TX / RX | PA11 / PA12 | USART6, 9600 8N1, 인터럽트 수신 |
| 왼쪽 모터 | PWM / DIR | PC6 / PB6, PA7 | TIM3 CH1 |
| 오른쪽 모터 | PWM / DIR | PC8 / PA5, PA6 | TIM3 CH3 |
| 초음파 정면 | Trig / Echo | PA10 / PC4 | EXTI4 |
| 초음파 오른쪽 | Trig / Echo | PC2 / PC3 | EXTI3 |
| 초음파 왼쪽 | Trig / Echo | PC1 / PC0 | EXTI0 |

### 타이머

| 타이머 | 설정 | 용도 |
|---|---|---|
| TIM3 | PSC 99, ARR 999 (1 kHz) | 모터 PWM, duty 0~999 |
| TIM11 | PSC 99, ARR 65535 (1 MHz 카운터) | 초음파 Echo 펄스 폭을 us 단위로 측정 |

## 소프트웨어 구조

```
 Application   StartMotorTask, StartDistanceTask  ---->  FreeRTOS Kernel
               StartCommTask, StartDefaultTask           (CMSIS-OS v1)
      |                                                  osMessagePut / Get
      v                                                  osMutexWait / Release
 Service       motor_task, distance_task,         ---->  osDelay
               comm_task, message (Queue, Mutex)
      |                                                      |
      v                                                      |
 Driver        dc_motor, HCSR04, lcd, delay (DWT)            |
      |                                                      |
      v                                                      |
 HAL (ST)      TIM3, TIM11, GPIO, EXTI,                  SysTick, PendSV
               USART2, USART6, I2C1                          |
      |                                                      |
      v                                                      v
 ======================== Hardware (Cortex-M4 core, peripherals) ========================
```

- 직접 작성한 부분은 Application, Service, Driver이고, HAL(ST)과 FreeRTOS는 가져다 썼습니다.
- 호출은 위에서 아래로만 갑니다: Application → Service → Driver → HAL.
- FreeRTOS 커널은 이 층 사이에 끼어 있지 않고 HAL과 나란히 하드웨어 위에 있습니다. 커널 API는 Application과 Service에서만 부르고, Driver는 HAL만 씁니다.
- 커널은 SysTick(1 ms 틱)과 PendSV(Task 전환)를 직접 쓰기 때문에, HAL의 시간 기준(`HAL_GetTick`)은 TIM10으로 옮겨져 있습니다.

### Task

| Task | 주기 | 하는 일 |
|---|---|---|
| CommTask | 1 ms | 한 줄이 들어오면 명령을 해석해 `commQueue`로 보냄 |
| MotorTask | 10 ms | 큐를 대기 없이 확인하고, 모드에 따라 수동 상태 머신 또는 자율 조향 실행 |
| DistanceTask | 50 ms | 초음파 측정 결과를 확인하고 다음 센서를 트리거, LCD 2행에 거리 표시 |

- 세 Task는 우선순위가 같습니다.
- LCD는 여러 Task가 같이 쓰므로 `lcdMutex`로 보호합니다.
- 인터럽트(UART 수신, EXTI)에서는 값만 저장하고, 판단과 출력은 Task에서 합니다.

### 앱 명령 형식 (블루투스, `\n`으로 끝남)

| 명령 | 예 | 처리 |
|---|---|---|
| 조이스틱 | `J0:90,0.75` | 각도 → 방향, 크기 × 999 → 속도 |
| 토글 | `T0:1` / `T0:0` | AUTO / MANUAL |
| 버튼 | `B0:D` | 누를 때마다 AUTO와 MANUAL 전환 (`B0:U`는 무시) |

조이스틱 각도와 방향은 이렇게 대응합니다.

| 각도 | 방향 |
|---|---|
| 67 ~ 112 | 전진 |
| 22 ~ 66 | 우측 대각 전진 |
| 113 ~ 157 | 좌측 대각 전진 |
| 158 ~ 202 | 좌회전 |
| 203 ~ 337 | 후진 |
| 338 ~ 360, 0 ~ 21 | 우회전 |

## 초음파 거리 측정 (distance_task, HCSR04)

- TIM11을 1 MHz로 계속 돌리고, Echo 핀의 상승과 하강 에지에서 EXTI가 카운터 값을 저장합니다. 두 값의 차이가 Echo 펄스 폭(us)이고, 카운터가 한 바퀴 넘어간 경우도 계산합니다.
- 거리는 `cm = echo_us * 17 / 1000`으로 구합니다 (음속 340 m/s, 왕복).
- 센서 3개를 동시에 쏘면 서로의 반사파를 받을 수 있어서, 정면 → 오른쪽 → 왼쪽 순서로 하나가 끝나면 다음 센서를 쏩니다.
- DistanceTask가 50 ms마다 돌기 때문에, 같은 센서는 150 ms 이상 간격으로 측정됩니다. HC-SR04 데이터시트가 권하는 60 ms 이상의 측정 주기를 만족합니다.
- Echo가 시작되지 않으면 5 ms, 끝나지 않으면 50 ms 뒤 타임아웃으로 처리합니다.

## 자율 주행 (auto_motor_controller)

> 조향 알고리즘은 처음에 직접 짜 보다가 잘 되지 않아, AI(Claude)에게 물어 받은 비례 조향 방식을 적용했습니다.

상태를 여러 개 두는 FSM 대신, 10 ms마다 세 방향 거리로 좌우 바퀴 속도를 바로 계산합니다. 기억하는 값은 마지막으로 정한 꺾는 방향(`turn_right`) 하나입니다.

| 단계 | 조건 | 동작 |
|---|---|---|
| 1 | 정면 ≥ 300 cm | 정지 (코스에서 나올 수 없는 값이라 센서가 벽에 붙은 것으로 봄) |
| 2 | 왼쪽 ≥ 300 cm 또는 오른쪽 ≥ 300 cm | 반대쪽으로 최대 조향 |
| 3 | 좌우 차이 > 7 cm | 넓은 쪽으로 방향(`turn_right`) 갱신. 7 cm 이하는 잡음으로 보고 이전 방향 유지 |
| 4 | | 조향 세기 = 400 × (좌우 차이 − 7) / 30, 최대 400 |
| 5 | 정면 < 60 cm | 정면이 가까울수록 조향을 더함. 35 cm 이하면 최대 |
| 6 | | 바퀴 속도 = 기본 400 ± 조향 세기 |

| 튜닝값 | 값 | 의미 |
|---|---|---|
| `base` | 400 | 기본 속도 |
| `deadband` | 7 cm | 이 이하의 좌우 차이는 무시 |
| `side_full` | 30 cm | 잡음을 뺀 좌우 차이가 이만큼이면 최대 조향 |
| `front_start` | 60 cm | 정면 보정 시작 거리 |
| `front_full` | 35 cm | 정면 보정 최대 거리 |
| `far_limit` | 300 cm | 센서가 벽에 붙은 것으로 보는 값 |

## 트러블슈팅

### 1. 앱 명령을 보드가 알아듣지 못함

- **문제:** 처음 명령 파서는 PC 디버깅용으로 `*`로 끝나는 문자열만 처리했습니다. 앱은 줄바꿈으로 끝나고 `J0:90,0.75`처럼 조이스틱 각도와 크기를 보내서 형식이 달랐습니다.
- **해결:** 블루투스(USART6)는 `\n`으로 명령 끝을 확인하고, 첫 글자 J, T, B로 조이스틱, 토글, 버튼을 나눠 방향, 속도, 모드로 바꿨습니다. PC(USART2)의 `*` 명령은 디버깅용으로 남겼습니다.

### 2. 오른쪽 거리가 계속 0

- **문제:** 정면을 잰 다음 좌우를 동시에 쏘도록 바꿨더니, LCD에서 정면과 왼쪽 값은 바뀌는데 오른쪽만 0에 멈춰 있었습니다.
- **원인:** 좌우 측정이 끝났는지를 한 줄로 확인했습니다.
  ```c
  if(((distance_get(L...)) && (distance_get(R...))) == DISTANCE_OK)
  ```
  `DISTANCE_OK`가 enum의 첫 값이라 0, 즉 거짓입니다. C의 `&&`는 왼쪽을 먼저 계산하고 왼쪽이 0이면 오른쪽을 계산하지 않습니다(C11 6.5.13). 그래서 왼쪽이 끝나는 순간 오른쪽 결과를 읽는 함수가 건너뛰어졌고, `0 == DISTANCE_OK`가 참이 되어 바로 정면을 쐈습니다. 다음 호출에서는 정면 완료 처리가 오른쪽 상태를 먼저 지워서, 오른쪽 값은 한 번도 읽히지 않았습니다.
- **해결:** 함수 호출을 `&&` 조건 안에 넣지 않고, 센서마다 if를 따로 두는 정면 → 오른쪽 → 왼쪽 순차 측정으로 되돌렸습니다. LCD에서 세 값이 모두 갱신되는 것을 확인했습니다.

## 한계와 개선할 점

- **센서 사이 간격:** 같은 센서는 150 ms 이상 간격이 보장되지만, 다른 센서는 앞 센서가 끝난 직후 바로 쏠 수 있어 간격이 보장되지 않습니다. 앞 센서 측정 후 일정 시간 기다렸다가 다음 센서를 쏘는 처리가 필요합니다.
- **자율 주행 성능:** 좌우로 흔들리며 주행하는 문제가 있었습니다. 7 cm 데드밴드로 방향이 자주 바뀌는 것을 줄였지만, 초음파가 언제 돌아오는지와 측정 지연이 조향에 주는 영향을 충분히 측정하지 못해 원하는 만큼 안정적으로 만들지는 못했습니다.
- **데이터 갱신 주기:** MotorTask는 10 ms마다 돌지만 각 센서 값은 150 ms 이상마다 바뀌어서, 같은 거리 값으로 여러 번 계산합니다.
- **막다른 곳:** 후진이나 제자리 회전 같은 탈출 동작이 없습니다. 이런 동작이 필요하면 지금의 비례 조향을 하나의 상태로 두고 회피, 탈출 상태를 더한 FSM으로 확장할 수 있습니다.

## 폴더 구조

```
RTOS_Final_Resource/
├── RTOS_RC_Car_Project.pptx     발표 자료
├── Video.mp4                    시연 영상
└── Src/Src/
    ├── main.c, freertos.c       초기화, Task 생성
    ├── gpio.c, i2c.c, tim.c, usart.c, stm32f4xx_it.c ...   CubeMX 생성 코드
    ├── ap/
    │   ├── message.c/h          Queue, Mutex, 명령 구조체
    │   └── task/
    │       ├── comm_task/       UART 수신, 명령 해석
    │       ├── motor_task/      수동 상태 머신, 자율 조향
    │       ├── distance_task/   초음파 순차 측정, LCD 거리 표시
    │       └── button_task/     (미사용)
    ├── driver/
    │   ├── dc_motor/            PWM, 방향 핀 제어
    │   ├── HCSR04/              Trig, Echo 에지 측정
    │   ├── lcd/                 I2C 16x2 LCD
    │   └── button/
    └── common/delay/            DWT 기반 us 지연
```

## 개발 환경

- STM32CubeIDE, STM32CubeMX
- FreeRTOS (CMSIS-RTOS v1)
- HAL 드라이버

## 담당

- **이기광:** 초음파 거리 측정(`distance_task`, `HCSR04`), 자율 주행 적용(`auto_motor_controller`, 조향 알고리즘은 AI에게 물어 받은 방식)
- **정명훈:** 

## Contact

- GitHub: https://github.com/Gigwangwakeup
- LinkedIn: https://www.linkedin.com/in/%EA%B8%B0%EA%B4%91-%EC%9D%B4-538212345/
- Email: gigwangwakeup@gmail.com
