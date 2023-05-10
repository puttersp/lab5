/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stdio.h"
#include "string.h"
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
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
uint8_t rx_buf[1];
uint8_t tx_buf[100];
float led_spd = 50;
uint8_t state = 0;
uint8_t action = 1;
uint8_t led_off = 0;
uint8_t button_state = 0;
uint8_t ledfreq = 10;
uint8_t startup = 0;
uint16_t led_buf[2];
uint8_t str[110];
uint8_t str2[110];
static uint32_t timestamp2 = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
void uart_poll();
void led();
void uart_it_conf();
void fsm();
void clrtx();
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
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  sprintf(str, "\n\n\n\n\n\n\n\n\n\n\rAvailable Options\r\na : Speed Up +1Hz\r\ns : Speed Down -1Hz\r\nd : On/Off\r\nx : Back\r\nLED Frequency : %d", ledfreq);
//  uint8_t text[] = "\r\nHELLO FIBO\r\n";
//  HAL_UART_Transmit(&huart2, text, 13, 10);
  //uart_it_conf();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  //uart_poll();

	  HAL_UART_Receive_IT(&huart2, rx_buf, 1);
	  led();
	  fsm();


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
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 57600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){action = 0;}
void fsm(){
	switch(state){
	case 0:
		//sprintf base menu
		if(startup == 0){
			HAL_UART_Transmit_IT(&huart2,"\n\n\n\n\n\n\n\n\n\n\rAvailable Options\r\n0 : LED Control\r\n1 : Button Status\r\n", 66);
			startup = 1;
		}
		if(action == 0 && rx_buf[0] == '0'){
			action = 1;
			HAL_UART_Transmit_IT(&huart2,"\n\n\n\n\n\n\n\n\n\n\rAvailable Options\r\na : Speed Up +1Hz\r\ns : Speed Down -1Hz\r\nd : On/Off\r\nx : Back\r\n", 92);
			state = 1;
		}
		else if(action == 0 && rx_buf[0] == '1'){
			action = 1;
			HAL_UART_Transmit_IT(&huart2,"\n\n\n\n\n\n\n\n\n\n\rAvailable Options\r\nx : Back\r\n", 40);
			state = 2;
		}
		else if(action == 0 && rx_buf[0] != '1' && rx_buf[0] != '0'){
			action = 1;
			//HAL_UART_Transmit_IT(&huart2,"\n\n\n\n\n\n\n\n\n\n\rAvailable Options\r\n0 : LED Control\r\n1 : Button Status\r\n", 66);
			HAL_UART_Transmit_IT(&huart2,"Wrong Button Pressed!\r\n", 23);
		}
		break;
	case 1:
		//led control menu
		if(action == 0 && rx_buf[0] == 'x'){
			action = 1;
			state = 0;
			HAL_UART_Transmit_IT(&huart2,"\n\n\n\n\n\n\n\n\n\n\rAvailable Options\r\n0 : LED Control\r\n1 : Button Status\r\n", 66);
		}
		else if(action == 0 && rx_buf[0] == 'a'){
			action = 1;
			led_spd = (500*led_spd)/(500+led_spd);
			ledfreq = ceil(500/led_spd);
			sprintf(str, "\n\n\n\n\n\n\n\n\n\n\rAvailable Options\r\na : Speed Up +1Hz\r\ns : Speed Down -1Hz\r\nd : On/Off\r\nx : Back\r\nLED Frequency : %d", ledfreq);
			//HAL_UART_Transmit_IT(&huart2,"\n\n\n\n\n\n\n\n\n\n\rAvailable Options\r\na : Speed Up +1Hz\r\ns : Speed Down -1Hz\r\nd : On/Off\r\nx : Back\r\nLED Frequency : ", 108);
			HAL_UART_Transmit_IT(&huart2, str, 110);

		}
		else if(action == 0 && rx_buf[0] == 's'){
			action = 1;
			led_spd = (500*led_spd)/(500-led_spd);
			ledfreq = ceil(500/led_spd);
			sprintf(str, "\n\n\n\n\n\n\n\n\n\n\rAvailable Options\r\na : Speed Up +1Hz\r\ns : Speed Down -1Hz\r\nd : On/Off\r\nx : Back\r\nLED Frequency : %d", ledfreq);
			//HAL_UART_Transmit_IT(&huart2,"\n\n\n\n\n\n\n\n\n\n\rAvailable Options\r\na : Speed Up +1Hz\r\ns : Speed Down -1Hz\r\nd : On/Off\r\nx : Back\r\nLED Frequency : ", 108);
			//HAL_UART_Transmit_IT(&huart2, str, 2);
			HAL_UART_Transmit_IT(&huart2, str, 110);
		}
		else if(action == 0 && rx_buf[0] == 'd'){
			action = 1;

			if(led_off == 0){
				led_off = 1;
				HAL_UART_Transmit_IT(&huart2,"\n\n\n\n\n\n\n\n\n\n\rAvailable Options\r\na : Speed Up +1Hz\r\ns : Speed Down -1Hz\r\nd : On/Off\r\nx : Back\r\nLED Turned Off!\r", 109);
			}
			else if(led_off == 1){
				led_off = 0;
				HAL_UART_Transmit_IT(&huart2,"\n\n\n\n\n\n\n\n\n\n\rAvailable Options\r\na : Speed Up +1Hz\r\ns : Speed Down -1Hz\r\nd : On/Off\r\nx : Back\r\nLED Turned On!\r", 108);
			}
		}
		else if(action == 0 && rx_buf[0] != 'x' && rx_buf[0] != 'a' && rx_buf[0] != 's' && rx_buf[0] != 'd'){
			//sprintf no button
			action = 1;
			HAL_UART_Transmit_IT(&huart2,"Wrong Button Pressed!\r\n", 23);
		}
		break;
	case 2:
		//read button state
		//sprint show button menu
		button_state = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);
		if(HAL_GetTick()>=timestamp2){
			timestamp2 = HAL_GetTick()+250;
			if(button_state == 0){
				HAL_UART_Transmit_IT(&huart2,"\n\n\n\n\n\n\n\n\n\n\rButton Status : Pressed\r\n\nAvailable Options\r\nx : Back\r\n\r", 67);
			}
			else if(button_state == 1){
				HAL_UART_Transmit_IT(&huart2,"\n\n\n\n\n\n\n\n\n\n\rButton Status : Not Pressed\r\n\nAvailable Options\r\nx : Back\r\n\r", 71);
			}
		}

		if(action == 0 && rx_buf[0] == 'x'){
			action = 1;
			state = 0;
			HAL_UART_Transmit_IT(&huart2,"\n\n\n\n\n\n\n\n\n\n\rAvailable Options\r\n0 : LED Control\r\n1 : Button Status\r\n", 66);
		}
		else if(action == 0 && rx_buf[0] != 'x'){
			action = 1;
			HAL_UART_Transmit_IT(&huart2,"Wrong Button Pressed!\r\n", 23);
		}
		break;
	}
}

void led(){
	static uint32_t timestamp = 0;
	if(led_spd > 60000){
		led_spd = 1000;
	}
	if(HAL_GetTick()>=timestamp && led_off == 0){
		timestamp = HAL_GetTick()+led_spd;
		HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
	}
	if(led_off == 1){
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 0);

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
