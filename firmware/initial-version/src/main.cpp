#undef F_CPU
#define F_CPU 16000000 // 16 MHz CPU clock.

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define SOUND_SPEED 343U

#define SETLED() PORTC |= (1 << 5)
#define CLRLED() PORTC &= ~(1 << 5)
#define TOGGLELED() PORTC ^= (1 << 5)

volatile unsigned int micros_time;
ISR(TIMER1_COMPA_vect) {
  micros_time += 10;
}

volatile unsigned int echo_start, echo_end, echo_time;
volatile unsigned char can_trigger;
unsigned char led;

ISR(INT0_vect) {
  PCIFR &= ~(1 << 2); // Disable PCINT2 ISR.
  echo_start = micros_time; // Get micros_time of activation.
  can_trigger = 0;
}

ISR(PCINT2_vect) {
  echo_end = micros_time; // Get time that has passed.
  echo_time = echo_end - echo_start;
  // TOGGLELED();

}

int main() {
  EICRA |= 0b00000011;
  EIMSK |= 0b00000001;
  PCICR |= 0b00000100;
  PCMSK2 = 0b00000100;

  TCCR1B |= 0b00001010;
  OCR1A = 10;
  TIMSK1 |= 0b00000010;

  DDRC |= (1 << PC5) | (1 << PC0);
  //DDRD &= ~(1 << PD2);

  can_trigger = 1;
  led = 0;

  sei();

  echo_time = 500;

  while (1) {
    if (can_trigger) {
      can_trigger = 0;
      PORTC |= (1 << PC0);
      _delay_us(50.0);
      PORTC &= ~(1 << PC0);
    }
    while (!can_trigger); // succ
    for (unsigned int i = 0; i < echo_time; ++i) {
      _delay_us(50.0);
    }
    PORTC ^= (1 << 5);
  }

  return 0;
}