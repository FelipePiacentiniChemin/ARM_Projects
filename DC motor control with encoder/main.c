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
#include "tim.h"
#include "usart.h"
#include "gpio.h"

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
/* USER CODE BEGIN PFP */

void setMotor(int dir, int pwmVal);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define DELAY 100
#define TAMANHO 1000
#define FUROS 6

#define MAX 4096
#define MIN 1900
#define KD 0.005

int contador;
float contador_rpm;

float setpoint = 0000;
float rpm;
float angulo;

float pwm = MIN;

float integral;

int teste;
int controle = 1;
int temp;

uint8_t tela = '4';
uint8_t menu[70] = "\033[2J\r\tMENU \n\r1. Controle \n\r2. Medir angulo \r\n3. Medir rotacoes";
uint8_t comando[1];
uint8_t set[1];
uint8_t key[2];
uint8_t mensagem[TAMANHO];
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
  MX_TIM2_Init();
  MX_TIM10_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	HAL_TIM_Base_Start_IT(&htim10);

	HAL_UART_Transmit(&huart2, menu, strlen(menu), DELAY);
	HAL_UART_Receive_IT(&huart2, comando, 1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {

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
  RCC_OscInitStruct.PLL.PLLN = 84;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	comando[0] = toupper(comando[0]);

	if(teste != 0){
		HAL_UART_Transmit(&huart2, set, 1, DELAY);
		if(teste == 1)temp = 0;
		temp = (temp * 10) + (set[0]-'0');
	}
	if(teste == 4)teste++;

	if (key[0] == '4') {
		comando[0] = key[0];
		key[0] = '0';
	}

	switch (comando[0]) {
	case '1':
		if(tela == '4'){
			snprintf(mensagem, TAMANHO, "\033[2J\r1. Controle do RPM \n\r2. Controle do PWM");
			HAL_UART_Transmit(&huart2, mensagem, strlen(mensagem), DELAY);
			tela = comando[0];
			HAL_UART_Receive_IT(&huart2, comando, 1);
		}
		else if (tela == '1' && teste==0) {
			snprintf(mensagem, TAMANHO, "\033[2J\rSETPOINT = %.0f \n\rRPM = %.2f \n\n\r1. Mudar SetPoint \n\r4. Menu", setpoint, rpm);
			HAL_UART_Transmit(&huart2, mensagem, strlen(mensagem), DELAY);
			controle = 1;
			tela = '5';
			HAL_UART_Receive_IT(&huart2, comando, 1);
		}
		else if (tela == '5' || teste>0){
			if(teste == 0){
				snprintf(mensagem, TAMANHO, "\033[2J\rDigite o novo SetPoint com 4 digitos (ex:0120)\n\n\rNovo SetPoint: ");
				HAL_UART_Transmit(&huart2, mensagem, strlen(mensagem), DELAY);
			}
			if(teste == 5){
				snprintf(mensagem, TAMANHO, "\n\n\rPressione qualquer tecla para voltar");
				HAL_UART_Transmit(&huart2, mensagem, strlen(mensagem), DELAY);
				HAL_UART_Receive_IT(&huart2, set, 1);
				teste = 0;
				setpoint = temp;
			}
			else{
				tela = comando[0];
				HAL_UART_Receive_IT(&huart2, set, 1);
				teste++;
			}
		}
		break;
	case '2':
		if (tela == '1' || tela == '6') {
			snprintf(mensagem, TAMANHO, "\033[2J\rPWM = %.0f \n\rRPM = %.2f \n\n\r1. Aumentar PWM \n\r2. Diminuir PWM \n\r4. Menu", pwm, rpm);
			HAL_UART_Transmit(&huart2, mensagem, strlen(mensagem), DELAY);
			tela = '6';
			controle = 0;
			if (key[0] == '1') {
				pwm += 10;
				key[0] = '0';
			}
			else if (key[0] == '2') {
				pwm -= 10;
				key[0] = '0';
			}
			setMotor(1, pwm);
			HAL_UART_Receive_IT(&huart2, key, 1);
		}
		else if (tela == '4') {
			snprintf(mensagem, TAMANHO,
					"\033[2J\rAngulo = %.2f \n\r4. Menu \n\rR. Reset", angulo);
			HAL_UART_Transmit(&huart2, mensagem, strlen(mensagem), DELAY);

			tela = comando[0];
			HAL_UART_Receive_IT(&huart2, comando, 1);
		}
		break;
	case '3':
		snprintf(mensagem, TAMANHO, "\033[2J\rRotacoes = %i \n\r4. Menu \n\rR. Reset", contador / FUROS);
		HAL_UART_Transmit(&huart2, mensagem, strlen(mensagem), DELAY);

		tela = comando[0];
		HAL_UART_Receive_IT(&huart2, comando, 1);
		break;
	case '4':
		HAL_UART_Transmit(&huart2, menu, strlen(menu), DELAY);
		tela = comando[0];
		HAL_UART_Receive_IT(&huart2, comando, 1);
		break;
	case 'R':
		if(tela == '2'){
			angulo = 0;
			snprintf(mensagem, TAMANHO, "\033[2J\rAngulo = %.2f \n\r4. Menu \n\rR. Reset", angulo);
			HAL_UART_Transmit(&huart2, mensagem, strlen(mensagem), DELAY);
			HAL_UART_Receive_IT(&huart2, comando, 1);
		}
		else if(tela == '3'){
			contador = 0;
			snprintf(mensagem, TAMANHO, "\033[2J\rRotacoes = %i \n\r4. Menu \n\rR. Reset", contador / FUROS);
			HAL_UART_Transmit(&huart2, mensagem, strlen(mensagem), DELAY);
			HAL_UART_Receive_IT(&huart2, comando, 1);
		}
		break;
	default:
		HAL_UART_Receive_IT(&huart2, comando, 1);
		break;
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM10) {
		rpm = (contador_rpm / FUROS) * 60;
		contador_rpm = 0;

		if(tela == '5'){
			snprintf(mensagem, TAMANHO, "\033[2J\rSETPOINT = %.0f \n\rRPM = %.2f \n\n\r1. Mudar SetPoint \n\r4. Menu", setpoint, rpm);
			HAL_UART_Transmit(&huart2, mensagem, strlen(mensagem), DELAY);
		}
		if(tela == '6'){
			snprintf(mensagem, TAMANHO, "\033[2J\rPWM = %.0f \n\rRPM = %.2f \n\n\r1. Aumentar PWM \n\r2. Diminuir PWM \n\r4. Menu", pwm, rpm);
			HAL_UART_Transmit(&huart2, mensagem, strlen(mensagem), DELAY);
		}
		float KP = 0.1;
		float KI = 0.2;

		if (controle == 1 && setpoint < 200) {
			setMotor(0, 0);
			integral = 0;
		}
		else if(controle == 1){
			float e = setpoint - rpm;

			integral = integral + (e * 1);
			if ((KI * integral) > MAX) {
				integral = MAX / KI;
			}

			float u = (KP * e) + (KI * integral) + MIN;

			int dir = 1;
			if (u < 0) {
				dir = -1;
			}

			pwm = (int) fabs(u);

			if (pwm > MAX) {
				pwm = MAX;
			}
			else if (pwm < MIN) {
				pwm = MIN;
			}

			setMotor(dir, pwm);
		}
		__HAL_TIM_CLEAR_FLAG(&htim10, TIM_FLAG_UPDATE);
	}

}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM2) {
		if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) {
			contador_rpm++;
			contador++;

			angulo += 360 / FUROS;
			if (angulo > 360)angulo -= 360;

			if(tela == '2'){
				snprintf(mensagem, TAMANHO, "\033[2J\rAngulo = %.2f \n\r4. Menu \n\rR. Reset", angulo);
				HAL_UART_Transmit(&huart2, mensagem, strlen(mensagem), DELAY);
			}

			if (tela == '3') {
				snprintf(mensagem, TAMANHO, "\033[2J\rRotacoes = %i \n\r4. Menu \n\rR. Reset", contador / FUROS);
				HAL_UART_Transmit(&huart2, mensagem, strlen(mensagem), DELAY);
			}

		}
	}
}

void setMotor(int dir, int pwm){
  if(dir == 1){
	  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, pwm);
	  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0);
  }
  else if(dir == -1){
	  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
	  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, pwm);
  }
  else{
	  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
	  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0);
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
	while (1) {
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
