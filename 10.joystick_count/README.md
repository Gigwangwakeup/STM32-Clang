# 10. joystick_count

**NUCLEO-F411RE**에 조이스틱을 연결해서, 기울인 방향으로 **X·Y 카운트를 올리거나 내리는** 프로젝트입니다.
조이스틱 값은 **ADC + DMA**로 읽고, 결과는 16x2 I2C LCD에 표시합니다.
조금 기울이면 **1초에 한 번**, 끝까지 기울이면 **0.1초에 한 번** 카운트가 바뀝니다.

## 개발 환경

| 항목 | 내용 |
|---|---|
| 보드 | NUCLEO-F411RE (STM32F411, Cortex-M4) |
| 시스템 클럭 | 100 MHz |
| 개발 도구 | STM32CubeIDE |
| 라이브러리 | STM32 HAL |

## 하드웨어 연결

| 장치 | 연결 |
|---|---|
| 조이스틱 X축 | PA4 (ADC1_IN4) |
| 조이스틱 Y축 | PB0 (ADC1_IN8) |
| 16x2 I2C LCD | I2C1 (0x27) |

> ADC는 4채널(IN0, IN1, IN4, IN8)을 스캔하지만, 이 프로젝트에서는 Rank 3(IN4)과 Rank 4(IN8)만 사용합니다.

## ADC 설정

| 항목 | 값 |
|---|---|
| 분해능 | 12bit (0 ~ 4095) |
| 모드 | Scan + Continuous + DMA (`DMAContinuousRequests` ENABLE) |
| 변환 순서 | Rank1 IN0 → Rank2 IN1 → Rank3 IN4 → Rank4 IN8 |
| 샘플링 시간 | 480 cycles |

`cds_init()`에서 `HAL_ADC_Start_DMA()`를 한 번 호출하면, DMA가 4채널 변환 결과를 `value[4]` 배열에 계속 채워 넣습니다.
한 바퀴 변환이 끝날 때마다 `HAL_ADC_ConvCpltCallback()`에서 `adcCpltFlag`를 세우고, 메인 루프가 플래그를 보고 값을 읽습니다.

```
ADC 변환 ──DMA──▶ value[0..3] ──▶ ConvCpltCallback: adcCpltFlag = 1
                                        │
ap_main() ◀── cds_getValue(2), (3) ◀────┘
```

## 동작

ADC 값(0 ~ 4095)을 구간으로 나누고, 구간마다 카운트 방향과 간격을 다르게 했습니다.

| ADC 값 | X 카운트 | Y 카운트 | 간격 |
|---|---|---|---|
| 0 ~ 1000 | -1 | +1 | 100ms (빠르게) |
| 1001 ~ 1999 | -1 | +1 | 1000ms (천천히) |
| 2000 ~ 2100 | 변화 없음 | 변화 없음 | 중앙(데드존) |
| 2101 ~ 3099 | +1 | -1 | 1000ms (천천히) |
| 3100 ~ 4095 | +1 | -1 | 100ms (빠르게) |

- Y축은 조이스틱을 위로 올렸을 때 값이 작아지기 때문에, X축과 증감 방향을 반대로 했습니다.
- 중앙 부근(2000 ~ 2100)은 조이스틱을 놓았을 때 값이 조금씩 흔들려도 카운트가 바뀌지 않도록 비워 두었습니다.

### LCD 표시

```
x:0012
y:-003
```

## 코드 구조

```
10.joystick_count/
├── main.c                CubeMX 생성 코드 (ADC1, DMA2, I2C1, USART2 초기화), ap_main() 호출
├── ap_main/
│   └── ap_main.c         ADC 콜백, X·Y 카운트 증감, LCD 출력
└── driver/
    ├── cds/              ADC DMA 시작, 채널 값 읽기 (cds_getValue)
    └── lcd/              16x2 I2C LCD
```

## 시간 간격 처리

`HAL_Delay()`로 기다리면 그동안 Y축이나 LCD를 처리할 수 없습니다.
그래서 마지막으로 카운트한 시각을 기억해 두고, 지금 시각과의 차이가 정한 간격보다 클 때만 카운트하도록 했습니다.

```c
void x_count_upDown(void){
	static uint32_t prevXtime = 0;

	if ((x <= 1000) && ((HAL_GetTick() - prevXtime) > 100)){
		prevXtime = HAL_GetTick();
		x_count --;
	}
	...
}
```

X축과 Y축이 각자의 `prevXtime`, `prevYtime`을 가지고 있어서, 서로 다른 속도로 독립적으로 카운트됩니다.

## 디버깅 기록

### 간격(100ms / 1000ms)을 바꿔도 카운트 속도가 변하지 않음

처음에는 `prevXtime`, `prevYtime`을 `static` 없이 선언했습니다.

```c
void x_count_upDown(void){
	uint32_t prevXtime = 0;    // static 없음

	if ((x <= 1000) && ((HAL_GetTick() - prevXtime) > 100)){
		prevXtime = HAL_GetTick();
		x_count --;
	}
	...
}
```

- **증상**: 비교하는 간격 값(100, 1000)을 바꿔도 카운트되는 속도가 그대로였습니다.
- **원인**: 지역 변수는 함수가 **호출될 때마다 새로 만들어지고 초기화**됩니다.
  그래서 `prevXtime = HAL_GetTick()`으로 저장한 값이 함수가 끝나면 사라지고, 다음 호출에서는 다시 0이 됩니다.
  - 매번 `HAL_GetTick() - 0` = 부팅 후 지난 시간 → 부팅하고 1초만 지나면 조건이 **항상 참**
  - 결국 ADC 변환이 끝날 때마다(루프가 돌 때마다) 카운트되어서, 속도는 간격 값이 아니라 **루프 속도(LCD I2C 전송 시간 등)** 로 정해졌습니다.
- **해결**: `static`을 붙였습니다.

```c
static uint32_t prevXtime = 0;
static uint32_t prevYtime = 0;
```

`static` 지역 변수는 프로그램이 시작될 때 **한 번만 초기화**되고, 함수가 끝나도 값이 남아 있습니다.
보이는 범위는 그 함수 안으로 그대로라서, 전역 변수로 빼지 않고도 이전 호출의 시각을 기억할 수 있습니다.

| | 일반 지역 변수 | `static` 지역 변수 |
|---|---|---|
| 초기화 | 호출될 때마다 | 프로그램 시작 시 한 번 |
| 함수가 끝나면 | 사라짐 | 값 유지 |
| 접근 범위 | 함수 안 | 함수 안 |
