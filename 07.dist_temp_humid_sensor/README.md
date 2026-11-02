# 07. dist_temp_humid_sensor

**HC-SR04 초음파 센서**로 거리를, **DHT11**로 온도·습도를 측정해서 I2C LCD에 1초마다 표시합니다.

## 화면

```
Dist:123cm
T:25.0C H:40.0%
```

## 하드웨어 연결

| 장치 | 핀 |
|---|---|
| HC-SR04 Trig | PC5 (출력) |
| HC-SR04 Echo | PC6 (EXTI 인터럽트, 상승·하강 엣지) |
| DHT11 Data | PC8 |
| 16x2 I2C LCD | I2C1 |
| 시간 측정 타이머 | TIM10 (PSC 99 → 1us 단위 카운트) |

## 동작 흐름

```
while(1)
  TH_task()        2초마다 DHT11 읽기
  display_task()   1초마다
                     ├ HC-SR04 결과가 있으면 LCD 1행에 거리 표시
                     ├ 다음 측정 시작 (Trig)
                     └ LCD 2행에 온습도 표시
```

## 구현 포인트

### HC-SR04 (`driver/HCSR04`)
기다리는 동안 CPU를 막지 않도록 **인터럽트 + 상태 머신**으로 만들었습니다.

```
IDLE ──start()──▶ WAIT_RISE ──Echo 상승──▶ WAIT_FALL ──Echo 하강──▶ DONE
                      │                        │
                      └─ 타임아웃 ─▶ IDLE ◀─ 타임아웃 ┘
```

1. `HCSR04_start()`: Trig 핀에 10us 펄스
2. Echo 핀 EXTI 인터럽트(`HCSR04_echoISR`)에서 상승·하강 시점의 TIM10 카운터 값 저장
3. `HCSR04_getResult()`: 두 값의 차이 = Echo 시간(us). 타이머가 한 바퀴 넘어간 경우도 계산
4. 거리(cm) = Echo 시간(us) ÷ 58

### DHT11 (`driver/DHT11`)
선 하나로 주고받는 단선 통신입니다.

1. 핀을 출력으로 바꿔 18ms 동안 LOW → 시작 신호
2. 핀을 입력으로 바꾸고 센서 응답(LOW 80us, HIGH 80us) 확인
3. 40비트 수신: 각 비트의 HIGH 길이가 **40us보다 길면 1, 짧으면 0**
4. 습도 정수·소수, 온도 정수·소수 4바이트를 더한 값이 체크섬과 같은지 확인
5. 응답이 없거나 시간이 넘으면 에러 코드 반환

### us 단위 딜레이 (`common/delay`)
`HAL_Delay`는 ms 단위라서 Cortex-M4의 **DWT 사이클 카운터**를 사용했습니다.

- 100MHz에서 1us = 100 사이클
- `delay_us()`: 정해진 사이클만큼 대기
- `delay_elapsed_us()`: 시작 시점부터 지난 시간 측정 (DHT11 비트 길이 측정에 사용)

## 구조

```
ap/
  ap_main.c               메인 루프, EXTI 콜백
  service/
    temp_humid/           2초마다 DHT11 읽고 값 보관
    display/              거리·온습도를 LCD에 출력
driver/
  HCSR04/ DHT11/ lcd/
common/
  delay/                  DWT 기반 us 딜레이
```

04~06번의 `ap / driver` 구조에 **service** 계층(센서 값을 다루는 기능 단위)과 **common**(공통 유틸)을 추가했습니다.
