# 09. smartWatch

**NUCLEO-F411RE**로 만든 스마트워치입니다.
모드 버튼 하나로 **달력 → 온습도 → 거리 측정 → 스톱워치** 화면을 차례대로 바꿉니다.
버튼 입력을 바로 처리하지 않고 **큐(링 버퍼)에 이벤트로 넣었다가 꺼내 쓰는 구조**로 만들었습니다.

## 개발 환경

| 항목 | 내용 |
|---|---|
| 보드 | NUCLEO-F411RE (STM32F411, Cortex-M4) |
| 시스템 클럭 | 100 MHz |
| 개발 도구 | STM32CubeIDE 2.2.0 |
| 라이브러리 | STM32 HAL |

## 동작

### 모드 전환 (PC0 버튼, 뗄 때)

```
S_CALENDAR ──▶ S_TEMP_HUMID ──▶ S_RULER ──▶ S_STOPWATCH ──┐
    ▲                                                    │
    └────────────────────────────────────────────────────┘
```

| 모드 | LCD 1행 | LCD 2행 | 버튼 |
|---|---|---|---|
| 달력 | `Time : HH:MM:SS` | `20YY-MM-DD [요일]` | - |
| 온습도 | `Time : HH:MM:SS` | `T:25.0C H:40%` (2초마다 갱신) | - |
| 거리 측정 | `Time : HH:MM:SS` | `dist : 0000cm` | PC1: 측정 요청 |
| 스톱워치 | `Time : HH:MM:SS` | `SW : HH:MM:SS:cc` | PC1: Run / Stop, PC3: Clear (Stop 상태에서) |

- LCD 1행의 현재 시각은 모드와 상관없이 1초마다 갱신됩니다.

## 하드웨어 연결

| 장치 | 연결 |
|---|---|
| 모드 버튼 | PC0 |
| 기능 버튼 | PC1 (거리 측정 / 스톱워치 Run·Stop), PC3 (스톱워치 Clear) |
| DS3231 RTC | I2C1 (0x68) |
| 16x2 I2C LCD | I2C1 (0x27) |
| DHT11 | PC8 |
| HC-SR04 | Trig PC5, Echo PC6 (EXTI 상승·하강 엣지), **VCC 5V** |

## 타이머

| 타이머 | PSC | ARR | 주기 | 용도 |
|---|---|---|---|---|
| TIM3 | 999 | 999 | 10ms | 스톱워치 시간 증가 (`stopWatch_ISR`) |
| TIM10 | 99 | 65535 | 1us 카운트 | HC-SR04 Echo 시간 측정 |

## 코드 구조

```
09.smartWatch/
├── main.c                    CubeMX 생성 코드, ap_main() 호출
├── ap_main/
│   ├── ap_main.c             모드 상태 머신, 인터럽트 콜백
│   └── service/
│       ├── inputEvent/       모드 버튼을 읽어서 큐에 이벤트로 넣음
│       ├── que/              모드 큐, 기능 버튼 큐
│       ├── timeClock/        RTC 시간·날짜 읽기, LCD 표시
│       ├── tempHumid/        DHT11 온습도
│       ├── ruler/            HC-SR04 거리 측정
│       └── stopWatch/        스톱워치 (STOP / RUN / CLEAR)
├── driver/                   button, led, fnd, lcd, DS3231, DHT11, HCSR04
└── common/
    ├── queue/                범용 링 버퍼 (que_buffer)
    └── delay/                DWT 기반 us 딜레이
```

## 버튼 이벤트 큐

### 흐름

```
[PC0 버튼] → inputEvent_task() → que_putMode(ACT_RELEASED)
                                         │
                                   ┌─────▼─────┐
                                   │  que_mode │  링 버퍼
                                   └─────┬─────┘
                                         │
ap_main() ← que_getMode(&btn_mode) ──────┘  → 모드 전환
```

