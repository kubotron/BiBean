#include "sequencer.h"
#include "buzzer.h"

//sequencer
volatile bool seq_enabled = false;

volatile const uint16_t * seq_tone_ptr;
volatile const uint16_t * seq_length_ptr;
volatile const uint16_t * seq_volume_ptr;

volatile uint8_t seq_index;
volatile uint8_t seq_len;
volatile uint16_t seq_duration;
volatile uint16_t seq_envelope;
volatile uint8_t seq_volume;
volatile uint16_t tone1;
volatile uint16_t tone2;


#define CONST_ENV_LEN 20
const uint16_t constant_duration = 1;
volatile uint16_t const_tone;

//values 0-4 only
const uint8_t constant_envelope[CONST_ENV_LEN] = {2,4,4,3,3,3,2,2,2,2,2,2,2,1,1,1,1,1,1,1}; 

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


void seq_start_env(const sequence_t_env * seq)
{
    audio_off();
    seq_enabled = true;

    seq_len = pgm_read_byte(&seq->length);
    seq_tone_ptr = (const uint16_t*)pgm_read_word(&seq->tone_ptr);
    seq_length_ptr = (const uint16_t*)pgm_read_word(&seq->length_ptr);
    seq_volume_ptr = (const uint16_t*)pgm_read_word(&seq->volume_ptr);
    seq_index = 0;   
    
    timer_buzzer_tone.Start();
}

void seq_next_tone_env()
{
    uint16_t tone;
    if (seq_index < seq_len)
    {
        //load tone and length from pgm
        tone = pgm_read_word(&seq_tone_ptr[seq_index]);
        if (seq_index == 0){
            tone1 = tone;
        }
        if (seq_index == 2){
            tone2 = tone;
        }
        seq_duration = pgm_read_word(&seq_length_ptr[seq_index]);
        seq_envelope = pgm_read_word(&seq_volume_ptr[seq_index]);
    }
    else
    {
        //this will separate sequence end from vario sound
        tone = 0;
        seq_duration = AUDIO_SILENT_AFTER_SEQ;
    }

    seq_index++;
    seq_volume = seq_envelope;//fixme cast to 8bit int
    buzzer_set_envelope(tone, seq_volume);
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
        if (seq_index == seq_len + 1)
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
