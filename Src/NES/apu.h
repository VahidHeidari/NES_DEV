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

#ifndef APU_H
#define APU_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define APU_DIVIDER_VALUE		89490
#define APU_DIVIDER_FREQ		240

#define APU_CHUNK_BUFFER_SIZE	(1024 * 4)

/// $4000 / $4004	Pulse
#define APU_PULSE1_R1				0x4000
#define APU_PULSE2_R1				0x4004
#define APU_PULSE_DUTY				0xC0
#define APU_PULSE_LOOP_ENVELOPE		0x20
#define APU_PULSE_LENGTH_ENABLE		0x20
#define APU_PULSE_CONSTANT_VOLUME	0x10
#define APU_PULSE_ENVELOPE_PERIOD	0x0F
#define APU_PULSE_ENVELOPE_VOLUME	0x0F

/// $4001 / $4005
#define APU_PULSE1_R2				0x4001
#define APU_PULSE2_R2				0x4005
#define APU_PULSE_SWEEP_UNIT_ENABLE	0x80
#define APU_PULSE_PERIOD			0x70
#define APU_PULSE_NEGATIVE			0x08
#define APU_PULSE_SHIFT_COUNT		0x07

/// $4002 / $4006
#define APU_PULSE1_R3				0x4002
#define APU_PULSE2_R3				0x4006
#define APU_PULSE_TIMER_LOW			0xFF

/// $4003 / $4007
#define APU_PULSE1_R4				0x4003
#define APU_PULSE2_R4				0x4007
#define APU_PULSE_LENGTH_COUNTER	0xF8
#define APU_PULSE_TIMRE_HIGH		0x07

/// $4008 - $400B	Triangle
#define APU_TRI_R1				0x4008
#define APU_TRI_UNUSED			0x4009
#define APU_TRI_R2				0x400A
#define APU_TRI_R3				0x400B
#define APU_TRI_LENGTH_ENABLE	0x80

/// $400C - $400F	Noise
#define APU_NOISE_R1			0x400C
#define APU_NOISE_UNUSED		0x400D
#define APU_NOISE_R2			0x400E
#define APU_NOISE_R3			0x400F
#define APU_NOISE_LENGTH_ENABLE	0x80

/// $4010 - $4013	DMC
#define APU_DMC_R1				0x4010
#define APU_DMC_R2				0x4011
#define APU_DMC_R3				0x4012
#define APU_DMC_R4				0x4013

/// $4015 (write)	Control
#define APU_CONTROL					0x4015
#define APU_CTRL_DMC_ENABLE			0x10
#define APU_CTRL_NOISE_ENABLE		0x08
#define APU_CTRL_TRIANGLE_ENABLE	0x04
#define APU_CTRL_PULSE2_ENABLE		0x02
#define APU_CTRL_PULSE1_ENABLE		0x01

/// $4015 (read)		Status
#define APU_STATUS				0x4015
#define APU_DMC_INTERRUPT		0x80
#define APU_FRAME_INTERRUPT		0x40
#define APU_NOISE_STATUS		0x08
#define APU_TRIANGLE_STATUS		0x04
#define APU_PULSE2_STATUS		0x02
#define APU_PULSE1_STATUS		0x01

/// $4017 (write)	Frame counter
#define APU_FRAME					0x4017
#define APU_FRAME_SEQUENCE_MODE		0x80
#define APU_FRAME_INTERRUPT_DISABLE	0x40

#define PULSE_GET_FREQ(pulse)		((pulse)->r3 | (((pulse)->r4 & 0xF8) << 5))

#define APU_LENGTH_TABLE_SIZE		32
#define APU_NOISE_TABLE_SIZE		16
#define APU_DMC_TABLE_SIZE			16

/// pAPU period tables
extern const uint8_t apu_length_table[APU_LENGTH_TABLE_SIZE];
extern const uint16_t apu_nois_table[APU_NOISE_TABLE_SIZE];
extern const uint16_t apu_dmc_table[APU_DMC_TABLE_SIZE];

