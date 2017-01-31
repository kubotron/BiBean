//todo: envelope, better sound baro filtering/integration period, more sensitivity, dynamic range, tone/volume compensation
//todo silence before takeoff + override

#ifndef BIBIP_H_
#define BIBIP_H_

#include "sequencer.h"
 
//approximately pentatonical scale
#define BIBIP_0  220 
#define BIBIP_1  248
#define BIBIP_2  278
#define BIBIP_3  330
#define BIBIP_4  371
#define BIBIP_5  440
#define BIBIP_6  495
#define BIBIP_7  557
#define BIBIP_8  660
#define BIBIP_9  743
#define BIBIP_10 880
#define BIBIP_11 990
#define BIBIP_12 1114

#define BIBIP_SHORT 400 * 31
#define BIBIP_LONG 1200 * 31
#define BIBIP_MED 800 * 31

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
