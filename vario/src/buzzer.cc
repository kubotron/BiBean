#include "buzzer.h"
#include "bibip.h"
#include "buzzer_utils.h"

//uint8_t a = 0;
//uint32_t buzzer_next_iteration = 1000;
//#include "songs.h"

//#define TEST_SEQUENCE
//extern Usart usart;

#ifdef TEST_SEQUENCE
	#define SWITCH_STEP 100
	#define SWITCH_OFF 0
	#define SWITCH_ON  1

	#define TEST_CLIMB_FROM -5
	#define TEST_CLIMB_TO 10

	volatile float test_climb = TEST_CLIMB_FROM;

	uint16_t test_step = 0;
	uint8_t led_switch = SWITCH_OFF;
#endif

extern float climb;

Timer timer_buzzer_tone;
Timer timer_buzzer_delay;

volatile uint16_t next_bibip_freq1 = 0;
volatile uint16_t next_bibip_freq2 = 0;
volatile uint16_t next_bibip_length = 0;
volatile uint16_t next_bibip_pause = 0;

const uint16_t bibip_gap = 40 * 31;
const uint16_t bibip_sound = 250 * 31;

volatile bool delay_on = false;

#define PERIOD_SOUND		0
#define BIBIP_GAP	        1
#define BIBIP_SOUND		    2
#define PERIOD_PAUSE		3

uint16_t  * bibip_freq1;
uint16_t  * bibip_freq2;
uint16_t  * bibip_pause;

volatile uint8_t buzzer_period = PERIOD_SOUND;

bool bibit_initialized = false;

void buzzer_set_tone(uint16_t tone){}
void buzzer_set_delay(uint16_t length, uint16_t pause){}

ISR(timerC5_overflow_interrupt)
{
	timer_buzzer_delay.ClearOverflowFlag();

	if (buzzer_period == PERIOD_SOUND)
	//pause start
	{

		//printf(" \n\rCLIMB: %f\n\r", climb);

		audio_off();
		#ifdef TEST_SEQUENCE
			climb = test_climb;
			printf(" \n\rCLIMB: %f\n\r", climb);
			printf("pause: next_bibip_pause: %u\n\r", next_bibip_pause);
		#endif

		if (climb > 0.5)
		//lift
		{
			next_bibip_freq2 = get_near(climb, bibip_freq1);
			next_bibip_freq1 = get_near(climb, bibip_freq2);
		} 
		else if (climb <= 0.5 && climb > -1.5)
		//somewhat buoyant	
		{
			next_bibip_freq2 = get_near(climb, bibip_freq1);
			next_bibip_freq1 = get_near(climb, bibip_freq1);
		} 
		else  
		//sink
		{	
			next_bibip_freq1 = get_near(climb, bibip_freq1);
			next_bibip_freq2 = get_near(climb, bibip_freq2);
		}

		next_bibip_pause = 31 * get_near(climb, bibip_pause);

		timer_buzzer_delay.SetTop(next_bibip_pause);
		buzzer_period = PERIOD_PAUSE;
	}
	else if (buzzer_period == PERIOD_PAUSE)
	//sound start
	{
		#ifdef TEST_SEQUENCE
			printf(" *bi*\n\r");
			printf("freq1 %u\n\r *", next_bibip_freq1);
		#endif

//		seq_start_freq(next_bibip_freq1);
		beep(next_bibip_freq1);

		timer_buzzer_delay.SetTop(bibip_sound);
		buzzer_period = BIBIP_GAP;
		return;
	}
	else if (buzzer_period == BIBIP_GAP)
	//gap start
	{
		#ifdef TEST_SEQUENCE
			printf("--\n\r");
			//printf(" gap: %u ", bibip_gap);
			//printf("--");
		#endif

		audio_off();

		timer_buzzer_delay.SetTop(bibip_gap);
		buzzer_period = BIBIP_SOUND;
		return;
	}
	else if (buzzer_period == BIBIP_SOUND)
	//bibip start
	{
		#ifdef TEST_SEQUENCE
			printf("+bip+\n\r");
			printf(" freq2 %u\n\r", next_bibip_freq2);
		#endif
		
//		seq_start_freq(next_bibip_freq2);
		beep(next_bibip_freq2);

		timer_buzzer_delay.SetTop(bibip_sound);
		buzzer_period = PERIOD_SOUND;
		return;
	}
}

void buzzer_step(){
	if (!bibit_initialized){
		bibip_init();

		timer_buzzer_delay.SetTop(next_bibip_pause);
		buzzer_period = PERIOD_PAUSE;
		timer_buzzer_delay.Start();

		bibit_initialized = true;
	}

	seq_step();

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
			test_climb = test_climb + 0.5;

			if (test_climb > TEST_CLIMB_TO){
				test_climb = TEST_CLIMB_FROM;
				printf("WRAPAROUND\n");
			}
		}
	#endif
}


