 /******************************************************************************

 * File Name: bitwise.h
 
 *******************************************************************************/
#include <avr/io.h>
#ifndef COMMON_MACROS
#define COMMON_MACROS

// Set bit 
#define SET_BIT(REG,BIT) (REG|=(1<<BIT))

// Clear bitr
#define CLEAR_BIT(REG,BIT) (REG &= (~(1<<BIT)))

// Toggle bit
#define TOGGLE_BIT(REG,BIT) (REG^=(1<<BIT))

// Rotate right the register value  
#define ROR(REG,num) ( REG= (REG>>num) | (REG<<(8-num)) )

// Rotate left the register value 
#define ROL(REG,num) ( REG= (REG<<num) | (REG>>(8-num)) )

//Ktr bit set
#define BIT_IS_SET(REG,BIT) ( REG & (1<<BIT) )

//Ktr bit clear
#define BIT_IS_CLEAR(REG,BIT) ( !(REG & (1<<BIT)) )

//Lấy 1 bit bất kỳ và clear về 0
#define GET_BIT(REG,BIT) ( ( REG & (1<<BIT) ) >> BIT )

#endif
