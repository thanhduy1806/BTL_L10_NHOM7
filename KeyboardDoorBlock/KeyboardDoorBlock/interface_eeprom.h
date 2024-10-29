#include <twi.h>

uint8_t EEPROM_writeByte(uint16_t addr, uint8_t data)
{
    TWI_start();
    if (TWI_getStatus() != TWI_START)
        return 0;
		
    /* Send the device address, we need to get A8 A9 A10 address bits from the
     * memory location address and R/W=0 (write) */
    TWI_writeByte((uint8_t)(0xA0 | ((addr & 0x0700)>>7)));
    if (TWI_getStatus() != TWI_MT_SLA_W_ACK)
        return 0; 
		 
    /* Send the required memory location address */
    TWI_writeByte((uint8_t)(addr));
    if (TWI_getStatus() != TWI_MT_DATA_ACK)
        return 0;
		
    /* write byte to eeprom */
    TWI_writeByte(data);
    if (TWI_getStatus() != TWI_MT_DATA_ACK)
        return 0;

    /* Send the Stop Bit */
    TWI_stop();
	
    return 1;
}

uint8_t EEPROM_readByte(uint16_t addr, uint8_t *data)
{
	/* Send the Start Bit */
    TWI_start();
    if (TWI_getStatus() != TWI_START)
        return 0;
		
    /* Send the device address, we need to get A8 A9 A10 address bits from the
     * memory location address and R/W=0 (write) */
    TWI_writeByte((uint8_t)((0xA0) | ((addr & 0x0700)>>7)));
    if (TWI_getStatus() != TWI_MT_SLA_W_ACK)
        return 0;
		
    /* Send the required memory location address */
    TWI_writeByte((uint8_t)(addr));
    if (TWI_getStatus() != TWI_MT_DATA_ACK)
        return 0;
		
    /* Send the Repeated Start Bit */
    TWI_start();
    if (TWI_getStatus() != TWI_REP_START)
        return 0;
		
    /* Send the device address, we need to get A8 A9 A10 address bits from the
     * memory location address and R/W=1 (Read) */
    TWI_writeByte((uint8_t)((0xA0) | ((addr & 0x0700)>>7) | 1));
    if (TWI_getStatus() != TWI_MT_SLA_R_ACK)
        return 0;

    /* Read Byte from Memory without send ACK */
    *data = TWI_readByteWithNACK();
    if (TWI_getStatus() != TWI_MR_DATA_NACK)
        return 0;

    /* Send the Stop Bit */
    TWI_stop();

    return 1;
}
