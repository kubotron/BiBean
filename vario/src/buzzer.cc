#include "buzzer.h"
#include "buzzer_utils.h"

uint8_t a = 0;
uint32_t buzzer_next_iteration = 1000;
//#include "songs.h"
#include "bibip.h"

extern float climb;
extern Usart usart;

Timer timer_buzzer_tone;
Timer timer_buzzer_delay;

volatile uint16_t next_tone = 0;
volatile uint16_t next_length = 0;
volatile uint16_t next_pause = 0;
const uint16_t bibip_gap = 70 * 31;
const uint16_t bibip_sound = 300 * 31;

volatile bool delay_on = false;
//uint8_t buzzer_mode = 0;

#define PERIOD_SOUND		0
#define BIBIP_GAP	        1
#define BIBIP_SOUND		    2
#define PERIOD_PAUSE		3

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
		if (delay_on == false){
			tone_set(next_tone);
		}

		//fluid update is enabled
		if (cfg.fluid_update and buzzer_period == PERIOD_SOUND){
			tone_set(next_tone);
		}

		timer_buzzer_tone.Start(); //if it is not running
	}
}


ISR(timerC5_overflow_interrupt)
{
	timer_buzzer_delay.ClearOverflowFlag();

	if (buzzer_period == PERIOD_SOUND)
	//pause start
	{	
		timer_buzzer_tone.DisableOutputs(timer_A | timer_B | timer_C | timer_D);

		if (next_pause == 0)
		{
			timer_buzzer_delay.Stop();
			delay_on = false;

			return;
		}

		timer_buzzer_delay.SetTop(next_pause);

		buzzer_period = PERIOD_PAUSE;
	}
	else if (buzzer_period == PERIOD_PAUSE)
	//sound start
	{	
		if (next_tone > 0)
		{
			tone_set(next_tone);
			buzzer_set_volume();
		}

		if (next_length == 0)
		{
			timer_buzzer_delay.Stop();
			delay_on = false;

			return;
		}

		timer_buzzer_delay.SetTop(bibip_sound);

		buzzer_period = BIBIP_GAP;
		return;
	}
	else if (buzzer_period == BIBIP_GAP)
	//gap start
	{
		timer_buzzer_tone.DisableOutputs(timer_A | timer_B | timer_C | timer_D);

		timer_buzzer_delay.SetTop(bibip_gap);
		buzzer_period = BIBIP_SOUND;
		return;
	}
	else if (buzzer_period == BIBIP_SOUND)
	//bibip start
	{
		if (next_tone > 0)
		{
			tone_set(next_tone);
			buzzer_set_volume();
		}

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

extern uint8_t buzzer_override;
extern uint16_t buzzer_override_tone;

//bool climb_override = false;
uint16_t buzzer_tone;
uint16_t buzzer_delay;

uint16_t old_freq = 0;
uint16_t old_leng = 0;
uint16_t old_paus = 0;


//uint16_t blik = 0;

extern float ram_sink_begin;
extern float ram_lift_begin;

//#define TEST_SEQUENCE true
#define SWITCH_STEP 50
#define SWITCH_OFF 0
#define SWITCH_ON  1

#define TEST_CLIMB_FROM -1.4
#define TEST_CLIMB_TO 1.4

float test_climb = -1.2;
uint16_t test_step = 0;
uint8_t led_switch = SWITCH_OFF;
uint8_t fluid_lift_counter = 0;

void buzzer_step(){

	#ifdef TEST_SEQUENCE

//		if (led_switch == SWITCH_OFF){
//			LEDG_ON;LEDR_OFF;
//		} else {
//			LEDG_OFF;LEDR_ON;
//		}

		test_step++;
		if (test_step > SWITCH_STEP){
			test_step = 0;
//			if (led_switch == SWITCH_OFF){
//				led_switch = SWITCH_ON;
//			} else {
//				led_switch = SWITCH_OFF;
//			}
			test_climb = test_climb + 0.1;
			climb = test_climb;
			printf("test_climb: %f\n", climb);
		}

		if (climb > TEST_CLIMB_TO){
			climb = TEST_CLIMB_FROM;
			printf("WRAPAROUND\n");
		}
	#endif

	//generate sound for menu
	if (buzzer_override)
	{
		timer_buzzer_delay.Stop();
		buzzer_set_delay(0, 0);

		delay_on = false;
		buzzer_set_tone(buzzer_override_tone);
		return;
	}

	uint16_t freq;
	uint16_t length;
	uint16_t pause;

	//GET fresh values from table
	// - climb is float in m/s
	if (climb >= ram_lift_begin || climb <= (ram_sink_begin))
	{
		//get frequency from the table
		
		freq = get_near(climb, prof.buzzer_freq);
		length = get_near(climb, prof.buzzer_length);
		pause = get_near(climb, prof.buzzer_pause);

	}
	else
	//no threshold was exceeded -> silent
	{
		freq = 0;
		length = 0;
		pause = 0;
	}

	#ifdef TEST_SEQUENCE
		if (buzzer_period == PERIOD_SOUND){
			freq = get_near(climb, prof.buzzer_freq);
			length = get_near(climb, prof.buzzer_length);
			pause = get_near(climb, prof.buzzer_pause);
		} else if (buzzer_period == BIBIP_SOUND){
			freq =440;
		}
	#endif

	//update buzzer with new settings
	buzzer_set_tone(freq);
	buzzer_set_delay(length, pause);

}


