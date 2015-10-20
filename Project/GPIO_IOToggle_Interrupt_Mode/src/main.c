/**
  ******************************************************************************
  * @file GPIO_IOToggle_Interrupt_Mode\main.c
  * @brief This file contains the main function for GPIO_IOToggle_Interrupt_Mode example on STM8S-Discovery.
  * @author STMicroelectronics - MCD Application Team
  * @version V1.0
  * @date 03/02/2010
  ******************************************************************************
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  * @image html logo.bmp
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "stm8s_clk.h"
#include "timer.h"
#include "button.h"

/**
  * @addtogroup GPIO_IOToggle_Interrupt_Mode
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Evalboard I/Os configuration */

#define LEDS_PORT (GPIOB)

#define LED5_PIN  (GPIO_PIN_1)

#define BUTTON_PORT (GPIOB)
#define BUTTON_PIN1  (GPIO_PIN_6)
#define BUTTON_PIN2  (GPIO_PIN_7)

/* Private macro -------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void Delay (u16 nCount);

/* Private functions ---------------------------------------------------------*/
/* Public variables ---------------------------------------------------------*/

/* Public functions ----------------------------------------------------------*/

void clock_init(void)
{
  CLK_DeInit();
  CLK_HSICmd(ENABLE);
  CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV4);
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER1, ENABLE);
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER4, ENABLE);
}

void pin_init(void)
{
  /* Initialize LEDs in Output Push-Pull Mode */
  GPIO_Init(LEDS_PORT, (LED5_PIN), GPIO_MODE_OUT_PP_LOW_FAST);

  /* Initialize PB0 (BUTTON) in Input Floating Mode with Interrupt */
  GPIO_Init(BUTTON_PORT, (BUTTON_PIN1 | BUTTON_PIN2), GPIO_MODE_IN_PU_IT);

  /* Initialize the Interrupt sensitivity */
  EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOB, EXTI_SENSITIVITY_RISE_FALL);

  enableInterrupts();
}


/**
  * @brief Example firmware main entry point.
  * @par Parameters:
  * None
  * @retval
  * None
  */
void main(void)
{
  clock_init();
  timer_init();
	button_init();
  pin_init();

  while (1)
  {
  }

}

/**
  * @brief Delay.
  * @param[in] nCount
  * @retval
  * None
  */
void Delay(u16 nCount)
{
  /* Decrement nCount value */
  while (nCount != 0)
  {
    nCount--;
  }
}




#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval
  * None
  */
void assert_failed(u8* file, u32 line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
