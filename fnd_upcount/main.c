/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void fnd_on(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	  HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);
}

void fnd_off(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	  HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
}

void fnd_0(void)
{
	fnd_on(GPIOA, GPIO_PIN_6); //A
	fnd_on(GPIOA, GPIO_PIN_9); //B
	fnd_on(GPIOB, GPIO_PIN_5); //C
	fnd_on(GPIOB, GPIO_PIN_10); //D
	fnd_on(GPIOA, GPIO_PIN_8); //E
	fnd_on(GPIOA, GPIO_PIN_7); //F
	fnd_off(GPIOB, GPIO_PIN_3); //G
	fnd_off(GPIOB, GPIO_PIN_4); //DP
}

void fnd_1(void)
{
	fnd_off(GPIOA, GPIO_PIN_6); //A
	fnd_on(GPIOA, GPIO_PIN_9); //B
	fnd_on(GPIOB, GPIO_PIN_5); //C
	fnd_off(GPIOB, GPIO_PIN_10); //D
	fnd_off(GPIOA, GPIO_PIN_8); //E
	fnd_off(GPIOA, GPIO_PIN_7); //F
	fnd_off(GPIOB, GPIO_PIN_3); //G
	fnd_off(GPIOB, GPIO_PIN_4); //DP
}

void fnd_2(void)
{
	fnd_on(GPIOA, GPIO_PIN_6); //A
	fnd_on(GPIOA, GPIO_PIN_9); //B
	fnd_off(GPIOB, GPIO_PIN_5); //C
	fnd_on(GPIOB, GPIO_PIN_10); //D
	fnd_on(GPIOA, GPIO_PIN_8); //E
	fnd_off(GPIOA, GPIO_PIN_7); //F
	fnd_on(GPIOB, GPIO_PIN_3); //G
	fnd_off(GPIOB, GPIO_PIN_4); //DP
}

void fnd_3(void)
{
	fnd_on(GPIOA, GPIO_PIN_6); //A
	fnd_on(GPIOA, GPIO_PIN_9); //B
	fnd_on(GPIOB, GPIO_PIN_5); //C
	fnd_on(GPIOB, GPIO_PIN_10); //D
	fnd_off(GPIOA, GPIO_PIN_8); //E
	fnd_off(GPIOA, GPIO_PIN_7); //F
	fnd_on(GPIOB, GPIO_PIN_3); //G
	fnd_off(GPIOB, GPIO_PIN_4); //DP
}

void fnd_4(void)
{
	fnd_off(GPIOA, GPIO_PIN_6); //A
	fnd_on(GPIOA, GPIO_PIN_9); //B
	fnd_on(GPIOB, GPIO_PIN_5); //C
	fnd_off(GPIOB, GPIO_PIN_10); //D
	fnd_off(GPIOA, GPIO_PIN_8); //E
	fnd_on(GPIOA, GPIO_PIN_7); //F
	fnd_on(GPIOB, GPIO_PIN_3); //G
	fnd_off(GPIOB, GPIO_PIN_4); //DP
}

void fnd_5(void)
{
	fnd_on(GPIOA, GPIO_PIN_6); //A
	fnd_off(GPIOA, GPIO_PIN_9); //B
	fnd_on(GPIOB, GPIO_PIN_5); //C
	fnd_on(GPIOB, GPIO_PIN_10); //D
	fnd_off(GPIOA, GPIO_PIN_8); //E
	fnd_on(GPIOA, GPIO_PIN_7); //F
	fnd_on(GPIOB, GPIO_PIN_3); //G
	fnd_off(GPIOB, GPIO_PIN_4); //DP
}

void fnd_6(void)
{
	fnd_off(GPIOA, GPIO_PIN_6); //A
	fnd_off(GPIOA, GPIO_PIN_9); //B
	fnd_on(GPIOB, GPIO_PIN_5); //C
	fnd_on(GPIOB, GPIO_PIN_10); //D
	fnd_on(GPIOA, GPIO_PIN_8); //E
	fnd_on(GPIOA, GPIO_PIN_7); //F
	fnd_on(GPIOB, GPIO_PIN_3); //G
	fnd_off(GPIOB, GPIO_PIN_4); //DP
}

void fnd_7(void)
{
	fnd_on(GPIOA, GPIO_PIN_6); //A
	fnd_on(GPIOA, GPIO_PIN_9); //B
	fnd_on(GPIOB, GPIO_PIN_5); //C
	fnd_off(GPIOB, GPIO_PIN_10); //D
	fnd_off(GPIOA, GPIO_PIN_8); //E
	fnd_on(GPIOA, GPIO_PIN_7); //F
	fnd_off(GPIOB, GPIO_PIN_3); //G
	fnd_off(GPIOB, GPIO_PIN_4); //DP
}

void fnd_8(void)
{
	fnd_on(GPIOA, GPIO_PIN_6); //A
	fnd_on(GPIOA, GPIO_PIN_9); //B
	fnd_on(GPIOB, GPIO_PIN_5); //C
	fnd_on(GPIOB, GPIO_PIN_10); //D
	fnd_on(GPIOA, GPIO_PIN_8); //E
	fnd_on(GPIOA, GPIO_PIN_7); //F
	fnd_on(GPIOB, GPIO_PIN_3); //G
	fnd_off(GPIOB, GPIO_PIN_4); //DP
}

