#include "buzzer.h"

extern float climb;
volatile float next_climb = 0;

extern Usart usart;
extern configuration cfg;

Timer timer_buzzer_tone;
Timer timer_buzzer_delay;

volatile uint16_t next_tone = 0;
volatile uint16_t next_length = 0;
volatile uint16_t next_pause = 0;

volatile bool delay_on = false;
uint8_t buzzer_mode = 0;

const uint16_t bibip_gap = 40 * 31;
const uint16_t bibip_sound = 250 * 31;

uint16_t black_keys_freqs[41] = {
4978,
4435,
3729,
3322,
2960,
2489,
2217,
1865,
1661,
1479,
1244,
1109,
932	,
831	,
740	,
622	,
554	,
466	,
415	,
370	,
311	,
277	,
233	,
207	,
185	,
155	,
139	,
116	,
104	,
92	,
78	,
69	,
58	,
52	,
46  ,
39	,
34	,
29	,
26	,
23	,
19  };

uint16_t black_keys_shift[41] = {
4978,
4978,
4435,
3729,
3322,
2960,
2489,
2217,
1865,
1661,
1479,
1244,
1109,
932	,
831	,
740	,
622	,
554	,
466	,
415	,
370	,
311	,
277	,
233	,
207	,
185	,
155	,
139	,
116	,
104	,
92	,
78	,
69	,
58	,
52	,
46  ,
39	,
34	,
29	,
26	,
23 };

uint16_t bibip_pauses[41] = {
250,
300,
350,
400,
450,
500,
550,
600,
650,
700,
750,
800,
850,
900,
950,
1000,
1050,
1100,
1150,
1200,
1200,
1200,
1150,
1100,
1050,
1000,
950,
900,
850,
800,
750,
700,
650,
600,
550,
500,
450,
400,
350,
300,
250
};


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

