#include "bsp.h"
#ifdef __GNUC__
  #pragma GCC diagnostic ignored "-Wunused-variable"
#endif

void _Error_Handler(char* file, uint32_t line);
static uint32_t FSMC_Initialized = 0;

//��ʼ��lcd FSMC
SRAM_HandleTypeDef fsmcLcdHandle;

void STM_FSMC_LCD_TimeSet(uint8_t _as, uint8_t _ds)
{ 	
 
  GPIO_InitTypeDef GPIO_InitStruct;
  FSMC_NORSRAM_TimingTypeDef Timing;
  if (FSMC_Initialized) {
    return;
  }
  FSMC_Initialized = 1;
  /* Peripheral clock enable */
  /* Peripheral clock enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_FSMC_CLK_ENABLE();
  
 
  /** FSMC GPIO Configuration  
  PE7   ------> FSMC_D4
  PE8   ------> FSMC_D5
  PE9   ------> FSMC_D6
  PE10   ------> FSMC_D7
  PE11   ------> FSMC_D8
  PE12   ------> FSMC_D9
  PE13   ------> FSMC_D10
  PE14   ------> FSMC_D11
  PE15   ------> FSMC_D12
  PD8   ------> FSMC_D13
  PD9   ------> FSMC_D14
  PD10   ------> FSMC_D15
  PD13   ------> FSMC_A18
  PD14   ------> FSMC_D0
  PD15   ------> FSMC_D1
  PD0   ------> FSMC_D2
  PD1   ------> FSMC_D3
  PD4   ------> FSMC_NOE
  PD5   ------> FSMC_NWE
  PD7   ------> FSMC_NE1
  */
  /* GPIO_InitStruct */
  GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10 
                          |GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14 
                          |GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FSMC;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /* GPIO_InitStruct */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_13 
                          |GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_0|GPIO_PIN_1 
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FSMC;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /** Perform the SRAM1 memory initialization sequence
  */
  fsmcLcdHandle.Instance = FSMC_NORSRAM_DEVICE;
  fsmcLcdHandle.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
  /* fsmcLcdHandle.Init */
  fsmcLcdHandle.Init.NSBank = FSMC_NORSRAM_BANK1;
  fsmcLcdHandle.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
  fsmcLcdHandle.Init.MemoryType = FSMC_MEMORY_TYPE_SRAM;
  fsmcLcdHandle.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
  fsmcLcdHandle.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
  fsmcLcdHandle.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
  fsmcLcdHandle.Init.WrapMode = FSMC_WRAP_MODE_DISABLE;
  fsmcLcdHandle.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;
  fsmcLcdHandle.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
  fsmcLcdHandle.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
  fsmcLcdHandle.Init.ExtendedMode = FSMC_EXTENDED_MODE_DISABLE;
  fsmcLcdHandle.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
  fsmcLcdHandle.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;
  fsmcLcdHandle.Init.PageSize = FSMC_PAGE_SIZE_NONE;
  
  /* Timing  stm324xG-eval time*/
  Timing.AddressSetupTime      = _as/6;	  //  6ns(1/168M)*2(HCLK) = 12ns	
  Timing.AddressHoldTime       = 0;       //  FSMC_ACCESS_MODE_A unused 
  Timing.DataSetupTime         = _ds/6;   //  6ns(1/168M)* 5 (HCLK)=30ns
  Timing.BusTurnAroundDuration = 0;
  Timing.CLKDivision           = 0;
  Timing.DataLatency           = 0;
  Timing.AccessMode            = FSMC_ACCESS_MODE_A;
  /* ExtTiming */

  if (HAL_SRAM_Init(&fsmcLcdHandle, &Timing, NULL) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

#ifndef  LCD_ADDR_SETUPTIME
# define LCD_ADDR_SETUPTIME 20
#endif
#ifndef  LCD_DATA_SETUPTIME
# define LCD_DATA_SETUPTIME 45
#endif

void STM_FSMC_LCD_Init(void)
{
	STM_FSMC_LCD_TimeSet(LCD_ADDR_SETUPTIME, LCD_DATA_SETUPTIME);
#ifdef LCDBL_PIN
    pinMode(LCDBL_PIN,OUTPUT);
# ifdef LCDBL_ON
	digitalWrite(LCDBL_PIN, LCDBL_ON); //backlight on if can set
# else	
	digitalWrite(LCDBL_PIN, HIGH); //backlight on if can set
# endif	
#endif	
}
