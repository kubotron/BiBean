#ifndef BUZZER_UTILS_
#define BUZZER_UTILS_

#include "vario.h"

void audio_off();
void buzzer_init();
void buzzer_set_volume();
void tone_set(uint16_t tone);
void buzzer_set_envelope(uint16_t freq, uint16_t volume);

uint16_t get_near(float vario, uint16_t * src);

extern Timer timer_buzzer_tone;

#endif /* BUZZER_UTILS_ */
