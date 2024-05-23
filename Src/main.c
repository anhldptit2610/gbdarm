/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "dma.h"
#include "fatfs.h"
#include "sdmmc.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
// #include "Kirby_Dream_Land.gb.h"
// #include "Super_Mario_Land.gb.h"
#include "gbdarm.h"
#include "ili9225.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define FRAME_BUFFER      __attribute__((section(".frame_buffer"), used))
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
FRAME_BUFFER uint16_t frontFrameBuffer[SCREEN_HEIGHT * SCREEN_WIDTH];
FRAME_BUFFER uint16_t backFrameBuffer[SCREEN_HEIGHT * SCREEN_WIDTH];
uint16_t backgroundBuffer[LCD_HEIGHT * LCD_WIDTH];
// FRAME_BUFFER uint16_t frontFrameBuffer[LCD_HEIGHT * LCD_WIDTH];
// FRAME_BUFFER uint16_t backFrameBuffer[LCD_HEIGHT * LCD_WIDTH];
struct gb gb;
uint8_t rom[256 * KiB];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void system_init(void)
{
  for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
  // for (int i = 0; i < LCD_WIDTH * LCD_HEIGHT; i++)
    frontFrameBuffer[i] = backFrameBuffer[i] = ILI9225_WHITE;
  for (int i = 0; i < LCD_WIDTH * LCD_HEIGHT; i++) {
    backgroundBuffer[i] = ILI9225_BLACK;
  }
}

void rom_load(void)
{
  FATFS fatFS;
  FIL fp;
  FRESULT fResult;
  FATFS *fsPtr;
  UINT RWC, WWC;

  fResult = f_mount(&fatFS, SDPath, 1);
  fResult = f_open(&fp, "Dr_Mario.gb", FA_READ);
  f_read(&fp, rom, f_size(&fp), &RWC);
  f_close(&fp);
  fResult = f_mount(NULL, "", 0);
}

void joypad_check(void)
{
  bool left, right, up, down, select, start, a, b;

  left = gb.joypad.button[JOYPAD_LEFT]; 
  right = gb.joypad.button[JOYPAD_RIGHT]; 
  up = gb.joypad.button[JOYPAD_UP]; 
  down = gb.joypad.button[JOYPAD_DOWN]; 
  select = gb.joypad.button[JOYPAD_SELECT];
  start = gb.joypad.button[JOYPAD_START];
  a = gb.joypad.button[JOYPAD_A];
  b = gb.joypad.button[JOYPAD_B];
  gb.joypad.button[JOYPAD_LEFT] = HAL_GPIO_ReadPin(JOYPAD_LEFT_GPIO_Port, JOYPAD_LEFT_Pin);
  gb.joypad.button[JOYPAD_RIGHT] = HAL_GPIO_ReadPin(JOYPAD_RIGHT_GPIO_Port, JOYPAD_RIGHT_Pin);
  gb.joypad.button[JOYPAD_UP] = HAL_GPIO_ReadPin(JOYPAD_UP_GPIO_Port, JOYPAD_UP_Pin);
  gb.joypad.button[JOYPAD_DOWN] = HAL_GPIO_ReadPin(JOYPAD_DOWN_GPIO_Port, JOYPAD_UP_Pin);
  gb.joypad.button[JOYPAD_SELECT] = HAL_GPIO_ReadPin(JOYPAD_SELECT_GPIO_Port, JOYPAD_SELECT_Pin);
  gb.joypad.button[JOYPAD_START] = HAL_GPIO_ReadPin(JOYPAD_START_GPIO_Port, JOYPAD_START_Pin);
  gb.joypad.button[JOYPAD_A] = HAL_GPIO_ReadPin(JOYPAD_A_GPIO_Port, JOYPAD_A_Pin);
  gb.joypad.button[JOYPAD_B] = HAL_GPIO_ReadPin(JOYPAD_B_GPIO_Port, JOYPAD_B_Pin);

  // request interrupt if any button is pressed down
  if (IS_FALLING_EDGE(left, gb.joypad.button[JOYPAD_LEFT]) || IS_FALLING_EDGE(right, gb.joypad.button[JOYPAD_RIGHT]) ||
      IS_FALLING_EDGE(up, gb.joypad.button[JOYPAD_UP]) || IS_FALLING_EDGE(down, gb.joypad.button[JOYPAD_DOWN]) ||
      IS_FALLING_EDGE(select, gb.joypad.button[JOYPAD_SELECT]) || IS_FALLING_EDGE(start, gb.joypad.button[JOYPAD_START]) ||
      IS_FALLING_EDGE(a, gb.joypad.button[JOYPAD_A]) || IS_FALLING_EDGE(b, gb.joypad.button[JOYPAD_B])) {
    gb.interrupt.flag |= INTERRUPT_SRC_JOYPAD;
  }
}

