#undef F_CPU
#define F_CPU 16000000 // 16 MHz CPU clock.

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define SOUND_SPEED 343.f // m/s
#define DISTANCE(echo) (SOUND_SPEED * echo/20000.f) // Echo measured in us, distance measured in cm.
// #define ECHO(distance) (unsigned long)(2000000.f * distance/SOUND_SPEED) // Unused.
#define MIN_DISTANCE 25.f // 25cm
#define MAX_DISTANCE 125.f // 125cm
#define DISTANCE_RANGE (MAX_DISTANCE - MIN_DISTANCE)
#define MAX_LED_FREQ 20.0f // 20Hz max
#define LED_FREQ(distance) (MAX_LED_FREQ - (MAX_LED_FREQ/DISTANCE_RANGE)*(distance - MIN_DISTANCE)) // distance in cm.

#define SETLED() PORTC |= (1 << 5)
#define CLRLED() PORTC &= ~(1 << 5)
#define TOGGLELED() PORTC ^= (1 << 5)

// System clock variable.
#define TICK_us 10U
volatile unsigned long micros_time;
#define WAIT_us(us) unsigned long start = micros_time;\
  while (micros_time - start < us)

// Global variables.
volatile unsigned long echo_start, echo_end, echo_time, loop_time;
float distance;

ISR(TIMER0_COMPA_vect) { // System clock. 1 tick every 10us.
  micros_time += (unsigned long)TICK_us;
}

ISR(TIMER1_COMPA_vect) {
  // Activate sensor.
  PORTC |= (1 << 0);
  _delay_us(50.0);
  PORTC &= ~(1 << 0);
}

ISR(INT0_vect) {
  PCIFR &= ~(1 << 2); // Disable PCINT2 ISR.
  echo_start = micros_time; // Get micros_time of activation.
}

ISR(PCINT2_vect) {
  echo_end = micros_time;
  echo_time = echo_end - echo_start; // Measure echo time.
  distance = DISTANCE(echo_time);
}

int main() {
  EICRA |= 0b00000011;
  EIMSK |= 0b00000001;
  PCICR |= 0b00000100;
  PCMSK2 = 0b00000100;

  /* TIMER/COUNTER0
  This will be the system clock.
  It ticks every 10us (defined in TICK_us). Use prescaler of 8, CTC with TOP = OCR0A = 19.
  Ideally, T = 10^(-5)s = P*(R+1)/16000000 -> 160 = 8*(R+1) -> R+1 = 20 -> R = 19
  Generalizing, for dt us/tick, R = 2*dt - 1.

  Registers will be:
  TCCR0A = 0b00000010; (CTC mode, TOP = OCR0A)
  TCCR0B = 0b00000010; (select prescaler of 8)
  OCR0A = 19; (as calculated)
  TIMSK0 = 0b00000010; (only COMPA interrupt)
  */
  TCCR0A |= 0b00000010;
  TCCR0B |= 0b00000010;
  OCR0A = 2*TICK_us - 1;
  TIMSK0 |= 0b00000010;
  
  /* TIMER/COUNTER1
  This will be used to measure distance every 200ms.
  It ticks every 200ms. Use a prescaler of 1024, CTC with TOP = OCR1A = 3124.
  Ideally, T = 0.2s = P*(R+1)/16000000 -> 3200000 = 1024*(R+1) -> R+1 = 3125 -> R = 3124

  Registers will be:
  TCCR1A = 0b00000000; (CTC mode with TOP = OCR1A doesn't require any bits from TCCR1A)
  TCCR1B = 0b00001101; (CTC mode with TOP = OCR1A, prescaler of 1024)
  OCR1A = 3124; (as calculted)
  TIMSK1 = 0b00000010; (only COMPA interrupt)
  */
  TCCR1B |= 0b00001101;
  OCR1A = 3124;
  TIMSK1 |= 0b00000010;

  DDRC |= (1 << 5) | (1 << 0);
  //DDRD &= ~(1 << PD2);

  sei();

  echo_time = 0, loop_time = micros_time;
  while (1) {
    // Distance computation.
    if (distance > MAX_DISTANCE) { // If obstacle is too far, keep LED off.
      CLRLED();
    } else if (distance > MIN_DISTANCE) { // If obstacle is within range, blink LED.
      TOGGLELED();
      WAIT_us(1000000.f/LED_FREQ(distance)); // I don't want to use the backward compatible flag, so this is the option that's left.
    } else { // If obstacle is too close, keep LED on.
      SETLED();
    }
  }

  return 0;
}