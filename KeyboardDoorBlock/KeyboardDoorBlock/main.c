/*
 * KeyboardDoorBlock.c
 *
 * Created: 02/10/2024 7:41:54 CH
 * Author : ltkha
 */ 


#define F_CPU 8000000UL

#include <stdio.h>
#include <util/delay.h>
#include <avr/io.h>
#include <lcd.h>
#include <keypad.h>
#include <interface_eeprom.h>
#include <motor.h>
#include <uart.h>

#define MAX_INPUT_LEN 16
#define PASSWORD_SIZE 5
#define address_of_EEPROM  0x0001

int control_stage =0;
int stage = 0;
char pass_input[PASSWORD_SIZE] = {0};
char pass_1st[PASSWORD_SIZE]={0};
char pass_2st[PASSWORD_SIZE]={0};
char EEPROM_password[PASSWORD_SIZE]={0};
uint8_t input_index = 0;
uint8_t enter_doorOrChange_choice_return=0;
uint8_t tmp = 0;
uint8_t compare_passwords_return = 0;
uint8_t wrong_password_counter = 0;

void save_EEPROM(uint16_t first_address, char arr[], uint8_t size);
uint8_t compare_passwords(char compared_password_1[],char compared_password_2[]);
void enter_password(void);
void enter_or_change(void);
void readFromEEPROM(uint16_t first_address , uint8_t *arr,uint8_t size);
void choice(void);
	
int main(void)
{	keypad_init();
	lcd_init();
	//TWI_init();
	initMotor();
	UART_init(51);
	
	UART_sendString("UART Initialized. Send data:\r\n");
	
	lcd_gotoxy(1,1);
	lcd_print("Vui long nhap MK");
	lcd_gotoxy(1,2);
	lcd_print(":");
	while (1)
	{	
		switch (stage)
		{
			case 0: //create pass
				enter_password();
				_delay_ms(35); // Delay for debounce and readability
				break;
			case 1: //create pass 2
				enter_password();
				_delay_ms(35);
				break;
			case 2: //compare pass
				_delay_us(1);
				uint8_t compare_result =	compare_passwords(pass_1st,pass_2st); 
				_delay_ms(1);
				if (compare_result == 0){
					lcdCommand(0x01);
					lcd_print("passwords khong");
					lcd_gotoxy(4,2);
					lcd_print("khop!!!");
					
					_delay_ms(200);
					lcdCommand(0x01);
					lcd_gotoxy(1,1);
					lcd_print("Vui long nhap MK");
					lcd_gotoxy(1,2);
					
					stage =0 ; //reset stage to set password again
				}
				else if(compare_result ==1 ){
					lcdCommand(0x01);
					lcd_gotoxy(1,1);;
					lcd_print("Password khop <3");
					_delay_ms(200);
					save_EEPROM(address_of_EEPROM,pass_1st,PASSWORD_SIZE);
					stage = 3;
				//	control_stage = 1;
				}
				break;
			case 3: //display options for user
				lcdCommand(0x01);
				lcd_gotoxy(1,1);
				lcd_print("+ : mo cua");
				lcd_gotoxy(1,2);
				lcd_print("-:doi pass");
				_delay_ms(100);
					

				enter_or_change();
				tmp = enter_doorOrChange_choice_return;
				input_index =0;
				if (enter_doorOrChange_choice_return != 0) stage = 4;
				break;
			case 4: //pass to open 
				enter_password();
				_delay_ms(35);
				break;
			case 5:
				if(compare_passwords_return ==0){
					//pass dont match
					wrong_password_counter++;
					lcd_gotoxy(1,1);
					lcd_print("Password sai!!!!!");
					_delay_ms(100);
					lcdCommand(0x01);
					lcd_gotoxy(1,1);
					lcd_print("Vui long nhap MK");
					lcd_gotoxy(1,2);
					lcd_print(":");
					
					if(wrong_password_counter == 5)	{stage =6; break;}
					input_index = 0;
					stage =4;
				}
				else if(compare_passwords_return ==1 ){
					lcdCommand(0x01);
					lcd_gotoxy(1,1);
					lcd_print("MK chinh xac");
					_delay_ms(500);
					
					lcdCommand(0x01);
					lcd_gotoxy(1,1);
					choice();
				
				}
				break;
			case 8:
				enter_password();
				_delay_ms(35);
				break;
			case 6: //wrong pass 5 times
				lcdCommand(0x01);
				lcd_gotoxy(1,1);
				lcd_print("Ban da nhap MK");
				lcd_gotoxy(1,2);
				lcd_print("sai 5 lan");
				_delay_ms(500);
				lcdCommand(0x01);
				lcd_gotoxy(1,1);
				lcd_print("Vui long nhap lai");
				lcd_gotoxy(1,2);
				lcd_print("trong vong:");
				for (int i = 0; i<30; i++ ){
					lcd_gotoxy(1,2);
					lcd_print("trong vong:");
					lcd_print_int(30 - i,2);
					_delay_ms(250);
				}
				stage = 3;
				break;
//			case 7: //confirm one more
				//
				
		}
 	}
}


