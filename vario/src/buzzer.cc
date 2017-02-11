#include "buzzer.h"
//#define TEST_SEQUENCE
//#define HARDCODED_TONES


#ifdef TEST_SEQUENCE
	#define TEST_CLIMB_FROM -10
	#define TEST_CLIMB_TO 10

	volatile float test_climb = TEST_CLIMB_FROM;
#endif

#ifdef HARDCODED_TONES
	#define BIBIB_LENGTH 250
	uint16_t black_keys[41] = {26,29,34,39,46,52,58,69,78,92,104,116,139,155,185,207,233,277,311,370,415,466,554,622,740,831,932,1109,1244,1479,1661,1865,2217,2489,2960,3322,3729,4435,4978,4978,4978};
	uint16_t bibip_pauses[41] =  {100,200,300,400,500,600,700,800,900,1000,1100,1200,1300,1400,1500,1600,1700,1800,1900,2000,2000,2000,1900,1800,1700,1600,1500,1400,1300,1200,1100,1000,900,800,700,600,500,400,300,200,100};
#endif

extern float climb;
volatile float next_climb = 0;

extern Usart usart;
extern configuration cfg;

extern float ram_sink_begin;
extern float ram_lift_begin;

Timer timer_buzzer_tone;
Timer timer_buzzer_delay;

volatile uint16_t next_tone = 0;
volatile uint16_t next_length = 0;
volatile uint16_t next_pause = 0;

volatile bool delay_on = false;
uint8_t buzzer_mode = 0;

const uint16_t bibip_gap = 40;

//linear aproximation between two points
uint16_t get_near(float vario, uint16_t * src)
{
	//fixme check bounds
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

uint16_t get_lower(float vario, uint16_t * src){
	return get_near(vario - 0.5, src);
}

uint16_t get_higher(float vario, uint16_t * src){
	return get_near(vario + 0.5, src);
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
	buzzer_set_volume();

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

			audio_off();
			timer_buzzer_delay.SetTop(next_bibip_pause * 31);
			buzzer_period = PERIOD_PAUSE;
		}
		else if (buzzer_period == PERIOD_PAUSE)
		//sound start
		{
			#ifdef TEST_SEQUENCE
				test_climb = test_climb + 0.5;
				next_climb = test_climb;
			#endif
			if (next_climb >= ram_lift_begin)
			//lift
			{
				#ifdef HARDCODED_TONES
					next_bibip_freq1 = get_near(next_climb, black_keys);
					next_bibip_freq2 = get_higher(next_climb, black_keys);
				#else
					next_bibip_freq1 = get_near(next_climb, prof.buzzer_freq);
					next_bibip_freq2 = get_higher(next_climb, prof.buzzer_freq);
				#endif
			}
			else if (next_climb < ram_lift_begin && next_climb > ram_sink_begin)
			//somewhat buoyant
			{
				#ifdef HARDCODED_TONES
					next_bibip_freq2 = get_near(next_climb, black_keys);
					next_bibip_freq1 = get_near(next_climb, black_keys);
				#else
					next_bibip_freq2 = get_near(next_climb, prof.buzzer_freq);
					next_bibip_freq1 = get_near(next_climb, prof.buzzer_freq);
				#endif
			}
			else
			//sink
			{
				#ifdef HARDCODED_TONES
					next_bibip_freq1 = get_near(next_climb, black_keys);
					next_bibip_freq2 = get_lower(next_climb, black_keys);
				#else
					next_bibip_freq1 = get_near(next_climb, prof.buzzer_freq);
					next_bibip_freq2 = get_lower(next_climb, prof.buzzer_freq);
				#endif
			}
			#ifdef HARDCODED_TONES
				next_bibip_length = BIBIB_LENGTH;
				next_bibip_pause = get_near(next_climb, bibip_pauses);
			#else
				next_bibip_length = get_near(next_climb, prof.buzzer_length);
				next_bibip_pause = get_near(next_climb, prof.buzzer_pause);
			#endif

			beep(next_bibip_freq1);

			timer_buzzer_delay.SetTop(next_bibip_length * 31);
			buzzer_period = BIBIP_GAP;
			return;
		}
		else if (buzzer_period == BIBIP_GAP)
		//gap start
		{
			audio_off();
			#ifdef TEST_SEQUENCE

				printf("\r\nBibip TEST");
				printf("\r\nlift threshold: %0.2f m/s", ram_sink_begin);
				printf("\r\nsink threshold: %0.2f m/s", ram_lift_begin);
				printf("\r\nclimb: %0.2f m/s", next_climb);
				printf("\r\ntone 1: %u Hz", next_bibip_freq1);
				printf("\r\ntone 2: %u Hz", next_bibip_freq2);
				printf("\r\nlength: %u ms", next_bibip_length);
				printf("\r\npause: %u ms", next_bibip_pause);
			#endif

			timer_buzzer_delay.SetTop(bibip_gap * 31);
			buzzer_period = BIBIP_SOUND;
			return;
		}
		else if (buzzer_period == BIBIP_SOUND)
		//bibip start
		{
			beep(next_bibip_freq2);

			timer_buzzer_delay.SetTop(next_bibip_length * 31);
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

bool climb_override = false;
uint16_t buzzer_tone;
uint16_t buzzer_delay;


uint16_t old_freq = 0;
uint16_t old_leng = 0;
uint16_t old_paus = 0;

uint8_t fluid_lift_counter = 0;

//bool bibit_initialized = false;
bool was_override = true;

void buzzer_step()
{
	//generate sound for menu
	if (buzzer_override)
	{
		timer_buzzer_delay.Stop();
		buzzer_set_delay(0, 0);

		delay_on = false;
		buzzer_set_tone(buzzer_override_tone);
		was_override = true;
		return;
	} else if (was_override) {
		was_override = false;
		timer_buzzer_delay.SetTop(next_bibip_pause);
		buzzer_period = PERIOD_PAUSE;
		timer_buzzer_delay.Start();
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

	 if (buzzer_period != PERIOD_PAUSE){
		#ifndef TEST_SEQUENCE
		    next_climb = climb;
		#endif
	 }
}


