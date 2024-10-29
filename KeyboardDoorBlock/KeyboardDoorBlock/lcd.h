#include <avr/io.h>
#include <util/delay.h>
#define LCD_PRT PORTA
#define LCD_DDR DDRA
#define LCD_RS 0
#define LCD_RW 1
#define LCD_EN 2

void lcdCommand(unsigned char cmd)
{
	LCD_PRT = (LCD_PRT & 0x0F) | (cmd & 0xF0);
	LCD_PRT &= ~(1<<LCD_RS);
	LCD_PRT &= ~(1<<LCD_RW);
	LCD_PRT |= (1<<LCD_EN);
	_delay_us(20);
	LCD_PRT &= ~(1<<LCD_EN);
	
	_delay_us(20);
	
	LCD_PRT = (LCD_PRT & 0x0F) | (cmd<<4);
	LCD_PRT |= (1<<LCD_EN);
	_delay_us(20);
	LCD_PRT &= ~(1<<LCD_EN);
}

void lcdData(unsigned char data)
{
	LCD_PRT = (LCD_PRT & 0x0F) | (data & 0xF0);
	LCD_PRT |= (1<<LCD_RS);
	LCD_PRT &= ~(1<<LCD_RW);
	LCD_PRT |= (1<<LCD_EN);
	_delay_us(20);
	LCD_PRT &= ~(1<<LCD_EN);
	
	_delay_us(20);
	
	LCD_PRT = (LCD_PRT & 0x0F) | (data<<4);
	LCD_PRT |= (1<<LCD_EN);
	_delay_us(20);
	LCD_PRT &= ~(1<<LCD_EN);
}

void lcd_init()
{
	LCD_DDR = 0xFF;
	
	LCD_PRT &= ~(1<<LCD_EN);
	_delay_us(2000);
//	lcdCommand(0x33);
	//_delay_us(100);
	lcdCommand(0x32);
	_delay_us(100);
	lcdCommand(0x28);
	_delay_us(100);
	lcdCommand(0x0e);
	_delay_us(100);
	lcdCommand(0x01);
	_delay_us(2000);
	lcdCommand(0x06);
	_delay_us(100);
//	lcdCommand(0x0F); hiện màn hình, con trỏ chớp tắt 
	lcdCommand(0x0C);  //hiện màn hình tắt con trỏ  
	_delay_us(100);
}

void lcd_gotoxy (unsigned char col, unsigned char row)// function to position cursor
{
	unsigned char firstCharAdr[]={0x80,0xc0};
	lcdCommand(firstCharAdr[row-1]+col-1);
	_delay_us(100);
}

void lcd_print (char *str)
{
	unsigned char i = 0;
	while(str[i]!=0)
	{
		lcdData(str[i]);
		i++;
	}
}
void lcd_print_int (unsigned int value, int digits)
{
	unsigned int temp;
	unsigned int unit;
	unsigned int tens;
	unsigned int hundred;
	unsigned int thousand;
	unsigned int million;
	unsigned char flag=0;
	
	if(digits==5 || flag==1)
	{
		million=value/10000+48;
		lcdData(million);
		flag=1;
	}
	if(digits==4 || flag==1)
	{
		temp = value/1000;
		thousand = temp%10 + 48;
		lcdData(thousand);
		flag=1;
	}
	if(digits==3 || flag==1)
	{
		temp = value/100;
		hundred = temp%10 + 48;
		lcdData(hundred);
		flag=1;
	}
	if(digits==2 || flag==1)
	{
		temp = value/10;
		tens = temp%10 + 48;
		lcdData(tens);
		flag=1;
	}
	if(digits==1 || flag==1)
	{
		unit = value%10 + 48;
		lcdData(unit);
	}
	if(digits>5)
	{
		lcdData('E');
	}
	
}