typedef struct Pulse
{
	/**
	 * Duty, loop envelope/disable length counter, constant volume
	 * , envelope period/volume
	 *
	 * DDLC  NNNN
	 */
	uint8_t r1;			/// $4000 / $4004

	/**
	 * Sweep unit enable, period, negative, shift counter
	 *
	 * EPPP  NSSS
	 */
	uint8_t r2;			/// $4001 / $4005

	/**
	 * Timer low
	 *
	 * LLLL  LLLL
	 */
	uint8_t r3;			/// $4002 / $4006

	/**
	 * Length counter load, timer high
	 *
	 * LLLL  LHHH
	 */
	uint8_t r4;			/// $4003 / $4007

	int length_counter;
} Pulse, *pPulse;

typedef struct Triangle
{
	/**
	 * Length counter disable/linear counter control,
	 * linear counter reload value
	 *
	 * CRRR  RRRR
	 */
	uint8_t r1;			/// $4008

	/**
	 * Timer low
	 *
	 * LLLL  LLLL
	 */
	uint8_t r2;			/// $400A

	/**
	 * Length counter load, timer high
	 *
	 * LLLL  LHHH
	 */
	uint8_t r3;			/// $400B

	int length_counter;
} Triangle, *pTriangle;

typedef struct Noise
{
	/**
	 * Loop envelope/disable length counter, constatnt volume,
	 * envelope period/volume
	 *
	 * --LC  NNNN
	 */
	uint8_t r1;			/// $400C

	/**
	 * Loop noise, noise period
	 *
	 * L---   PPPP
	 */
	uint8_t r2;			/// $400E

	/**
	 * Length counter load
	 *
	 * LLLL   L---
	 */
	uint8_t r3;			/// $400F

	int length_counter;
} Noise, *pNoise;

typedef struct DMC
{
	/**
	 * IRQ enable, loop sample, frequency index
	 *
	 * IL--  FFFF
	 */
	uint8_t r1;			/// $4010

	/**
	 * Direct load
	 *
	 * -DDD  DDDD
	 */
	uint8_t r2;			/// $4011

	/**
	 * Sample address % 11AAAAAA.AA000000
	 *
	 * AAAA  AAAA
	 */
	uint8_t r3;			/// $4012

	/**
	 * Sample length % 0000LLLL.LLLL0001
	 *
	 * LLLL  LLLL
	 */
	uint8_t r4;			/// $4013

	int irq_flag;
} DMC, *pDMC;

/**
 * APU master clock is 21.47727 MHz and divided by 12 to obtain ~1.79 MHz
 */
typedef struct Apu
{
	Pulse pulse1;
	Pulse pulse2;
	Triangle tri;
	Noise noise;
	DMC dmc;

	/**
	 * Control: DMC enable, length counter enables: noise, triangle, pulse2, pulse1
	 *
	 * ---D  NT21
	 */
	uint8_t control;		/// $4015 (write)

	/**
	 * Status: DMC interrupt, frame interrupt, length counter status: nois, triangle
	 * , pulse2, pulse1
	 *
	 * IF-D  NT21
	 */
	uint8_t status;			/// $4015 (read)

	/**
	 * Frame counter: 5-frame sequence, disable frame interrupt
	 *
	 * SD--  ----
	 */
	uint8_t frame_counter_ctrl;	/// $4017

	int frame_counter;
	int sequencer;
	int irq_flag;
} Apu, *pApu;

int apu_init(pApu apu);
void apu_write(pApu apu, uint16_t addr, uint8_t value);
uint8_t apu_read(pApu apu, uint16_t addr);
void apu_close(pApu apu);
void apu_clock(pApu apu);

void apu_envelope_triangle_clock(pApu apu);
void apu_length_sweep_clock(pApu apu);

#ifdef __cplusplus
}
#endif

#endif

