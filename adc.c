//
// Created by malled on 3/18/17.
//
#include <avr/io.h>
#include "adc.h"
#include "main.h"

#define R1 330000                               // in r
#define R2 100000                               // in r
#define V_REF 5000                              // in mV

#define BAT_VOLTAGE_ADC 10                      // ADC2
#define ADC_ENABLE() (ADCSRA |= (1 << ADEN))
#define ADC_DISABLE() (ADCSRA &= ~(1 << ADEN))
#define ADC_START() (ADCSRA |= (1 << ADSC))

static uint16_t adc_read(uint8_t mux);

void adc_init(void){
    DIDR0 = (1 << ADC2D);                       // PB4 is analog input
    ADC_ENABLE();
}


uint16_t adc_get_bat_voltage(void){
    uint32_t voltage2;
    uint32_t voltage1;

    voltage2 = adc_read(BAT_VOLTAGE_ADC);
    voltage2 *= V_REF;
    voltage2 /= 1024;

    voltage1 = voltage2 * R1;
    voltage1 /= R2;

    return (uint16_t)(voltage2 + voltage1);
}


uint16_t adc_read(uint8_t mux){
    uint16_t adc;
    ADMUX = (0 << REFS0) | (mux);               // use VCC for rev voltage
    NOP();
    ADC_START();
    NOP();

    while (ADCSRA & (1 << ADSC));               // wait for conversion

    adc = ADCL;
    adc |= (ADCH << 8);
    return adc;
}