void enter_password(void){
 char key = keypad_get_key();
 if (key) {
	 if (input_index < MAX_INPUT_LEN) {
		 pass_input[input_index++] = key; // store
//		 pass_input[input_index] = '\0';

	 }
 }
 if (key != '*') {
	 lcd_gotoxy(2,2);
	 for (int i =0 ; i<input_index;i++)
	 lcd_print("*");}
	 if (input_index == 5) _delay_ms(100);
 if((input_index )   == PASSWORD_SIZE){
		if(stage ==0){
			//complete first input pass to set pass
			for (int i = 0; i<PASSWORD_SIZE; i++){
				pass_1st[i] = pass_input[i]  ;
			}			
			lcdCommand(0x01);
			lcd_gotoxy(1,1);
			lcd_print("Xin hay nhap lai");
			lcd_gotoxy(1,2);
			lcd_print(":");			
//			lcd_print(pass_input);
			stage =1; //
			input_index = 0;
		}
		else if (stage ==1){
			//complete second input pass to compare with first one
			for (int i = 0; i<PASSWORD_SIZE; i++){
				 pass_2st[i]		=	 pass_input[i] ;
			}
//			lcdCommand(0x01);
			
			stage =2;
			input_index = 0;
		}
		else if (stage ==3){
			lcdCommand(0x01);
//			input_index =0;
		}	
		else if (stage ==4){
			for (int i = 0; i<PASSWORD_SIZE; i++){
				pass_2st[i]		=	 pass_input[i] ;
			}
			readFromEEPROM(address_of_EEPROM,EEPROM_password,PASSWORD_SIZE);
			compare_passwords_return = compare_passwords(pass_2st,EEPROM_password);
//			if(compare_passwords_return == 0) 	wrong_password_counter++;
			stage =5;
			input_index = 0;
		}

// 		else {// pass nhap vao khong = 5digits
// 			lcdCommand(0x01);
// 			lcd_gotoxy(1,1);
// 			lcd_print("Error: PW bao gom");
// 			lcd_gotoxy(1,2);
// 			lcd_print("5 so");
 		}
//	}
// }
}
uint8_t compare_passwords(char compared_password_1[],char compared_password_2[]){
	for (int i = 0; i<PASSWORD_SIZE; i++){
		if(compared_password_1[i] != compared_password_2[i]){
			return 0;
		}else {
			asm("NOP");
	
		}
	}
			return 1;
}


void save_EEPROM(uint16_t first_address, char arr[], uint8_t size){
	for (int i =0; i<size; i++){
		EEPROM_writeByte(first_address+i,arr[i]);
		_delay_ms(10);
	}
}

void readFromEEPROM(uint16_t first_address , uint8_t *arr,uint8_t size){
	for (int i = 0; i<size; i++){
		EEPROM_readByte(first_address+i, &arr[i]);
		_delay_ms(10);
	}
}


void enter_or_change(void){
		char key = '\0';
	do { key = keypad_get_key();
	_delay_ms(35);} while(key == '\0');
	if(key =='+'){
		lcdCommand(0x01);
		lcd_gotoxy(1,1);
		lcd_print("Vui long nhap MK ");
		lcd_gotoxy(1,2);
		lcd_print(":");
		enter_doorOrChange_choice_return =2;
	}
	else if(key == '-'){
		lcdCommand(0x01);
		lcd_gotoxy(1,1);
		lcd_print("Vui long nhap MK ");
		lcd_gotoxy(1,2);
		lcd_print(":");
		enter_doorOrChange_choice_return =1;
	}
	else{
		lcdCommand(0x01);
		lcd_gotoxy(1,1);
		lcd_print("Error: Just only ");
		lcd_gotoxy(1,2);
		lcd_print("+ or - ");
		enter_doorOrChange_choice_return =0;
		_delay_ms(100);
		stage =3;
//		lcdCommand(0x01);
	}
}


void choice (void){
	if(tmp == 2){ //+
		lcdCommand(0x01);
		lcd_gotoxy(1,1);
		lcd_print("Cua dang mo ...");
		rotateMotorClockwise();
		_delay_ms(5000);
		//quay motor
		
		lcdCommand(0x01);
		lcd_gotoxy(1,1);
		lcd_print("Cua dang dong ...");
		rotateMotorCounterClockwise();
		_delay_ms(5000);
		stopMotor();
		lcdCommand(0x01);
		lcd_gotoxy(1,1);
		input_index = 0;
		stage =3;
	}
	else if (tmp ==1){ // -: change pass
		lcdCommand(0x01);
		lcd_gotoxy(1,1);
		lcd_print("Dang xu ly");
		_delay_ms(200);
		lcdCommand(0x01);
		lcd_gotoxy(1,1);
		lcd_print("Vui long nhap MK");
		lcd_gotoxy(1,2);
		lcd_print(":");
		input_index =0;
		stage =0;
		
	}
}