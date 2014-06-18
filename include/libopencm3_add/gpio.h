/** @defgroup gpio_defines GPIO Defines

@brief <b>Defined Constants and Types for the STM32F4xx General Purpose I/O</b>

@ingroup STM32F4xx_defines

@version 1.0.0

@date 1 July 2012

LGPL License Terms @ref lgpl_license
 */

/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2014 Daniel Davidek <lordmutty@gmail.com>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LIBOPENCM3_GPIO_H____DEBUG
#define LIBOPENCM3_GPIO_H____DEBUG

//#include <libopencm3/stm32/memorymap.h>
//#include <libopencm3/stm32/common/gpio_common_f24.h>

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/* not sure if right */


/* Timer2 GPIO */
#define GPIO_TIM2_CH1_ETR		GPIO0		/* PA0 */
#define GPIO_TIM2_CH2			GPIO1		/* PA1 */
#define GPIO_TIM2_CH3			GPIO2		/* PA2 */
#define GPIO_TIM2_CH4			GPIO3		/* PA3 */

#define GPIO_TIM2_PR1_CH1_ETR		GPIO5		/* PA5 */
#define GPIO_TIM2_PR1_CH2		GPIO3		/* PB3 */
#define GPIO_TIM2_PR1_CH3		GPIO10		/* PB10 */
#define GPIO_TIM2_PR1_CH4		GPIO11		/* PB11 */

#define GPIO_TIM2_PR2_CH1_ETR		GPIO15		/* PA15 */

/* Timer2 BANK */
#define GPIO_BANK_TIM2_CH1_ETR		GPIOA		/* PA5 */
#define GPIO_BANK_TIM2_CH2		GPIOA		/* PA1 */
#define GPIO_BANK_TIM2_CH3		GPIOA		/* PA2 */
#define GPIO_BANK_TIM2_CH4		GPIOA		/* PA3 */
#define GPIO_BANK_TIM2			GPIOA

#define GPIO_BANK_TIM2_PR1_CH1_ETR	GPIOA		/* PA15 */
#define GPIO_BANK_TIM2_PR1_CH2		GPIOB		/* PB3 */
#define GPIO_BANK_TIM2_PR1_CH3		GPIOB		/* PB10 */
#define GPIO_BANK_TIM2_PR1_CH4		GPIOB		/* PB11 */
#define GPIO_BANK_TIM2_PR1_CH234	GPIOB

#define GPIO_BANK_TIM2_PR2_CH1_ETR	GPIOA		/* PA0 */

#endif

