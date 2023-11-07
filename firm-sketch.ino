#include <avr/io.h>
#include <avr/interrupt.h>

// Pin Definitions
#define LED_PIN PB5
#define TEMPERATURE_THRESHOLD 30

// Global Variables
volatile uint16_t adcValue;

// Timer1 Overflow Interrupt Service Routine
ISR(TIMER1_OVF_vect)
{
  // Reset the timer
  TCNT1 = 0;

  // Read ADC value
  ADCSRA |= (1 << ADSC); // Start ADC conversion
  while (ADCSRA & (1 << ADSC)); // Wait for conversion to complete

  adcValue = ADC; // Read 10-bit ADC value

  // Calculate temperature in degrees Celsius
  float temperature = (adcValue *(5/1023)) / 10000;

  // Control LED blinking rate based on temperature
  if (temperature < TEMPERATURE_THRESHOLD) {
    PORTB ^= (1 << LED_PIN); // Toggle LED every 250 ms
    TCCR1B = (1 << CS12) | (1 << CS10); // Set prescaler to 1024
  } else {
    PORTB ^= (1 << LED_PIN); // Toggle LED every 500 ms
    TCCR1B = (1 << CS12); // Set prescaler to 256
  }
}

void setup()
{
  // Set LED pin as an output
  DDRB = (1 << LED_PIN);

  // Setup ADC
  ADMUX = (1 << REFS0); // Set reference voltage to AVCC
  ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Enable ADC and set prescaler to 128

  // Setup Timer1
  TCCR1A = 0; // Normal mode
  TIMSK1 = (1 << TOIE1); // Enable overflow interrupt

  // Enable global interrupts
  sei();
}

void loop()
{
  // The main loop is empty as everything is handled by interrupts
}
