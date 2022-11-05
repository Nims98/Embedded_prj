/*
 * micros.c
 *
 * Created: 11/5/2022 9:27:11 AM
 * Author : Nirmala
 */ 

#include <avr/io.h>
#define SREG _SFR_IO8(0x3F)
# define cli()  __asm__ __volatile__ ("cli" ::)
#define _BV(bit) (1 << (bit))
#define clockCyclesPerMicrosecond() ( F_CPU / 1000000L )

#define FRACT_INC ((MICROSECONDS_PER_TIMER0_OVERFLOW % 1000) >> 3)
#define FRACT_MAX (1000 >> 3)
#define MILLIS_INC (MICROSECONDS_PER_TIMER0_OVERFLOW / 1000)
#define MICROSECONDS_PER_TIMER0_OVERFLOW (clockCyclesToMicroseconds(64 * 256))
volatile unsigned long timer0_overflow_count = 0;
volatile unsigned long timer0_millis = 0;
static unsigned char timer0_fract = 0;

int main(void)
{
    /* Replace with your application code */
    while (1) 
    {
    }
}

unsigned long micros() {
	unsigned long m;
	uint8_t oldSREG = SREG, t;
	
	cli();
	m = timer0_overflow_count;
	t = TCNT0;
	
	if ((TIFR0 & _BV(TOV0)) && (t < 255))
	m++;
	
	SREG = oldSREG;
	
	return ((m << 8) + t) * (64 / clockCyclesPerMicrosecond());
}

ISR(TIMER0_OVF_vect)
{
	unsigned long m = timer0_millis;
	unsigned char f = timer0_fract;
	
	m += MILLIS_INC;
	f += FRACT_INC;
	if (f >= FRACT_MAX) {
		f -= FRACT_MAX;
		m += 1;
	}
	
	timer0_fract = f;
	timer0_millis = m;
	timer0_overflow_count++;
}