//todo: envelope, better sound baro filtering/integration period, more sensitivity, dynamic range, tone/volume compensation
//todo silence before takeoff + override

#ifndef BIBIP_H_
#define BIBIP_H_

#include "sequencer.h"
#include "sounds.h"

extern uint16_t * bibip_freq1;   
extern uint16_t * bibip_freq2;   
extern uint16_t * bibip_pause;   

void bibip_init();

#endif /* BIBIP_H_ */
