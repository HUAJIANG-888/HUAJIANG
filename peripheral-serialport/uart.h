#ifndef _UART_H_
#define _UART_H_


#include <libserialport.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int uart_led_init();
int uart_send(char *dat);
int check(enum sp_return result);
char *addr = "/dev/ttyS3";
extern int num_ports = 1;
char **port_names = &addr;
struct sp_port *ports[2];

#endif