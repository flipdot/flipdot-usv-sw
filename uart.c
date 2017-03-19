//
// Created by malled on 3/19/17.
//
#include <avr/interrupt.h>
#include "uart.h"
#include "main.h"

#define T0_STOP() (TCCR0B &= ~(3))              // pause T0
#define T0_START() (TCCR0B |= 1)                // start T0, no prescaler
#define T0_RESET() (TCNT0 = 157)                // set T0 to ~100us

static volatile uint8_t flag = 0;

void uart_init(void){

    PORTB |= (1 << PIN_RPI);                    // idle high
    TCCR0B = 1;                                 // no prescaler
    TIFR0 = 0;                                  // clear all interrupt flags
    TIMSK0 = (1 << TOIE0);                      // enable TMR0 interrupt
    sei();                                      // enable interrupts
}

void uart_send(uint8_t tx){
    uint8_t i = 0;
    uint8_t val;
    T0_STOP();
    T0_RESET();
    PORTB &= ~(1 << PIN_RPI);                   // start bit
    T0_START();
    while (flag);
    flag = 1;
    for(i = 0; i < 8; i++) {
        if (tx & (1 << i)) {
            val = PORTB | (1 << PIN_RPI);
        } else {
            val = PORTB & ~(1 << PIN_RPI);
        }
        while(flag);
        PORTB = val;
        flag = 1;
    }
    val = PORTB | (1 << PIN_RPI);               // stop bit
    while (flag);
    PORTB = val;
    T0_STOP();
}

void uart_send_array(uint8_t *data, uint8_t len){
    while(len--){
        uart_send(*data++);
    }
}

void uart_send_string(char *str){
    while(*str){
        uart_send(*str++);
    }
}

ISR( TIM0_OVF_vect ) {
    flag = 0;
    T0_STOP();
    T0_RESET();
    T0_START();
}
