#include "common.h"
#include "bibip.h"

uint16_t  bibip_first_tone[41] = {
		BIBIP_1 ,
		BIBIP_1 ,
		BIBIP_1 ,
		BIBIP_1 ,
		BIBIP_1 ,
		BIBIP_1 ,
		BIBIP_1 ,
		BIBIP_1 ,
		BIBIP_1 ,
		BIBIP_1 ,
		BIBIP_1 ,
		BIBIP_1 ,
		BIBIP_2 ,
		BIBIP_3 ,
		BIBIP_3 ,
		BIBIP_4 ,
		BIBIP_4 ,
		BIBIP_4 ,
		BIBIP_4 ,
		BIBIP_5 ,
		BIBIP_6 ,
		BIBIP_6 ,
		BIBIP_6 ,
		BIBIP_6 ,
		BIBIP_6 ,
		BIBIP_7 ,
		BIBIP_7 ,
		BIBIP_7 ,
		BIBIP_7 ,
		BIBIP_8 ,
		BIBIP_8 ,
		BIBIP_8 ,
		BIBIP_8 ,
		BIBIP_9 ,
		BIBIP_9 ,
		BIBIP_9 ,
		BIBIP_10,
		BIBIP_10,
		BIBIP_11,
		BIBIP_11,
		BIBIP_12
};

uint16_t bibip_second_tone[41] = {
		BIBIP_1	,
		BIBIP_1	,
		BIBIP_1 ,
		BIBIP_1	,
		BIBIP_1	,
		BIBIP_1	,
		BIBIP_1	,
		BIBIP_1	,
		BIBIP_1	,
		BIBIP_1	,
		BIBIP_1	,
		BIBIP_1	,
		BIBIP_1	,
		BIBIP_1	,
		BIBIP_2	,
		BIBIP_1	,
		BIBIP_2	,
		BIBIP_3	,
		BIBIP_4	,
		BIBIP_5	,
		BIBIP_6	,
		BIBIP_7	,
		BIBIP_8	,
		BIBIP_9	,
		BIBIP_10,
		BIBIP_8	,
		BIBIP_9	,
		BIBIP_10,
		BIBIP_11,
		BIBIP_9	,
		BIBIP_10,
		BIBIP_11,
		BIBIP_12,
		BIBIP_10,
		BIBIP_11,
		BIBIP_12,
		BIBIP_11,
		BIBIP_12,
		BIBIP_12,
		BIBIP_12,
		BIBIP_12
};

uint16_t bibip_pauses[41] = {
		BIBIP_SHORT,
		BIBIP_SHORT,
		BIBIP_SHORT,
		BIBIP_SHORT,
		BIBIP_SHORT,
		BIBIP_SHORT,
		BIBIP_SHORT,
		BIBIP_SHORT,
		BIBIP_SHORT,
		BIBIP_SHORT,
		BIBIP_SHORT,
		BIBIP_SHORT,
		BIBIP_MED,
		BIBIP_MED,
		BIBIP_MED,
		BIBIP_MED,
		BIBIP_MED,
		BIBIP_LONG,
		BIBIP_LONG,
		BIBIP_LONG,
		BIBIP_MED,
		BIBIP_MED,
		BIBIP_MED,
		BIBIP_MED,
		BIBIP_MED,
		BIBIP_SHORT,
		BIBIP_SHORT,
		BIBIP_SHORT,
		BIBIP_SHORT,
		BIBIP_SHORT,
		BIBIP_SHORT,
		BIBIP_SHORT,
		BIBIP_SHORT,
		BIBIP_SHORT,
		BIBIP_SHORT,
		BIBIP_SHORT,
		BIBIP_SHORT,
		BIBIP_SHORT,
		BIBIP_SHORT,
		BIBIP_SHORT,
		BIBIP_SHORT
};

uint16_t black_keys_rev[41] = {
19  ,
23	,
26	,
29	,
34	,
39	,
46  ,
52	,
58	,
69	,
78	,
92	,
104	,
116	,
139	,
155	,
185	,
207	,
233	,
277	,
311	,
370	,
415	,
466	,
554	,
622	,
740	,
831	,
932	,
1109,
1244,
1479,
1661,
1865,
2217,
2489,
2960,
3322,
3729,
4435,
4978};

uint16_t black_keys_shift[41] = {
19  ,
19  ,
23	,
26	,
29	,
34	,
39	,
46  ,
52	,
58	,
69	,
78	,
92	,
104	,
116	,
139	,
155	,
185	,
207	,
233	,
277	,
311	,
370	,
415	,
466	,
554	,
622	,
740	,
831	,
932	,
1109,
1244,
1479,
1661,
1865,
2217,
2489,
2960,
3322,
3729,
4435};


void bibip_init(){
	bibip_freq1 = (uint16_t *) &black_keys_rev;
	bibip_freq2 = (uint16_t *) &black_keys_shift;
	bibip_pause = (uint16_t *) &bibip_pauses;
}