//set buzzer volume
void buzzer_set_vol(uint8_t volume)
{
	 switch(volume)
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

//set buzzer volume
void buzzer_set_volume()
{
	buzzer_set_vol(cfg.buzzer_volume);
}


void audio_off(){
	timer_buzzer_tone.DisableOutputs(timer_A | timer_B | timer_C | timer_D);
	timer_buzzer_tone.Stop();
}

uint16_t tone;
void beep(uint16_t freq){

	if (freq == 0)
	{
		audio_off();
		return;
	}

	tone = 31250 / freq;
	timer_buzzer_tone.SetCompare(timer_A | timer_B | timer_C | timer_D, tone / 2);
	timer_buzzer_tone.SetTop(tone);
	//enable output & set volume
	buzzer_set_vol(2);

	//if period is smaller than CNT restart timer -> overflow protection
	if (timer_buzzer_tone.GetValue() > tone)
		timer_buzzer_tone.SetValue(0);
	timer_buzzer_tone.Start();

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

#define PERIOD_SOUND	0
#define PERIOD_PAUSE	1
#define BIBIP_SOUND		2
#define BIBIP_GAP		3

volatile uint8_t buzzer_period = PERIOD_SOUND;

void buzzer_set_tone(uint16_t tone)
{
	if (tone == 0)
	{
		next_tone = 0;

		//buzzer is running continuously turn off sound
		if (!delay_on)
		{
			//stop timer
			timer_buzzer_tone.Stop();
			//disable sound output
			timer_buzzer_tone.DisableOutputs(timer_A | timer_B | timer_C | timer_D);
		}
	}
	else
	{
		next_tone = 31250 / tone;

		//buzzer is running continuously update freq now
		if (delay_on == false)
			tone_set(next_tone);

		//fluid update is enabled
		if (cfg.fluid_update and buzzer_period == PERIOD_SOUND)
			tone_set(next_tone);

		timer_buzzer_tone.Start(); //if it is not running
	}
}

volatile uint16_t next_bibip_freq1 = 0;
volatile uint16_t next_bibip_freq2 = 0;
volatile uint16_t next_bibip_length = 0;
volatile uint16_t next_bibip_pause = 0;

ISR(timerC5_overflow_interrupt)
{
	timer_buzzer_delay.ClearOverflowFlag();
	if (buzzer_period == PERIOD_SOUND)
		//pause start
		{
//			printf(" \n\rCLIMB: %f\n\r", climb);
			audio_off();

			if (next_climb > 0.5)
			//lift
			{
				next_bibip_freq2 = get_near(climb, black_keys_freqs);
				next_bibip_freq1 = get_near(next_climb,  black_keys_shift);
			}
			else if (next_climb <= 0.5 && next_climb > -1.5)
			//somewhat buoyant
			{
				next_bibip_freq2 = get_near(next_climb, black_keys_freqs);
				next_bibip_freq1 = get_near(next_climb, black_keys_freqs);
			}
			else
			//sink
			{
				next_bibip_freq1 = get_near(next_climb, black_keys_freqs);
				next_bibip_freq2 = get_near(next_climb, black_keys_shift);
			}

			next_bibip_pause = 31 * 3 * get_near(next_climb, bibip_pauses);

			timer_buzzer_delay.SetTop(next_bibip_pause);
			buzzer_period = PERIOD_PAUSE;
		}
		else if (buzzer_period == PERIOD_PAUSE)
		//sound start
		{
			beep(next_bibip_freq1);

			timer_buzzer_delay.SetTop(bibip_sound);
			buzzer_period = BIBIP_GAP;
			return;
		}
		else if (buzzer_period == BIBIP_GAP)
		//gap start
		{
			audio_off();

			timer_buzzer_delay.SetTop(bibip_gap);
			buzzer_period = BIBIP_SOUND;
			return;
		}
		else if (buzzer_period == BIBIP_SOUND)
		//bibip start
		{
			beep(next_bibip_freq2);

			timer_buzzer_delay.SetTop(bibip_sound);
			buzzer_period = PERIOD_SOUND;
			return;
		}
}

void buzzer_set_delay(uint16_t length, uint16_t pause)
{
	//Continuous sound (sink)
	if (pause == 0 || length == 0)
	{
		next_length = 0;
		next_pause = 0;

	}
	else
	//with pauses (lift)
	{
		//convert from Hz and ms
		next_length = 31 * length;
		next_pause = 31 * pause;

		//if previous sound was continuous (timer_buzzer_delay is not working)
		if (delay_on == false)
		{
			//restart timer counter
			timer_buzzer_delay.SetValue(1);

			//set new tone + enable sound
			tone_set(next_tone);

			timer_buzzer_delay.SetTop(next_length);

			//start timer
			timer_buzzer_delay.Start();

			//set the period state state
			buzzer_period = PERIOD_SOUND;
		}

		//we have pauses enabled
		delay_on = true;
	}
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

extern uint8_t buzzer_override;
extern uint16_t buzzer_override_tone;

uint8_t a = 0;
uint32_t buzzer_next_iteration = 1000;

float test_climb = 0;

bool climb_override = false;
uint16_t buzzer_tone;
uint16_t buzzer_delay;


uint16_t old_freq = 0;
uint16_t old_leng = 0;
uint16_t old_paus = 0;

extern float ram_sink_begin;
extern float ram_lift_begin;

uint8_t fluid_lift_counter = 0;

bool bibit_initialized = false;

void buzzer_step()
{
	//generate sound for menu
	if (buzzer_override)
	{
		timer_buzzer_delay.Stop();
		buzzer_set_delay(0, 0);

		delay_on = false;
		buzzer_set_tone(buzzer_override_tone);
		return;
	}

//	For demonstration
//	led handling in bui_task need to be commented out
//	if (climb > 0)
//		{LEDG_ON;}
//	else
//		{LEDG_OFF;}
//
//	if (climb < 0)
//		{LEDR_ON;}
//	else
//		{LEDR_OFF;}

	//GET fresh values from table
	// - climb is float in m/s
	
		//get frequency from the table
	if (!bibit_initialized){

		timer_buzzer_delay.SetTop(next_bibip_pause);
		buzzer_period = PERIOD_PAUSE;
		timer_buzzer_delay.Start();

		bibit_initialized = true;
	}

	 if (buzzer_period != PERIOD_SOUND){
		 next_climb = climb;
	 }
}


