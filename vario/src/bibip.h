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

#define ENVELOPE_TEST ARR({1,4,4,4,3,3,3,2,2,2,2,1,1,1,0,1,0,1,0,1})

MK_SEQ_ENV(env_seq, ARR({440,
440,
440,
440,
440,
440,
440,
440,
440,
440,
440,
440,
440,
440,
440,
440,
440,
440,
440,
440}),
ARR({1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1}), ENVELOPE_TEST);

#endif /* BIBIP_H_ */
