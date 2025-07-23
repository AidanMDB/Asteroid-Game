/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "crc.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <sprites.h>
#include "LCD_Driver.h"
#include "graphics.h"
#include "game.h"
#include "stdio.h"
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
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define DEBOUNCE_DELAY 100
volatile uint8_t left_pressed = 0;
volatile uint8_t right_pressed = 0;
volatile uint8_t shoot_pressed = 0;
volatile uint32_t last_right = 0;
volatile uint32_t last_left = 0;
volatile uint32_t last_shoot = 0;
uint16_t movement_delay = 100;
uint32_t last_move = 0;
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
  MX_DMA_Init();
  MX_SPI2_Init();
  MX_TIM15_Init();
  MX_CRC_Init();
  /* USER CODE BEGIN 2 */

  LCD_Init();


  game_init();
  //volatile int counter = 0;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	update_score_board();
	increase_score();



	if (left_pressed) {
		uint32_t now = HAL_GetTick();
		if (now - last_move >= movement_delay) {
			move(-1);
			last_move = now;
		}
	}


	if (right_pressed) {
		uint32_t now = HAL_GetTick();
		if (now - last_move >= movement_delay) {
			move(1);
			last_move = now;
		}
	}


	if (shoot_pressed == 1) {
		shoot_pressed = 0;
		shoot_laser();
	}

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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {

	uint32_t now = HAL_GetTick();

	// turns out gpio user labels are defined in main.h not gpio.h
	// Inside an interrupt context, interrupts are usually disabled or of lower priority.
	// The DMA complete callback (which sets lcd_dma_ready = true) happens in an interrupt.
	// Since interrupts are disabled while servicing GPIO EXTI, the DMA complete interrupt never runs.
	// So I need to do these flags

	switch (GPIO_Pin) {

		// set flags then in the main loop call the move() function
		case RIGHT_PUSH_Pin:
			// toggle for visual debugging
			HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);

			// verify debounce then check if button was pushed or released
			if (now - last_right >= DEBOUNCE_DELAY) {
				last_right = now;
				if (HAL_GPIO_ReadPin(RIGHT_PUSH_GPIO_Port, RIGHT_PUSH_Pin) == GPIO_PIN_SET) {
					right_pressed = 1;
				}
				else {
					right_pressed = 0;
				}
			}

			break;

		case LEFT_PUSH_Pin:
			HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);


			if (now - last_left >= DEBOUNCE_DELAY) {
				last_left = now;
				if (HAL_GPIO_ReadPin(LEFT_PUSH_GPIO_Port, LEFT_PUSH_Pin) == GPIO_PIN_SET) {
					left_pressed = 1;
				}
				else {
					left_pressed = 0;
				}
			}
			break;


		case SHOOT_PUSH_Pin:
			HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
			if (now - last_shoot > DEBOUNCE_DELAY) {
				last_shoot = now;
				shoot_pressed = 1;
			}
			break;

		default:
			break;

	}
}
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

#ifdef  USE_FULL_ASSERT
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
