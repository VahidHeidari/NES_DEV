/**
 * NES_DEV is a cross-platform, portable, and hand-held NES emulator.
 *
 * Copyright (C) 2015  Vahid Heidari (DeltaCode)
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "usart.h"

#include <stdio.h>

#include "config.h"

void init_usart()
{
	// Initialize USART
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN			// Enable clock
			| RCC_APB2ENR_AFIOEN;									// Enable alternate function clock
	
	AFIO->MAPR &= ~(1 << 2);									// Clear USART2 remap
	
	GPIOA->CRH = 0x00000400										// PA10 RX input float / input push-pull
			| 0x000000B0;													// PA9  Tx alternate function push-pull

	USART1->BRR = 0x0271;											// 115200 kbs buad rate
	USART1->CR1 = USART_CR1_UE								// USART1 enable
			| USART_CR1_TE												// Transmiter enable
			| USART_CR1_RE;												// Receiver enable

	USART1->CR2 = 0x00000000;
	USART1->CR3 = 0x00000000;
}

