#include<reg51.h>
sfr LCD = 0xA0; //P2 = LCD data pins
sbit RS=P0^5; //Register Select(RS) pin of 16*2 lcd
sbit RW=P0^6; //Read/write(RW) pin of 16*2 lcd
sbit EN=P0^7; //Enable(E) pin of 16*2 lcd

sfr ADC = 0x90; //P1 = ADC Data pins
sbit RD_ADC=P3^0; //Read(RD) pins of ADC 0804
sbit WR_ADC=P3^1; //Write(WR) pin of ADC0804
sbit INTR=P3^2;   //Interrupt(INTR) pin of ADC0804

void LCD_CMD(unsigned char x); //LCD command function
void LCD_DATA(unsigned char w); //LCD data functiom
void LCD_INI(void); //LCD initialized function
void Send_Data(unsigned char *str);
void msDelay(unsigned int); //function for creating delay

void convert_display(unsigned char); //function for Converting ADC value to temperature and dispaly it on 16X2 lcd display
unsigned char i, value;

void main(void) //main function
{
	msDelay(20); //delay
	LCD_INI();  //Call LCD initialized function
	Send_Data("Temperature"); //Data to send
	
	INTR=1; //make INTR pin as input
	RD_ADC=1; //set RD pin high
	WR_ADC=1; //set WR pin low
	
	while(1)  //repeat forever
	{
	
		WR_ADC=0; //send LOW to HIGH pulse on WR pin
		msDelay(1);
		WR_ADC=1;
		while(INTR==1); //wait for End of conversion
		RD_ADC=0; //make RD = 0 to read the data from ADC 0804
		value=ADC; //copy ADC data
		convert_display(value); //function call to convert ADC data to Temperature
		msDelay(1000); //interval between every cycles
		RD_ADC=1; //make RD = 1 for next cycle
	}

}

void convert_display(unsigned char value)
{
	unsigned char x1,x2,x3;
	
	LCD_CMD(0xc6); //command to set the cursor to 6th position of 2nd line on 16X2 LCD
	
	x1=(value/5); //divide the value by 5 and store the quotient in variable x1
	x1=x1+(0x30); //convert variable x1 to ascii by adding 0x30
	x2=value%5; //divide the value by 5 and store the remainder in variable x2
	x2=x2+(0x30); //convert variable x2 to ascii by adding 0x30
	x3=0xDF; //ascii value of degree
	
	LCD_DATA(x1); //display temperature on 16*2 lcd display
	LCD_DATA(x2);
	LCD_DATA(x3);
	LCD_DATA('C');
}

void LCD_CMD(unsigned char x)
{
	LCD = x; //Put X into LCD
	RS = 0; // RS is equal to 0
	RW = 0; // RW is equal to 0
	EN = 1; // EN is equal to 1
	msDelay(10); //delay
	EN = 0; //EN is equal to 0
	return; //return
}
void LCD_DATA(unsigned char w)
{
	LCD = w; //Put X into LCD
	RS = 1; // RS is equal to 1
	RW = 0; // RW is equal to 0
	EN = 1; // EN is equal to 1
	msDelay(1); //delay
	EN = 0; //EN is equal to 0
	return; //return
}
void Send_Data(unsigned char *Str)
{
	while(*Str) //stay while str is not finish
	LCD_DATA(*Str++); //call serTx
}

void LCD_INI(void)
{
	msDelay(100); //Delay 
	LCD_CMD(0x38); //Set function 00111000
	LCD_CMD(0x0E); //set display on/off 00001110
	LCD_CMD(0x01); //clear display 00000001
}
void msDelay(unsigned int Time)
{
	unsigned int y,z; //define variable
	for(y=0; y<Time; y++) // for loop for delay
	for(z=0; z<500; z++); // for loop for delay
}
