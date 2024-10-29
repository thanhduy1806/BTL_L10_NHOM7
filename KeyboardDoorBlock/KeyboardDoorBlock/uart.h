#include <avr/io.h>


// Initialize UART with baud rate 9600
void UART_init(unsigned int ubrr) {
    // Set baud rate
    UBRRH = (unsigned char)(ubrr>>8);
    UBRRL = (unsigned char)ubrr;
    // Enable receiver and transmitter
    UCSRB = (1<<RXEN) | (1<<TXEN);
    // Set frame format: 8 data bits, 2 stop bits
    UCSRC = (1<<USBS) | (3<<UCSZ0);
}

// Transmit data
void UART_transmit(unsigned char data) {
    // Wait for empty transmit buffer
    while (!(UCSRA & (1<<UDRE)));
    // Put data into buffer, sends the data
    UDR = data;
}

// Receive data
unsigned char UART_receive(void) {
    // Wait for data to be received
    while (!(UCSRA & (1<<RXC)));
    // Get and return received data from buffer
    return UDR;
}

// Send a string via UART
void UART_sendString(const char *str) {
    while(*str) {
        UART_transmit(*str++);
    }
}