static void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();
	
	/* Configure the MPU attributes for the QSPI 256MB without instruction access */
  MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
  MPU_InitStruct.Number           = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress      = QSPI_BASE;
  MPU_InitStruct.Size             = MPU_REGION_SIZE_256MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.IsBufferable     = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable      = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_DISABLE;
  MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL1;
  MPU_InitStruct.SubRegionDisable = 0x00;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);
	
  /* Configure the MPU attributes for the QSPI 8MB (QSPI Flash Size) to Cacheable WT */
  MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
  MPU_InitStruct.Number           = MPU_REGION_NUMBER1;
  MPU_InitStruct.BaseAddress      = QSPI_BASE;
  MPU_InitStruct.Size             = MPU_REGION_SIZE_8MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_PRIV_RO;
  MPU_InitStruct.IsBufferable     = MPU_ACCESS_BUFFERABLE;
  MPU_InitStruct.IsCacheable      = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_ENABLE;
  MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL1;
  MPU_InitStruct.SubRegionDisable = 0x00;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* configure the D2 RAM region, make it non-cacheable*/
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0x30000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_128KB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);
	
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

static void CPU_CACHE_Enable(void)
{
  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
  ili9225_set_cs(STATE_DISABLE);
  gb.whichBuffer = (gb.whichBuffer == FRONT) ? BACK : FRONT;
  gb.frontBufferPtr = (gb.whichBuffer == FRONT) ? frontFrameBuffer : backFrameBuffer;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	
  #ifdef W25Qxx
    SCB->VTOR = QSPI_BASE;
  #endif
  MPU_Config();
  CPU_CACHE_Enable();
	
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
  MX_SPI4_Init();
  MX_TIM1_Init();
  MX_SDMMC1_SD_Init();
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */
  gb.frontBufferPtr = frontFrameBuffer;
  gb.backBufferPtr = backFrameBuffer;
  gb.whichBuffer = FRONT;
  system_init();
  rom_load();
  ili9225_init();
  cartridge_load(&gb, rom);
  load_state_after_booting(&gb);
  // ili9225_set_gram_ptr(153, 0);
  ili9225_set_gram_ptr(0, 0);
  ili9225_draw_bitmap(backgroundBuffer, LCD_HEIGHT, LCD_WIDTH, PLAINSPI);
  ili9225_set_window_area(0, 159, 9, 153);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    // ili9225_draw_bitmap(gb.frontBufferPtr, LCD_HEIGHT, LCD_WIDTH, DMA);
    ili9225_draw_bitmap(gb.frontBufferPtr, SCREEN_WIDTH, SCREEN_HEIGHT, DMA);
    while (!gb.ppu.frameReady)
      cpu_step(&gb);
    joypad_check();
    if (gb.whichBuffer == BACK) {
      gb.backBufferPtr = frontFrameBuffer;
    } else if (gb.whichBuffer == FRONT) {
      gb.backBufferPtr = backFrameBuffer;
    }
    // ili9225_set_gram_ptr(0, 0);
    ili9225_set_gram_ptr(153, 0);
    gb.ppu.frameReady = false;
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

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
