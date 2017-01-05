#include "buzzer_utils.h"

extern Timer timer_buzzer_tone;

//set buzzer volume
void buzzer_set_volume()
{
	switch(cfg.buzzer_volume)
	{
	case(0): //off
		//DisableOutputs(timer_A | timer_B | timer_C | timer_D)
		PORTC.DIRCLR = 0b11110000;
	break;
	case(1): //4k7 + 1k
		timer_buzzer_tone.EnableOutputs(timer_A | timer_D);
		//DisableOutputs(timer_B | timer_C)
		PORTC.DIRCLR = 0b01100000;
		PORTC.DIRSET = 0b10010000;
	break;
	case(2): //4k7
		timer_buzzer_tone.EnableOutputs(timer_A | timer_C);
		//DisableOutputs(timer_B | timer_D)
		PORTC.DIRCLR = 0b10100000;
		PORTC.DIRSET = 0b01010000;
	break;
	case(3): //1k
		timer_buzzer_tone.EnableOutputs(timer_B | timer_D);
		//DisableOutputs(timer_A | timer_C)
		PORTC.DIRCLR = 0b01010000;
		PORTC.DIRSET = 0b10100000;
	break;
	case(4): //0k
		timer_buzzer_tone.EnableOutputs(timer_B | timer_C);
		//DisableOutputs(timer_A | timer_D)
		PORTC.DIRCLR = 0b10010000;
		PORTC.DIRSET = 0b01100000;
	break;
	}

}
