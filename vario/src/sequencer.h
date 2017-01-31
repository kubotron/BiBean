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

void seq_start_freq(uint16_t freq);
void seq_step();

#endif /* SEQUENCER_H_ */
