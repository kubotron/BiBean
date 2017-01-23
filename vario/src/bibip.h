
#ifndef BIBIP_H_
#define BIBIP_H_
 
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

#define BIBIP_S 300 * 31
#define BIBIP_L 900 * 31
#define BIBIP_M 600 * 31

extern uint16_t * bibip_freq1;   
extern uint16_t * bibip_freq2;   
extern uint16_t * bibip_pause;   

void bibip_init();

#endif /* BIBIP_H_ */