- **inputEvent**: 50ms마다 모드 버튼을 확인하고, 버튼을 뗀 순간(`ACT_RELEASE`) 큐에 이벤트를 넣습니다.
- **ap_main**: 큐가 비어 있지 않으면 하나 꺼내서 다음 모드로 넘어갑니다.
- 버튼을 읽는 쪽과 처리하는 쪽이 분리되어 있어서, 처리가 늦어도 입력이 큐에 쌓여 순서대로 처리됩니다.

### 범용 링 버퍼 (`common/queue/que_buffer`)

`void *`와 `elem_size`를 받기 때문에 어떤 자료형이든 담을 수 있습니다.

| 함수 | 동작 |
|---|---|
| `que_buf_init(que, storage, elem_size, capacity)` | 저장할 배열과 원소 크기 지정 |
| `que_put(que, elem)` | `head` 위치에 `memcpy`, 가득 차면 `false` |
| `que_get(que, elem)` | `tail` 위치에서 `memcpy`, 비었으면 `false` |
| `que_is_empty` / `que_is_full` | `head == tail` / `next(head) == tail` |

> 가득 참과 비어 있음을 구분하려고 한 칸을 비워 두기 때문에, `capacity`가 N이면 실제로는 **N-1개**까지 담깁니다.

## 디버깅 기록

버튼 입력을 큐 구조로 바꾸는 과정에서 생긴 문제를 직접 디버깅해서 해결했습니다.

### 1. 링크 에러: `multiple definition of 'btn_mode'`

