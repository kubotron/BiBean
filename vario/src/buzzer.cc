#include "buzzer.h"
#include "bibip.h"
#include "buzzer_utils.h"

//uint8_t a = 0;
//uint32_t buzzer_next_iteration = 1000;
//#include "songs.h"

//#define TEST_SEQUENCE
// extern Usart usart;

#ifdef TEST_SEQUENCE
	#define SWITCH_STEP 200
	#define SWITCH_OFF 0
	#define SWITCH_ON  1

	#define TEST_CLIMB_FROM -3.5
	#define TEST_CLIMB_TO 3.0

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
		audio_off();
		#ifdef TEST_SEQUENCE
				climb = test_climb;
				printf(" \nCLIMB: %f\n", climb);
		#endif

		next_bibip_freq1 = get_near(climb, bibip_freq1);
		next_bibip_freq2 = get_near(climb, bibip_freq2);
		next_bibip_pause = get_near(climb, bibip_pause);

		#ifdef TEST_SEQUENCE
			printf("pause: next_bibip_pause\n next_bibip_pause");
		#endif

		timer_buzzer_delay.SetTop(next_bibip_pause);
		buzzer_period = PERIOD_PAUSE;
	}
	else if (buzzer_period == PERIOD_PAUSE)
	//sound start
	{
		#ifdef TEST_SEQUENCE
			printf(" *bi*");
			printf("freq1 %u *", next_bibip_freq1);
		#endif

		 seq_start_freq(next_bibip_freq1);
		timer_buzzer_delay.SetTop(bibip_sound);
		buzzer_period = BIBIP_GAP;
		return;
	}
	else if (buzzer_period == BIBIP_GAP)
	//gap start
	{
		#ifdef TEST_SEQUENCE
			printf("--");
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
			printf("+bip+");
			printf(" freq2 %u", next_bibip_freq2);
		#endif
		
		 seq_start_freq(next_bibip_freq2);

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
			test_climb = test_climb + 0.2;

			if (test_climb > TEST_CLIMB_TO){
				test_climb = TEST_CLIMB_FROM;
				printf("WRAPAROUND\n");
			}
		}
	#endif
}


