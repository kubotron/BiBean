/*
 * sequencer.h
 *
 *  Created on: 3.9.2015
 *      Author: horinek
 */

#ifndef SEQUENCER_H_
#define SEQUENCER_H_

#include "common.h"
#include "buzzer_utils.h"

struct sequence_t_env
{
	const uint16_t * tone_ptr;
	const uint16_t * length_ptr;
	const uint16_t * volume_ptr;
	uint8_t length;
};

#define ARR(...) __VA_ARGS__

#define MK_SEQ_ENV(name, tone, length, volume) \
	const uint16_t name ## _tone[] PROGMEM = tone; \
	const uint16_t name ## _length[] PROGMEM = length; \
	const uint16_t name ## _volume[] PROGMEM = volume; \
	const sequence_t_env name PROGMEM = {name ## _tone, name ## _length, name ## _volume, sizeof(name ## _tone) / 2}; \

void seq_start_freq(uint16_t freq);
void seq_start_env(const sequence_t_env * seq);
void seq_step();

#endif /* SEQUENCER_H_ */
