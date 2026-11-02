# 05. LCD_RTC_I2C

**I2C** 통신으로 **DS3231 RTC**에서 시간과 날짜를 읽고, **I2C 캐릭터 LCD(16x2)**와 FND에 표시합니다.
04번의 시계·스톱워치에 RTC와 LCD를 붙이고, 화면 출력을 `Display` 모듈로 분리했습니다.

## 동작

| 버튼 | 동작 |
|---|---|
| PC0 | 시계 ↔ 스톱워치 모드 전환 (버튼을 뗄 때) |
| PC1 | (스톱워치) Run / Stop |
| PC3 | (스톱워치) Stop 상태에서 Clear |

### 화면

| 모드 | LCD 1행 | LCD 2행 | FND |
|---|---|---|---|
| 시계 | `Time:HH:MM:SS:ms` | `20YY-MM-DD [요일]` | MM.SS |
| 스톱워치 | `Time:HH:MM:SS:ms` | `SW  :HH:MM:SS:ms` | M.SS.s |

LED: PB15 시계 모드, PB14 스톱워치 모드, PC4 Run, PB13 Stop

## 하드웨어 연결

| 장치 | 연결 | I2C 주소 |
|---|---|---|
| DS3231 RTC | I2C1 | 0x68 |
| 16x2 LCD (PCF8574 I2C 모듈) | I2C1 | 0x27 |

## 타이머 설정

| 타이머 | PSC | ARR | 주기 | 용도 |
|---|---|---|---|---|
| TIM3 | 99 | 999 | 1ms | FND 표시 (`Disp_ISR`) |
| TIM4 | 999 | 999 | 10ms | 시계·스톱워치 시간 증가 |

## 구조

```
ap/
  ap_main.c         모드 전환, 인터럽트 콜백
  TimeClock/        RTC 시간 읽기, 10ms 단위 시간 증가
  StopWatch/        스톱워치 상태 머신 (10ms 단위)
  Display/          LCD·FND·LED 출력만 담당
driver/
  DS3231/           RTC 읽기 / 쓰기
  lcd/              I2C LCD 드라이버
  fnd/ button/ led/
```

## 구현 포인트

### DS3231 드라이버
- RTC 레지스터는 **BCD** 형식 → `bcd2bin()`, `bin2bcd()`로 변환
- `HAL_I2C_Mem_Read`로 레지스터 주소를 지정해서 한 번에 읽기
  (시간 0x00~0x02, 날짜 0x03~0x06)
- 초가 바뀔 때만 RTC에서 시간을, 시가 바뀔 때만 날짜를 다시 읽어서 I2C 통신을 줄임

### I2C LCD 드라이버
- PCF8574 모듈을 통해 **4비트 모드**로 HD44780 LCD 제어
- 한 바이트를 상위 4비트, 하위 4비트로 나눠 보내고, EN 핀을 High → Low로 바꿔 값을 확정
- 초기화 순서: `0x30` ×3 → `0x20` (4비트 모드) → Function set, Display on 등
- `Lcd_gotoXY(row, col)`: 2행은 DDRAM 주소 0x40부터 시작

### 출력 분리
`ap_main`은 TimeClock / StopWatch에서 데이터 포인터를 받아 `Display`에 넘기기만 합니다.
화면에 어떻게 그릴지는 `Display` 모듈이 정합니다.
