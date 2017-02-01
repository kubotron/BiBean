#include "sequencer.h"
#include "buzzer.h"

//sequencer
volatile bool seq_enabled = false;

volatile uint8_t seq_index;
volatile uint16_t seq_duration;
volatile uint8_t seq_volume;

#define CONST_ENV_LEN 40
const uint16_t constant_duration = 1;
volatile uint16_t const_tone;

//values 0-4 only
const uint8_t constant_envelope[CONST_ENV_LEN] = {2,4,4,3,3,3,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,0,0,1,0,0,0,1}; 

#define AUDIO_SILENT_AFTER_SEQ	250

void seq_start_freq(uint16_t freq)
{
    audio_off();
    seq_enabled = true;
    seq_index = 0;   
    const_tone = freq;
    timer_buzzer_tone.Start();
}

void seq_next_tone_freq()
{
    seq_duration = constant_duration;
    seq_index++;
    if (seq_index < CONST_ENV_LEN){
        seq_volume = constant_envelope[seq_index];
    }
    buzzer_set_envelope(const_tone, seq_volume);
}

//audio_step @ 100Hz (called from fc meas_timer)
#define AUDIO_STEP_MS	10

void seq_loop()
{   
    if (!seq_enabled)
    {
        audio_off();
        return;
    }
    if (seq_duration > AUDIO_STEP_MS)
    {
        seq_duration -= AUDIO_STEP_MS;
    }
    else
    {
        if (seq_index == CONST_ENV_LEN + 1)
        {
            seq_enabled = false;
            audio_off();
        }
        else 
        {       
            seq_next_tone_freq();
        }
    }
};

void seq_step(){
    if (seq_enabled){
        seq_loop();
    }
}
