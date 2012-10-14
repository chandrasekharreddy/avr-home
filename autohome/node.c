// This can run on several nodes, and get data to the central "poller" node.
// 2011-11-23 <jcw@equi4.com> http://opensource.org/licenses/mit-license.php
//
// Warning: this test will flood the radio band so nothing else gets through!
//
// To prepare for this test, you need to upload RF12demo to each pollee first,
// and set its node ID, group (77) and band (868 MHz). Node IDs must be in the
// range 1 .. NUM_NODES, as defined in poller.ino (all nodes must be present).

#include "config.h"
#include "rfm12_config.h"
#include <ds1621/ds1621.h>
#include <util/delay.h>
#include <string.h>
#include <avr/sleep.h>
#include <uart/uart.h>
#include <rfm12/rfm12.h>
#include <avr/interrupt.h>

#define UART_BAUD_RATE      57600      
#define POWER_SAVE 0



char buf[10];
int i = 0;
ISR(TIMER2_COMPA_vect){
   //wakeup
}

void send_data(){
    buf[0]='7';
    buf[1]='4';
    buf[2]='7';
    buf[3]='4';
    int temp = 0;
    temp = ds1621_getHrTemp();
    memset(buf+4,0,6);
    uint8_t start = 4;
    if (temp > 0xff) {
       buf[start] = '0';
       start++;       
    }
    itoa(temp,buf + start,16);
    uart_puts(buf);
    uart_puts("\r\n");

       
    uint8_t str[10];
    for (int i=0;i<10;i++){
       str[i] = buf[i];
    }
    rfm12_tx(10,0,str);
    for (int i=0;i<20;i++){
        rfm12_tick();
	_delay_ms(10);
    }
 

}

int main () {
    unsigned int c;
    uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,16000000L) ); 
    _delay_ms(100);
    sei();
    uart_puts("Starting temp node\r\n");
    rfm12_init();

    ds1621_init();

    for(;;){

       send_data();
       _delay_ms(5000);


    }
}

 