void fnd_9(void)
{
	fnd_on(GPIOA, GPIO_PIN_6); //A
	fnd_on(GPIOA, GPIO_PIN_9); //B
	fnd_on(GPIOB, GPIO_PIN_5); //C
	fnd_off(GPIOB, GPIO_PIN_10); //D
	fnd_off(GPIOA, GPIO_PIN_8); //E
	fnd_on(GPIOA, GPIO_PIN_7); //F
	fnd_on(GPIOB, GPIO_PIN_3); //G
	fnd_off(GPIOB, GPIO_PIN_4); //DP
}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET); // D1
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET); // D2
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET); // D3
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);// D4


  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET); // A
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET); // B
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET); // C
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);// D
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET); // E
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET); // F
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET); // G
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET); // DP


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  int i = 0;
  int k = 0;
  int one = 0;
  int ten = 0;
  int hun = 0;
  int tho = 0;

  while (1)
  {

	  for(i = 0; i<10000; i++)
	  {
		  one = i % 10;
	  	  ten = (i / 10) % 10;
	  	  hun = (i / 100) % 10;
	  	  tho = (i / 1000) % 10;

	  	  for(k = 0; k < 5; k ++)
	  	  {
	  		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET); // D1
	  		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET); // D2
	  		  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET); // D3
	  		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);// D4
	  		  switch(tho)
	  		    	  {
	  		    	  case 0 :
	  		    		  fnd_0();
	  		    	  break;
	  		    	  case 1 :
	  		    		  fnd_1();
	  		    	  break;
	  		    	  case 2 :
	  		    		  fnd_2();
	  		    	  break;
	  		    	  case 3 :
	  		    		  fnd_3();
	  		    	  break;
	  		    	  case 4 :
	  		    		  fnd_4();
	  		    	  break;
	  		    	  case 5 :
	  		    		  fnd_5();
	  		    	  break;
	  		    	  case 6 :
	  		    		  fnd_6();
	  		    	  break;
	  		    	  case 7 :
	  		    		  fnd_7();
	  		    	  break;
	  		    	  case 8 :
	  		    		  fnd_8();
	  		    	  break;
	  		    	  case 9 :
	  		    		  fnd_9();
	  		    	  break;
	  		    	  }
	  		  HAL_Delay(4);

	  		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET); // D1
	  		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET); // D2
	  		  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET); // D3
	  		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);// D4
	  		  switch(hun)
	  		    	  {
	  		    	  case 0 :
	  		    		  fnd_0();
	  		    	  break;
	  		    	  case 1 :
	  		    		  fnd_1();
	  		    	  break;
	  		    	  case 2 :
	  		    		  fnd_2();
	  		    	  break;
	  		    	  case 3 :
	  		    		  fnd_3();
	  		    	  break;
	  		    	  case 4 :
	  		    		  fnd_4();
	  		    	  break;
	  		    	  case 5 :
	  		    		  fnd_5();
	  		    	  break;
	  		    	  case 6 :
	  		    		  fnd_6();
	  		    	  break;
	  		    	  case 7 :
	  		    		  fnd_7();
	  		    	  break;
	  		    	  case 8 :
	  		    		  fnd_8();
	  		    	  break;
	  		    	  case 9 :
	  		    		  fnd_9();
	  		    	  break;
	  		    	  }
	  		  	  HAL_Delay(4);

	  		  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET); // D1
	  		  	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET); // D2
	  		  	  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET); // D3
	  		  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);// D4
	  		  switch(ten)
	  		    	  {
	  		    	  case 0 :
	  		    		  fnd_0();
	  		    	  break;
	  		    	  case 1 :
	  		    		  fnd_1();
	  		    	  break;
	  		    	  case 2 :
	  		    		  fnd_2();
	  		    	  break;
	  		    	  case 3 :
	  		    		  fnd_3();
	  		    	  break;
	  		    	  case 4 :
	  		    		  fnd_4();
	  		    	  break;
	  		    	  case 5 :
	  		    		  fnd_5();
	  		    	  break;
	  		    	  case 6 :
	  		    		  fnd_6();
	  		    	  break;
	  		    	  case 7 :
	  		    		  fnd_7();
	  		    	  break;
	  		    	  case 8 :
	  		    		  fnd_8();
	  		    	  break;
	  		    	  case 9 :
	  		    		  fnd_9();
	  		    	  break;
	  		    	  }
	  		  HAL_Delay(4);

	  		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET); // D1
	  		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET); // D2
	  		  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET); // D3
	  		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);// D4
	  		  switch(one)
	  		    	  {
	  		    	  case 0 :
	  		    		  fnd_0();
	  		    	  break;
	  		    	  case 1 :
	  		    		  fnd_1();
	  		    	  break;
	  		    	  case 2 :
	  		    		  fnd_2();
	  		    	  break;
	  		    	  case 3 :
	  		    		  fnd_3();
	  		    	  break;
	  		    	  case 4 :
	  		    		  fnd_4();
	  		    	  break;
	  		    	  case 5 :
	  		    		  fnd_5();
	  		    	  break;
	  		    	  case 6 :
	  		    		  fnd_6();
	  		    	  break;
	  		    	  case 7 :
	  		    		  fnd_7();
	  		    	  break;
	  		    	  case 8 :
	  		    		  fnd_8();
	  		    	  break;
	  		    	  case 9 :
	  		    		  fnd_9();
	  		    	  break;
	  		    	  }
	  		  HAL_Delay(4);
	  	  }
	  }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8
                          |GPIO_PIN_9|GPIO_PIN_10, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5
                          |GPIO_PIN_6, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA5 PA6 PA7 PA8
                           PA9 PA10 */
  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8
                          |GPIO_PIN_9|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB10 PB3 PB4 PB5
                           PB6 */
  GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5
                          |GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PC7 */
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
