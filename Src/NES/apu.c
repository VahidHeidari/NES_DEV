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

#include "apu.h"

#include <stdio.h>
#include <string.h>
#include <limits.h>

#if defined _WIN32
#define SDL_MAIN_HANDLED
#include "..\\..\\3rdParty\\SDL2-2.0.5\\include\\SDL.h"
#elif defined __linux__
#include <SDL2/SDL.h>
#endif

#include "config.h"
#include "emulator.h"
#include "debuger.h"

uint8_t const apu_length_table[APU_LENGTH_TABLE_SIZE] =
{
	0x0A, 0x14, 0x28, 0x50, 0xA0, 0x3C, 0x0E, 0x1A,
	0x0C, 0x18, 0x30, 0x60, 0xC0, 0x48, 0x10, 0x20,
	0xFE, 0x02, 0x04, 0x06, 0x08, 0x0A, 0x0C, 0x0E,
	0x10, 0x12, 0x14, 0x16, 0x18, 0x1A, 0x1C, 0x1E
};

uint16_t const apu_noise_table[APU_NOISE_TABLE_SIZE] =
{
	0x004, 0x008, 0x010, 0x020, 0x040, 0x060, 0x080, 0x0A0,
	0x0CA, 0x0FE, 0x17C, 0x1FC, 0x2FA, 0x3F8, 0x7F2, 0xFE4
};

uint16_t const apu_dmc_table[APU_DMC_TABLE_SIZE] =
{
	0x1AC, 0x17C, 0x154, 0x140, 0x11E, 0x0FE, 0x0E2, 0x0D6,
	0x0BE, 0x0A0, 0x08E, 0x080, 0x06A, 0x054, 0x048, 0x036
};

static int freq_dist = 0;
static uint16_t freq_gen(int freq)
{
	if (freq)
	{
		int freq_sample = APU_SAMPLE_RATE_FREQUENCY_HZ / freq;
		if (++freq_dist >= freq_sample)
			freq_dist = 0;

		if (freq_dist < freq_sample / 2)
			return SHRT_MIN;
		return SHRT_MAX;
	}

	return 0;
}

static void fill_audio(void* data, uint8_t* stream, int len)
{
	int i;
	uint16_t* buff = (uint16_t*)stream;
	int square_freq = 0;

	(void)data;		// Unused parameter

#ifdef __linux__
	return;		// Disable audio in linux.
#endif

	if (APU_IS_MODULE_ENABLED(APU_PULSE1_STATUS) && (apu.pulse1.r1 & APU_PULSE_ENVELOPE_VOLUME))
		square_freq = PULSE_GET_FREQ(&apu.pulse1);

	len /= 2;
	for (i = 0; i < len; i += 2)
	{
		buff[i] = freq_gen(square_freq);
		buff[i + 1] = freq_gen(square_freq);
	}
}

void apu_envelope_triangle_clock(pApu apu)
{
	(void)apu;
}

void apu_length_sweep_clock(pApu apu)
{
	if (apu->pulse1.r1 & ~APU_PULSE_LENGTH_ENABLE)
		if (apu->pulse1.length_counter != 0)
			--apu->pulse1.length_counter;

	if (apu->pulse2.r1 & ~APU_PULSE_LENGTH_ENABLE)
		if (apu->pulse2.length_counter != 0)
			--apu->pulse2.length_counter;

	if (apu->tri.r1 & ~APU_TRI_LENGTH_ENABLE)
		if (apu->tri.length_counter != 0)
			--apu->tri.length_counter;

	if (apu->noise.r1 & ~APU_NOISE_LENGTH_ENABLE)
		if (apu->noise.length_counter != 0)
			--apu->noise.length_counter;
}

int apu_init(pApu apu)
{
	SDL_AudioSpec as;

#if __linux__
	return 1;		// Disable audio in linux.
#endif

	if (SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		debug_message("Could not initialize audio!");
		return 0;
	}

	as.freq     = APU_SAMPLE_RATE_FREQUENCY_HZ;
	as.format   = AUDIO_S16SYS;
	as.channels = APU_NUMBER_OF_CHANNELS;
	as.samples  = APU_AUDIO_BUFFER_SIZE;
	as.callback = fill_audio;

	if (SDL_OpenAudio(&as, NULL) < 0)
		return 0;
	SDL_PauseAudio(0);

	memset(apu, 0, sizeof(Apu));
	return 1;
}

