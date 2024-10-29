#include <avr/io.h>


/* I2C Status Bits in the TWSR Register */
#define TWI_START         0x08 /* start has been sent */
#define TWI_REP_START     0x10 /* repeated start */
#define TWI_MT_SLA_W_ACK  0x18 /* Master transmit ( slave address + Write request ) to slave + ACK received from slave. */
#define TWI_MT_SLA_R_ACK  0x40 /* Master transmit ( slave address + Read request ) to slave + ACK received from slave. */
#define TWI_MT_DATA_ACK   0x28 /* Master transmit data and ACK has been received from Slave. */
#define TWI_MR_DATA_ACK   0x50 /* Master received data and send ACK to slave. */
#define TWI_MR_DATA_NACK  0x58 /* Master received data but doesn't send ACK to slave. */

typedef uint16_t TWI_Address;
typedef uint8_t TWI_BaudRate;

typedef struct{
 TWI_Address address;
 TWI_BaudRate bit_rate;
}TWI_ConfigType;

void TWI_init ( const TWI_ConfigType * Config_Ptr){
	TWBR = Config_Ptr->bit_rate;//configurable bitrate
	TWSR = 0x00;
	TWAR = Config_Ptr->address; //specific address
	TWCR = (1<<TWEN); /* enable TWI */
}

void TWI_start(void){
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    //wait for TWINt flag set
	while(!(TWCR&(1<<TWINT))); 
}
void TWI_stop(void)
{
    /* 
	 * Clear the TWINT flag before sending the stop bit TWINT=1
	 * send the stop bit by TWSTO=1
	 * Enable TWI Module TWEN=1 
	 */
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}
void TWI_writeByte(uint8_t data)
{
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);

    while(!(TWCR&(1<<TWINT)));
}

uint8_t TWI_readByteWithACK(void)
{
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    // Wait for TWINT flag set 
    while(!(TWCR&(1<<TWINT)));
 
    return TWDR;
}


uint8_t TWI_readByteWithNACK(void)
{
    TWCR = (1 << TWINT) | (1 << TWEN);
    // wait for TWINT flag set 
    while(!(TWCR&(1<<TWINT)));
    
    return TWDR;
}

uint8_t TWI_getStatus(void)
{
    uint8_t status;
    status = TWSR & 0xF8;
    return status;
}
