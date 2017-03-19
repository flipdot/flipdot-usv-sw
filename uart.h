//
// Created by malled on 3/19/17.
//

#ifndef FLIPDOT_USV_SW_UART_H
#define FLIPDOT_USV_SW_UART_H

void uart_init(void);
void uart_send(uint8_t tx);
void uart_send_array(uint8_t *data, uint8_t len);
void uart_send_string(char *str);

#endif //FLIPDOT_USV_SW_UART_H
