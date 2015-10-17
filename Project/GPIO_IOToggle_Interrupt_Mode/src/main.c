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

/**
  * @addtogroup GPIO_IOToggle_Interrupt_Mode
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Evalboard I/Os configuration */

#define LED1_PORT (GPIOD)
#define LEDS_PORT (GPIOB)
#define LED1_PIN	(GPIO_PIN_0)
#define LED2_PIN  (GPIO_PIN_4)
#define LED3_PIN  (GPIO_PIN_3)
#define LED4_PIN  (GPIO_PIN_2)
#define LED5_PIN  (GPIO_PIN_1)

#define BUTTON_PORT (GPIOB)
#define BUTTON_PIN  (GPIO_PIN_0)

/* Private macro -------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void Delay (u16 nCount);

/* Private functions ---------------------------------------------------------*/
/* Public variables ---------------------------------------------------------*/
extern u8 ButtonState;

/* Public functions ----------------------------------------------------------*/

/**
  * @brief Example firmware main entry point.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
void main(void)
{
  u8 Leds; /* Contains which LEDs to operate */

  /* Initialize LEDs in Output Push-Pull Mode */
  GPIO_Init(LEDS_PORT, (LED2_PIN | LED3_PIN | LED4_PIN | LED5_PIN), GPIO_MODE_OUT_PP_LOW_FAST);

  /* Initialize PB0 (BUTTON) in Input Floating Mode with Interrupt */
  GPIO_Init(BUTTON_PORT, BUTTON_PIN, GPIO_MODE_IN_FL_IT);

  /* Initialize the Interrupt sensitivity */
  EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOB, EXTI_SENSITIVITY_FALL_ONLY);

  enableInterrupts();

  /* Toggles LEDs */

  while (1)
  {
    /* Check button status */
    if (ButtonState == (u8)0x00)
    {
      Leds = (LED2_PIN | LED5_PIN);
    }
    else
    {
      Leds = (LED3_PIN | LED4_PIN);
    }
		
    /* LEDs ON */
    GPIO_WriteHigh(LEDS_PORT, Leds);
    
		Delay((u16)120000);
    
		/* LEDs OFF */
    GPIO_WriteLow(LEDS_PORT, Leds);
    
		Delay((u16)120000);
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
