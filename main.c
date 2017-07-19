//
// Created by malled on 3/19/17.
//

#include <avr/io.h>
#include <util/delay.h>
#include "main.h"
#include "adc.h"
#include "uart.h"

static void init(void);

#define AKKU_TYPE_NIMH
#define AKKU_TYPE_LEAD


#if defined(AKKU_TYPE_NIMH) && defined(AKKU_TYPE_LEAD)
    #error nope
#endif

#if defined(AKKU_TYPE_LEAD)
#define MIN_VOLTAGE 10500                       // min voltage for lead-gel batteries [mV]
#endif
#if defined(AKKU_TYPE_NIMH)
#define MIN_VOLTAGE 8000                        // min voltage for 8-cell NIMH batteries [mV]
#endif

typedef struct {
    uint16_t bat_voltage;
    uint16_t crc;
    union {
        struct{
            uint8_t ext_power : 1;              // external power supply connected
            uint8_t reserved  : 7;
        }flags;
        uint8_t value;
    } usv_state;
} USV_STATE;

static USV_STATE state;

int main(void){
    init();

    while(1) {
        for (int i = 0; i < 100; i++) {
            _delay_ms(10);
        }
        state.bat_voltage = adc_get_bat_voltage();
        if (state.bat_voltage < MIN_VOLTAGE) {
            //shutdown
            PORTB |= (1 << PIN_REL);
        }
        state.usv_state.value = 0;
        state.usv_state.flags.ext_power = (PORTB & (1 << PIN_EXT_POWER)) >> PIN_EXT_POWER;
        state.crc = 0;          // TODO
        uart_send_array((uint8_t*)&state, sizeof(USV_STATE));
    }
    return 42;
}


static void init(void){
    PORTB &= ~((1 << PIN_REL) | (1 << PIN_RPI));
    DDRB = (0 << PIN_EXT_POWER) | (1 << PIN_REL) | (1 << PIN_RPI);
    adc_init();
    uart_init();
}