void apu_write(pApu apu, uint16_t addr, uint8_t value)
{
	switch (addr)
	{
		// Pulse 1
		case APU_PULSE1_R1: apu->pulse1.r1 = value; break;
		case APU_PULSE1_R2: apu->pulse1.r2 = value; break;
		case APU_PULSE1_R3: apu->pulse1.r3 = value; break;
		case APU_PULSE1_R4:
			apu->pulse1.r4 = value;
			apu->pulse1.length_counter = apu_length_table[(value >> 3) & 0x1F];
			break;

		// Pulse 2
		case APU_PULSE2_R1: apu->pulse2.r1 = value; break;
		case APU_PULSE2_R2: apu->pulse2.r2 = value; break;
		case APU_PULSE2_R3: apu->pulse2.r3 = value; break;
		case APU_PULSE2_R4:
			apu->pulse2.r4 = value;
			apu->pulse2.length_counter = apu_length_table[(value >> 3) & 0x1F];
			break;

		// Triangle
		case APU_TRI_R1: apu->tri.r1 = value; break;
		case APU_TRI_UNUSED: break;
		case APU_TRI_R2: apu->tri.r2 = value; break;
		case APU_TRI_R3:
			apu->tri.r3 = value;
			apu->tri.length_counter = apu_length_table[(value >> 3) & 0x1F];
			break;

		// Noise
		case APU_NOISE_R1: apu->noise.r1 = value; break;
		case APU_NOISE_UNUSED: break;
		case APU_NOISE_R2: apu->noise.r2 = value; break;
		case APU_NOISE_R3:
			apu->noise.r3 = value;
			apu->tri.length_counter = apu_length_table[(value >> 3) & 0x1F];
			break;

		// DMC
		case APU_DMC_R1: apu->dmc.r1 = value; break;
		case APU_DMC_R2: apu->dmc.r2 = value; break;
		case APU_DMC_R3: apu->dmc.r3 = value; break;
		case APU_DMC_R4: apu->dmc.r4 = value; break;

		// Contorl
		case APU_CONTROL:
			apu->control = value;
			
			//if (value & APU_CTRL_DMC_ENABLE) apu->dmc.r1 |= APU_DMC_
			
			if (value & APU_CTRL_PULSE1_ENABLE)
				apu->pulse1.r1 |= APU_PULSE_LENGTH_ENABLE;
			else
				apu->pulse1.r1 &= ~APU_PULSE_LENGTH_ENABLE;

			if (value & APU_CTRL_PULSE2_ENABLE)
				apu->pulse2.r1 |= APU_PULSE_LENGTH_ENABLE;
			else
				apu->pulse2.r1 &= ~APU_PULSE_LENGTH_ENABLE;

			if (value & APU_CTRL_TRIANGLE_ENABLE)
				apu->tri.r1 |= APU_TRI_LENGTH_ENABLE;
			else
				apu->tri.r1 &= ~APU_TRI_LENGTH_ENABLE;

			if (value & APU_CTRL_NOISE_ENABLE)
				apu->noise.r1 |= APU_NOISE_LENGTH_ENABLE;
			else
				apu->noise.r1 &= ~APU_NOISE_LENGTH_ENABLE;

			apu->dmc.irq_flag = 0;
			break;

		// Frame counter
		case APU_FRAME:
			apu->frame_counter_ctrl = value;
			apu->frame_counter = 0;
			apu->sequencer = 0;
			break;

		default:
			debug_message("Unknown pAPU register! addr:$%04x     value:$%02x\n", addr, value);
			log_error("Unknown pAPU register! addr:$%04x     value:$%02x\n", addr, value);
			break;
	}
}

uint8_t apu_read(pApu apu, uint16_t addr)
{
	// TODO: Change APU status to evnet diriven.
	if (addr == APU_STATUS)
	{
		// IRQ sources
		if (apu->dmc.irq_flag)
			apu->status = 0x80;
		if (apu->irq_flag)
			apu->status |= APU_FRAME_INTERRUPT;

		// Length counter status
		if (apu->pulse1.length_counter)
			apu->status |= APU_PULSE1_STATUS;
		if (apu->pulse2.length_counter)
			apu->status |= APU_PULSE2_STATUS;
		if (apu->tri.length_counter)
			apu->status |= APU_TRIANGLE_STATUS;
		if (apu->noise.length_counter)
			apu->status |= APU_NOISE_STATUS;
		
		// Clear IRQ flag.
		apu->irq_flag = 0;
		
		return apu->status;
	}

	debug_message("APU reading invalide address:%d", addr);
	log_error("APU reading invalid address:%d", addr);
	return 0;
}

void apu_close(pApu apu)
{
	(void)apu;
	return;

	SDL_CloseAudio();
}

void apu_clock(pApu apu)
{
	if (apu->frame_counter == APU_DIVIDER_VALUE)
	{
		apu->frame_counter = 0;		// Reset divider.
		++apu->sequencer;			// Clock sequencer.

		// Clock sub-systems.
		if (apu->frame_counter_ctrl & APU_FRAME_SEQUENCE_MODE)		// 5 step mode
		{
			if (apu->sequencer == 5)
				apu->sequencer = 0;			// Reset sequencer.
			else
			{
				apu_envelope_triangle_clock(apu);

				if (apu->sequencer == 1 || apu->sequencer == 3)
					apu_length_sweep_clock(apu);
			}
		}
		else														// 4 step mode
		{
			apu_envelope_triangle_clock(apu);	// Clock envelopes and triangle's linear conter

			if (apu->sequencer == 2)
				apu_length_sweep_clock(apu);	// Clock length counters and sweep units
			else if (apu->sequencer == 4)
			{
				apu_length_sweep_clock(apu);	// Clock length counters and sweep units
				p.interrupts |= INTR_IRQ;		// Assert IRQ flag.
				apu->irq_flag = 1;
				apu->sequencer = 0;				// Reset sequencer.
			}
		}
	}
	else
		++apu->frame_counter;
}