```
que.c:12: multiple definition of `btn_mode'; ap_main.c:27: first defined here
```

- **원인**: `ap_main.c`(현재 모드 변수)와 `que.c`(큐 저장 배열)에 같은 이름의 전역 변수 `btn_mode`가 있었습니다. `static`이 없는 전역 변수는 프로젝트 전체에서 이름이 하나여야 하기 때문에, 파일을 합치는 링크 단계에서 실패했습니다.
- **해결**: `que.c`의 저장 배열을 `static`으로 바꿔서 그 파일 안에서만 보이게 했습니다.

### 2. 큐에서 꺼낸 값이 사라짐

```c
void que_getMode(event_btn_act_t act){   // 값으로 받음
	que_get(&que_mode, &act);            // 함수 안의 복사본에만 씀
}
```

- **원인**: C는 인자를 복사해서 넘깁니다. 그래서 꺼낸 값이 복사본 `act`에만 들어가고, `ap_main`의 `btn_mode`는 바뀌지 않았습니다.
- **해결**: 포인터로 받도록 바꿨습니다. `que_getMode(event_btn_act_t *act)` → `que_get(&que_mode, act)`, 호출부는 `que_getMode(&btn_mode)`
- **주의**: 포인터로 바꾼 뒤에도 `&act`가 남아 있으면 "포인터 변수의 주소"에 쓰게 되어 같은 문제가 생깁니다.

### 3. 버튼 이벤트 비교 값 혼동 (`-Wenum-compare` 경고)

| 이름 | 정의 | 값 |
|---|---|---|
| `ACT_PUSH` / `ACT_RELEASE` | button.h (`button_getState` 반환값) | 1 / 2 |
| `ACT_RELEASED` | que.h (큐 이벤트) | 1 |

- **원인**: `button_getState() == ACT_RELEASED`로 비교했습니다. 값 1은 `ACT_PUSH`와 같아서, 뗄 때가 아니라 누를 때 반응했습니다.
- **해결**: 버튼 상태는 `ACT_RELEASE`와 비교하고, 큐에는 `ACT_RELEASED`를 넣었습니다.

### 4. `inputEvent_task()` 호출 누락

- `inputEvent_init()`만 있고 메인 루프에서 `inputEvent_task()`를 부르지 않아서, 버튼을 읽지 않았습니다. `while(1)` 안에 추가했습니다.

### 5. 디버거로 큐 동작 확인

STM32CubeIDE 디버그 모드에서 브레이크포인트와 Expressions로 확인했습니다.

| 확인 항목 | 방법 | 결과 |
|---|---|---|
| 버튼 핀 | `GPIOC->IDR & 1` | 안 누름 1, 누름 0 |
| 버튼 상태 | `btnMode.prevState` | 누름 0 → 뗌 1 |
| 큐에 넣기 | `inputEvent.c` 27줄 브레이크포인트 | 버튼을 뗄 때 멈춤 |
| 큐 동작 | `que_mode.head`, `que_mode.tail` | 넣으면 head +1, 꺼내면 tail +1 |

- `elem_size`가 1로 보이는 건 정상입니다. ARM GCC는 enum을 값 범위에 맞는 가장 작은 크기(1바이트)로 만듭니다.

### 6. 큐에 쌓이는지 테스트

꺼내는 쪽을 일부러 느리게 만들어서, 버튼을 빠르게 여러 번 눌렀을 때 이벤트가 쌓였다가 순서대로 처리되는지 확인했습니다.

```c
static bool ap_getModeEvent(void){
	static uint32_t prevTime = 0;

	if(HAL_GetTick() - prevTime < 1000) return false;   // 1초에 하나만 꺼냄
	if(que_isEmptyMode() == true) return false;
	prevTime = HAL_GetTick();

	que_getMode(&btn_mode);
	return (btn_mode == ACT_RELEASED);
}
```

- **결과**: 버튼을 빠르게 누른 횟수만큼 1초 간격으로 모드가 넘어가는 것을 확인했습니다.
- 테스트 코드는 `ap_main.c`에 주석으로 남겨 두었습니다.

### 7. 거리 측정 값이 `0000cm`에서 바뀌지 않음

`0000cm`은 측정값이 아니라, 모드에 들어올 때 `ruler_disp(0)`이 찍은 초기 화면이었습니다. 원인이 두 가지였습니다.

**① 측정 결과를 확인하기 전에 상태부터 검사함 (코드)**

```c
static hcsr04_status_t hcsr04_status = HCSR04_ERR_NOT_STARTED;
...
if(hcsr04_status != HCSR04_OK){                 // 처음 값이 NOT_STARTED → 항상 return
	return;
}
hcsr04_status = HCSR04_getResult(&echo_us);     // 한 번도 실행되지 않음
```

- `hcsr04_status`는 `HCSR04_getResult()`를 불러야만 바뀌는데, 그 앞에서 항상 return돼서 결과를 받아 오지 못했습니다.
- **해결**: 결과를 먼저 받고, 그 값으로 판단하도록 순서를 바꿨습니다.

```c
hcsr04_status = HCSR04_getResult(&echo_us);   // 먼저 결과 확인
if(hcsr04_status != HCSR04_OK){               // 측정 중이거나 실패면 다음 루프에서
	return;
}
start_flag = false;
ruler_disp(echo_us * 17 / 1000);
```

> 측정이 실패(`ERR_NO_ECHO`)하면 `start_flag`가 true로 남지만, 다음에 PC1을 누르면 `ruler_request()`가 새로 측정을 시작하므로 동작에는 문제가 없습니다.

**② HC-SR04 전원이 연결되지 않음 (하드웨어)**

- 코드를 고친 뒤에도 측정이 되지 않았습니다. 핀·타이머·EXTI 설정은 예전에 동작했던 프로젝트와 같았습니다.
- 디버거로 측정 단계를 나눠 확인하려던 중, 센서 VCC(5V)가 연결되지 않은 것을 발견했습니다.
- 전원을 연결하고 정상적으로 측정되는 것을 확인했습니다.

**단계별 확인 방법 (다음에 같은 증상이 생기면)**

```
PC1 버튼 → ruler_request() → Echo 인터럽트 → getResult() == OK → 화면 표시
```

| 확인 위치 | 안 멈추면 |
|---|---|
| `ruler.c` `start_flag = true;` 브레이크포인트 | PC1 버튼 인식 문제 |
| `HCSR04.c` `HCSR04_echoISR()` 첫 줄 브레이크포인트 | Echo가 안 옴 → **센서 전원**, 배선 확인 |
| Live Expressions `hcsr04_status` | 4(`ERR_NO_ECHO`)면 시간 안에 Echo가 안 온 것 |
