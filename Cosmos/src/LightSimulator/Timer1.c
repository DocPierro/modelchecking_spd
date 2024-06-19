//
//  Timer1.c
//  Cosmos
//
//  Created by Benoît Barbot on 11/02/2015. Based on Arduino librairy
//
//

#include "Timer1.h"


#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <Arduino.h>

#define TIME1_PRESCALLER 256
// the prescaler is set so that timer1 ticks every 256 clock cycles, and the
// the overflow handler is called every 65536 ticks.
#define MICROSECONDS_PER_TIMER1_OVERFLOW (clockCyclesToMicroseconds(TIME1_PRESCALLER * 65536))

// the whole number of milliseconds per timer1 overflow
#define MILLIS_INC (MICROSECONDS_PER_TIMER1_OVERFLOW / 1000)

// the fractional number of milliseconds per timer1 overflow. we shift right
// by three to fit these numbers into a byte. (for the clock speeds we care
// about - 8 and 16 MHz - this doesn't lose precision.)
#define FRACT_INC ((MICROSECONDS_PER_TIMER1_OVERFLOW % 1000) >> 3)
#define FRACT_MAX (1000 >> 3)

volatile unsigned long timer1_millis = 0;
static unsigned char timer1_fract = 0;

/*********************************************
 * Interupt for Timer1 overflow
 ********************************************/
ISR(TIMER1_OVF_vect)
{
    unsigned long m = timer1_millis;
    unsigned char f = timer1_fract;

    m += MILLIS_INC;
    f += FRACT_INC;
    if (f >= FRACT_MAX) {
        f -= FRACT_MAX;
        m += 1;
    }

    timer1_fract = f;
    timer1_millis = m;
}

void initTimer1(){
    /* Normal timer operation.*/
    TCCR1A = 0x00;

    /* Clear the timer counter register.
     * You can pre-load this register with a value in order to
     * reduce the timeout period, say if you wanted to wake up
     * ever 4.0 seconds exactly.
     */
    TCNT1=0x0000;

    /* Configure the prescaler for 1:256,
     */
    TCCR1B = 0x04;

    /* Enable the timer overlow interrupt. */
    TIMSK1=0x01;
    
    timer1_millis = 0;
    timer1_fract = 0;
}

/*********************************************
 * Return the global time in millisseconds
 * Computed with timer1.
 * This time is real time whereas millis() return cpu time
 ********************************************/
unsigned long millis1()
{
    unsigned long m;
    unsigned int t;
    uint8_t oldSREG = SREG;

    // disable interrupts while we read timer0_millis or we might get an
    // inconsistent value (e.g. in the middle of a write to timer0_millis)
    cli();
    m = timer1_millis;
    t = TCNT1;
    SREG = oldSREG;
    m+= clockCyclesToMicroseconds(TIME1_PRESCALLER*(unsigned long)t) / 1000;

    return m;
}

/*****************************************************************
 * Sleep for at most k milliseconds in Idle modes
 * Three event put bach the microcontroller in normal mode:
 * - k milliseconds elapse
 * - The maximal waiting time elapse
 * - A serial event or interupt occurs
 *****************************************************************/
void sleepMillis(unsigned long k)
{
    unsigned long cycle =0;
    unsigned long remainingmicro = 0;
    if( k >= clockCyclesToMicroseconds(65536*TIME1_PRESCALLER)/1000 ){
        cycle = 0;
        remainingmicro = 0;
    } else {
        cycle = 65536 - k*1000/clockCyclesToMicroseconds(TIME1_PRESCALLER);
        remainingmicro = clockCyclesToMicroseconds(cycle*TIME1_PRESCALLER);
    }

    set_sleep_mode(SLEEP_MODE_IDLE);   // sleep mode is set here
    
    PORTD = SET_BOTH_LOW(7, 4);
    
    sleep_enable();          // enables the sleep bit in the mcucr register
                             // so sleep is possible. just a safety pin
    power_adc_disable();
    power_spi_disable();
    power_timer0_disable();
    power_timer2_disable();
    power_twi_disable();
    //sleep_bod_disable();

    uint8_t oldSREG = SREG;
    cli();
    unsigned long m = timer1_millis;
    m += clockCyclesToMicroseconds(TIME1_PRESCALLER*(unsigned long)TCNT1) / 1000;
    m -= remainingmicro/1000;
    timer1_millis = m;
    SREG = oldSREG;

    TCNT1 = cycle;
    sleep_mode();            // here the device is actually put to sleep!!

    // THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP
    sleep_disable();         // first thing after waking from sleep:
                             // disable sleep...
    power_all_enable();
}
