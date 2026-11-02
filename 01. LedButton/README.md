# 01. LedButton

버튼 3개로 LED 5개를 켜고, 끄고, 깜빡이게 하는 GPIO 입출력 기초 실습입니다.

## 동작

| 버튼 | 동작 |
|---|---|
| PC0 | LED 5개 모두 켜기 |
| PC1 | LED 5개 모두 끄기 + 깜빡임 정지 |
| PC3 | 200ms 간격으로 깜빡이기 시작 |

## 핀 구성

| 구분 | 핀 |
|---|---|
| 버튼 (입력) | PC0, PC1, PC3 (버튼을 누르면 0) |
| LED (출력) | PA5(보드 LED), PB13, PB14, PB15, PC4 |

## 구현 포인트

- `HAL_GPIO_ReadPin`으로 버튼을 읽고, `HAL_GPIO_WritePin` / `HAL_GPIO_TogglePin`으로 LED를 제어
- 깜빡임 상태를 변수 `i`에 저장해서, PC3을 한 번 누르면 손을 떼도 계속 깜빡임
- 깜빡이는 동안에는 `HAL_Delay(200)` 때문에 버튼 반응이 최대 200ms 늦어짐

## 파일

- `main.c`: 모든 코드 (CubeMX 생성 코드 + 메인 루프)
