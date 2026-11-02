# 04. watch_stopwatch

FND에 표시하는 **시계**와 **스톱워치**를 버튼 하나로 전환합니다.
이 프로젝트부터 코드를 `ap`(애플리케이션)와 `driver`(하드웨어)로 나눴고, 타이머 인터럽트를 사용합니다.

## 동작

| 버튼 | 동작 |
|---|---|
| PC0 | 시계 ↔ 스톱워치 모드 전환 |
| PC1 | (스톱워치) Run / Stop |
| PC3 | (스톱워치) Stop 상태에서 Clear |

- 시계와 스톱워치 모두 **MM.SS** 형식 (59:59 다음 00:00)
- 시계는 모드와 상관없이 계속 흐름
- 가운데 점(DP)이 0.5초 간격으로 깜빡임

| LED | 의미 |
|---|---|
| PB15 | 시계 모드 |
| PB14 | 스톱워치 모드 |
| PC4 | 스톱워치 Run |
| PB13 | 스톱워치 Stop |

## 타이머 설정 (시스템 클럭 100MHz)

| 타이머 | PSC | ARR | 주기 | 용도 |
|---|---|---|---|---|
| TIM2 | 9999 | 9999 | 1초 | 시계 1초 증가 (`Watch_tick`) |
| TIM3 | 99 | 999 | 1ms | FND 한 자리씩 표시 (`fnd_show_number`) |

> 주기 = 100MHz ÷ (PSC+1) ÷ (ARR+1)

## 구조

```
main.c                CubeMX 생성 코드, ap_main() 호출
ap/
  ap_main.c           모드 전환, 타이머 인터럽트 콜백
  Watch/              시계 (TIM2로 1초씩 증가)
  stopWatch/          스톱워치 (STOP / RUN / CLEAR 상태 머신)
driver/
  button/             버튼 눌림·뗌 감지 (디바운싱)
  fnd/                FND 표시
  led/                LED on / off / toggle
```

## 구현 포인트

- **FND 표시를 인터럽트로**: 03번에서는 메인 루프가 FND를 그렸지만, 여기서는 TIM3 1ms 인터럽트가 그림
  → 메인 루프는 `fnd_set_num()`으로 값만 바꾸면 됨
- **드라이버를 구조체로**: `button_t`, `led_t`에 GPIO 포트·핀을 담아서 같은 코드로 버튼·LED 여러 개를 다룸
- **버튼 이벤트**: 이전 상태와 비교해서 `ACT_PUSH`(눌림), `ACT_RELEASE`(뗌), `NO_ACT`를 구분
