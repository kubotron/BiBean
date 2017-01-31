#include "buzzer_utils.h"
extern configuration cfg;
extern Timer timer_buzzer_tone;
extern Timer timer_buzzer_delay;

//set buzzer volume
void buzzer_set_volume(uint16_t vol)
{

	switch(vol)
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
	default:
		//off
		//DisableOutputs(timer_A | timer_B | timer_C | timer_D)
		PORTC.DIRCLR = 0b11110000;
	}
}

//set buzzer volume
void buzzer_set_volume()
{
	#ifdef TEST_SEQUENCE
		cfg.buzzer_volume = 1;
	#endif
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

volatile uint16_t tone;
void buzzer_set_envelope(uint16_t freq, uint16_t volume){

	if (freq == 0 || volume == 0)
	{
		audio_off();
		return;
	}

	tone = 31250 / freq;
	timer_buzzer_tone.SetCompare(timer_A | timer_B | timer_C | timer_D, tone / 2);
	timer_buzzer_tone.SetTop(tone);
	//enable output & set volume
	buzzer_set_volume(volume);

	//if period is smaller than CNT restart timer -> overflow protection
	if (timer_buzzer_tone.GetValue() > tone)
		timer_buzzer_tone.SetValue(0);

}

void tone_set(uint16_t tone)
{
	//set tone
	timer_buzzer_tone.SetCompare(timer_A | timer_B | timer_C | timer_D, tone / 2);
	timer_buzzer_tone.SetTop(tone);
	//enable output & set volume
	buzzer_set_volume();

	//if period is smaller than CNT restart timer -> overflow protection
	if (timer_buzzer_tone.GetValue() > tone)
		timer_buzzer_tone.SetValue(0);
}

void audio_off(){
	timer_buzzer_tone.DisableOutputs(timer_A | timer_B | timer_C | timer_D);
	timer_buzzer_tone.Stop();
}

void buzzer_init()
{
	GpioSetInvert(portc6, ON);
	GpioSetInvert(portc7, ON);

#ifdef FAST_CLOCK
	timer_buzzer_tone.Init(timerC4, timer_div1024);
	timer_buzzer_delay.Init(timerC5, timer_div1024);
#endif

#ifdef SLOW_CLOCK
	timer_buzzer_tone.Init(timerC4, timer_div64);
	timer_buzzer_delay.Init(timerC5, timer_div64);
#endif

	timer_buzzer_tone.SetMode(timer_pwm);
	TCC4.CTRLC = 0b00001100;
	timer_buzzer_tone.EnableOutputs(timer_A | timer_B | timer_C | timer_D);

	timer_buzzer_delay.EnableInterrupts(timer_overflow);
}

//linear aproximation between two points
uint16_t get_near(float vario, uint16_t * src)
{
	vario = vario * 2; //1 point for 50cm
	float findex = floor(vario) +  20;
	float m = vario - floor(vario);

	uint8_t index = findex;

	if (findex > 39)
	{
		index = 39;
		m = 1.0;
	}

	if (findex < 0)
	{
		index = 0;
		m = 0.0;
	}

	m = round(m * 10) / 10.0;

	int16_t start = src[index];

	start = start + (float)((int16_t)src[index + 1] - start) * m;

	return start;
}
