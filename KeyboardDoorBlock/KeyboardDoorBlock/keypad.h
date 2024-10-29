#include <avr/io.h>
#include <util/delay.h>

#define KEYPAD_ROWS PORTB
#define KEYPAD_COLS PINC


char keys[4][4] = {
	{'*', '0', '=', '+'},
	{'1', '2', '3', '-'},
	{'4', '5', '6', 'X'},
	{'7', '8', '9', '/'}
	
};


void keypad_init() {

	DDRB = 0x0F; // PB0-PB3  output

	DDRC = 0x00; // PC4-PC7  input
	KEYPAD_COLS = 0xF0;
}


char keypad_get_key() {
	for (uint8_t row = 0; row < 4; row++) {
		// row low
		KEYPAD_ROWS &= ~(1 << row);
		_delay_ms(1); 

		// check col
		uint8_t col = KEYPAD_COLS & 0xF0;
		if (col != 0xF0) {
			for (uint8_t col_index = 0; col_index < 4; col_index++) {
				if (!(col & (1 << (col_index + 4)))) { 
					KEYPAD_ROWS |= (1 << row); // reset row 
					return keys[row][col_index]; 
				}
			}
		}
		KEYPAD_ROWS |= (1 << row); // reset row
	}
	return 0;
}