/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "software_timer.h"
#include "led_7seg.h"
#include "button.h"
#include "lcd.h"
#include "picture.h"
#include "ds3231.h"
#include "timer.h"
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
uint8_t count_led_debug = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void system_init();
void test_LedDebug();
void displayTime();
void updateTime();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_TIM2_Init();
  MX_SPI1_Init();
  MX_FSMC_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  system_init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
 lcd_Clear(BLACK);
 updateTime();
 uint8_t currentMode = 0;
 uint8_t confirmChangeTime = 0;
 uint8_t setAlarmFlag = 0;
 uint8_t holdCounter = 0;
 enum timeElements currentEle = SEC;
 enum timeElements nextEle = currentEle;
 char *modes[] = { "NORMAL", "MODIFY", "SCHEDULE"};
  while (1)
  {
	  while(!flag_timer2);
	  flag_timer2 = 0;
	  button_Scan();
	  ds3231_ReadTime();
	  if (button_count[0]%20 == 1 ) {
		  currentMode = (currentMode + 1) % 3;
		  confirmChangeTime = 1;
	  }
	  if (currentMode != 0) {
		  if (confirmChangeTime) {
			  currentEle = SEC;
			  nextEle = currentEle;
			  storeTime();
			  confirmChangeTime = 0;
		  }
			  if (button_count[3]%20 == 1) {
				  holdCounter = 0;
				  switch (currentEle){
				  case SEC:
					  timeElements[6]++;
					  if (timeElements[6] >= 60) {
						  timeElements[6] = 0;
					  }
					  nextEle = MIN;
					  break;
				  case MIN:
					  timeElements[5]++;
					  if (timeElements[5] >= 60) {
						  timeElements[5] = 0;
					  }
					  nextEle = HOUR;
					  break;
				  case HOUR:
					  timeElements[4]++;
					  if (timeElements[4] >= 24) {
						  timeElements[4] = 0;
					  }
					  nextEle = DAY;
					  break;
				  case DAY:
					  timeElements[3]++;
					  if (timeElements[3] >= 8) {
						  timeElements[3] = 1;
					  }
					  nextEle = DATE;
					  break;
				  case DATE:
					  timeElements[2]++;
					  if (timeElements[2] >= 32) {
						  timeElements[2] = 1;
					  }
					  nextEle = MONTH;
					  break;
				  case MONTH:
					  timeElements[1]++;
					  if (timeElements[1] >= 13) {
						  timeElements[1] = 1;
					  }
					  nextEle = YEAR;
					  break;
				  case YEAR:
					  timeElements[0]++;
					  if (timeElements[0] >= 100) {
						  timeElements[0] = 1;
					  }
					  break;
				  }

			  }
			  if (button_count[3] >= 40) { // Long press behavior
				  holdCounter++;
				  if (holdCounter >= 4) { // 4 * 50ms = 200ms
					  holdCounter = 0; // Reset hold counter
					  switch (currentEle) {
						  case SEC:
							  timeElements[6]++;
							  if (timeElements[6] >= 60) timeElements[6] = 0;
							  break;
						  case MIN:
							  timeElements[5]++;
							  if (timeElements[5] >= 60) timeElements[5] = 0;
							  break;
						  case HOUR:
							  timeElements[4]++;
							  if (timeElements[4] >= 24) timeElements[4] = 0;
							  break;
						  case DAY:
							  timeElements[3]++;
							  if (timeElements[3] >= 8) timeElements[3] = 1;
							  break;
						  case DATE:
							  timeElements[2]++;
							  if (timeElements[2] >= 32) timeElements[2] = 1;
							  break;
						  case MONTH:
							  timeElements[1]++;
							  if (timeElements[1] >= 13) timeElements[1] = 1;
							  break;
						  case YEAR:
							  timeElements[0]++;
							  if (timeElements[0] >= 100) timeElements[0] = 1;
							  break;
					  }
				  }
			  }
			  if (button_count[12]%20 == 1) {
				  if (currentMode != 2) {
					  updateTime();
				  }
				  if (nextEle == currentEle && currentEle != YEAR) {
					  switch(currentEle) {
					  case SEC:
						  nextEle = MIN;
						  break;
					  case MIN:
						  nextEle = HOUR;
						  break;
					  case HOUR:
						  nextEle = DAY;
						  break;
					  case DAY:
						  nextEle = DATE;
						  break;
					  case DATE:
						  nextEle = MONTH;
						  break;
					  case MONTH:
						  nextEle = YEAR;
						  break;

					  }
				  }

				  if (currentEle == YEAR) {
					  if (currentMode == 2) {
						  setAlarmFlag = 1;
						  for (int i = 0; i < 7; i++){
							  scheduleElements[i] = timeElements[i];
						  }
					  }
					  confirmChangeTime = 1;
					  currentMode = 0;

				  }
				  currentEle = nextEle;


			  }

			  timeEdit(currentEle);

	  } else displayTime();

	  lcd_ShowStr(30, 50, modes[currentMode], WHITE, RED, 24,0);
	  if (setAlarmFlag) {
		  lcd_ShowStr(30, 185, "alarm at" , WHITE, RED, 24,0);
		  lcd_ShowIntNum(130, 185, 2000 + scheduleElements[0], 2, GREEN, BLACK, 24);
		  lcd_ShowStr(160, 185, "year" , WHITE, RED, 24,0);
		  lcd_ShowIntNum(130, 210, scheduleElements[1], 2, GREEN, BLACK, 24);
		  lcd_ShowStr(160, 210, "month" , WHITE, RED, 24,0);
		  lcd_ShowIntNum(130, 235, scheduleElements[2], 2, GREEN, BLACK, 24);
		  lcd_ShowStr(160, 235, "date" , WHITE, RED, 24,0);
		  lcd_ShowStr(160, 260, (scheduleElements[3] == 1) ? "MONDAY"
				  	  	  : (scheduleElements[3] == 2) ? "TUESDAY"
						  : (scheduleElements[3] == 3) ? "WEDNESDAY"
						  :(scheduleElements[3] == 4) ? "THURSDAY"
						  : (scheduleElements[3] == 5) ? "FRIDAY"
						  :(scheduleElements[3] == 6) ? "SATURDAY"
						  : "SUNDAY", WHITE, RED, 24,0);
		  lcd_ShowIntNum(20, 260, scheduleElements[4], 2, GREEN, BLACK, 24);
		  lcd_ShowStr(50, 260, ":" , WHITE, RED, 24,0);
		  lcd_ShowIntNum(65, 260, scheduleElements[5], 2, GREEN, BLACK, 24);
		  lcd_ShowStr(100, 260, ":" , WHITE, RED, 24,0);
		  lcd_ShowIntNum(110, 260, scheduleElements[6], 2, GREEN, BLACK, 24);
		  if (checkAlarm()) setAlarmFlag = 0;
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void system_init(){
	  HAL_GPIO_WritePin(OUTPUT_Y0_GPIO_Port, OUTPUT_Y0_Pin, 0);
	  HAL_GPIO_WritePin(OUTPUT_Y1_GPIO_Port, OUTPUT_Y1_Pin, 0);
	  HAL_GPIO_WritePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin, 0);
	  timer_init();
	  led7_init();
	  button_init();
	  lcd_init();
	  ds3231_init();
	  setTimer2(50);
}

void test_LedDebug(){
	count_led_debug = (count_led_debug + 1)%20;
	if(count_led_debug == 0){
		HAL_GPIO_TogglePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin);
	}
}

void test_7seg(){
	led7_SetDigit(0, 0, 0);
	led7_SetDigit(5, 1, 0);
	led7_SetDigit(4, 2, 0);
	led7_SetDigit(7, 3, 0);
}
void test_button(){
	for(int i = 0; i < 16; i++){
		if(button_count[i] == 1){
			led7_SetDigit(i/10, 2, 0);
			led7_SetDigit(i%10, 3, 0);
		}
	}
}

void updateTime(){
	ds3231_Write(ADDRESS_YEAR, timeElements[0]);
	ds3231_Write(ADDRESS_MONTH, timeElements[1]);
	ds3231_Write(ADDRESS_DATE, timeElements[2]);
	ds3231_Write(ADDRESS_DAY, timeElements[3]);
	ds3231_Write(ADDRESS_HOUR, timeElements[4]);
	ds3231_Write(ADDRESS_MIN, timeElements[5]);
	ds3231_Write(ADDRESS_SEC, timeElements[6]);
}

uint8_t isButtonUp()
{
    if (button_count[3] == 1)
        return 1;
    else
        return 0;
}
uint8_t isButtonDown()
{
    if (button_count[7] == 1)
        return 1;
    else
        return 0;
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
