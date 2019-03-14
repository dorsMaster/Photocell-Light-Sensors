/*
 * LCD.cpp
 *
 *  Created on: Sep 22, 2016
 *      Author: Nancy Minderman
 *      nancy.minderman@ualberta.ca
 *      Much of the Initialization Code and raw
 *      font tables were taken from the Arduino code
 *      provided by Sparkfun
 *      https://www.sparkfun.com/products/10168
 *      Arduino sketch code
 *      http://playground.arduino.cc/Code/PCD8544
 */

#include "LCD.h"
#include "point.h"
#include <dspi.h>
#include <pins.h>
#include <basictypes.h>
#include <stdio.h>
#include <constants.h>
#include "error_wrapper.h"

#define __DEBUG_DSPI
const BYTE * blackBox = ASCII_7[96];  // Save black box sprite



LCD::LCD() {
	// TODO Auto-generated constructor stub

}

LCD::~LCD() {
	// TODO Auto-generated destructor stub
}

/* Initialize LCD hardware and DQSPI module*/
/* Name: Init
 * Description: Initializes both the SPI module on the MOD54415
 * and the LCD controller
 * Inputs: none
 * Outputs: none
 */

void LCD::Init(void) {

		init_spi();
		init_lcd();
}

/* Clear the display */
/* Name: Clear
 * Description: Clears the LCD by sending a screen full of space characters
 * Inputs: 48 X 84 pixels (504 Bytes) array of space characters. clear_array is in bitmaps.h
 * Outputs: none
 */
void LCD::Clear(void){
		send_data((BYTE *)clear_array, SCREEN_SIZE);
}

/* Invert every pixel on screen */
/* Name: Invert
 * Description: Inverts evry pixel on screen from white to black and vice versa
 * Note that the data in memory on the LCD does not change
 * Inputs: none
 * Outputs: none
 */
void LCD::Invert(void) {
	static BOOL inverted = false;
	if (inverted) {
		send_cmd(0x20);
		send_cmd(0x0C);

		inverted = false;
	} else {

		send_cmd(0x20);
		send_cmd(0x0D);

		inverted = true;
	}

}

/* Name: DrawBitmap
 * Description: Sends a bitmap of raw data to the screen starting at the upper left hand pixel of the screen
 * Array must be 504 Bytes in size.
 * Inputs: const BYTE * bitmap
 * Outputs: none
 */
void LCD::DrawBitmap(const BYTE * bitmap) {
	Home();
	send_data((BYTE *) bitmap, SCREEN_SIZE);
	send_cmd(0x20);
	send_cmd(0x0C);
}

/* Name: DrawString
 * Description: Sends a c-style string to the display at the current index. String must be null terminated
 * All elements in the string must be ASCII characters > space (0x20). Do not send
 * non-printable characters. Bad things will happen.
 * Inputs: char * str - address of first character in string. String will be displayed
 * at fixed locations on screen determined by points in ASCII_7 array in bitmaps.h
 * Font table is also in bitmaps.h
 * Outputs: none
 */
void LCD::DrawString(char * str) {

	BYTE index = 0;

	while ( (*(str)) != '\0'){
		index = *str - ASCII_BASE; // convert ASCII character to index in font table
		DrawChar((const BYTE *)&(ASCII_7[index]));
		str++;
	}
}
/* Name: DrawChar
 * Description: Draws a 7 pixel (width) by 8 pixel (height) sprite at the current location.
 * Use the font tables in bitmaps.h to select a sprite.
 * Inputs: const BYTE * ch -  an array of raw data that represents the character.
 * Outputs:
 */
void LCD::DrawChar(const BYTE * ch) {
	send_data((BYTE *) ch, CHAR_SIZE);
	send_cmd(0x20);
	send_cmd(0x0C);
}
/* Name:DrawChar
 * Description: Draws a 7 pixel(width) by 8 pixel( height) sprite at the provided location
 * Inputs:  const BYTE * ch -  an array of raw data that represents the character.
 * 			point loc must be one of the locations in the char_index array in bitmaps.h
 * Outputs: none
 */
void LCD::DrawChar(const BYTE * ch, point loc){
	move(loc);
	DrawChar(ch);
}

/* * Name: DrawBarGraph
 * Description: Displays a 7 X 8 box sprite to on the designated line
 * Inputs: BYTE line (valid from 0 - 5) BYTE length (valid from 0-11)
 * Outputs: none
 * Sprite is in bitmaps.h at index 96 ASCII array.
 */
void LCD::DrawBarGraph(BYTE line, BYTE length){
	Clear();  // Clear LCD Screen

	// Initialize current point
	point cur;
	cur.col = 0;
	cur.row = line;

	// Draw black box from start to level
	for (int i=0; i < length * 7; i = i + 7){
		cur.col = i;
		DrawChar(blackBox, cur);
	}


}

/* * Name: Test LCD
 * Description: Sends a fixed bitmap to the screen 84 X 48 pixels = 504 Bytes
 * Inputs: none
 * Outputs: none
 */
void LCD::TestLCD(void){
	Home();
	send_data((BYTE *) xkcdSandwich, SCREEN_SIZE);
}
	/* Name: Home
	 * Description: Move the internal counter of the LCD to the upper
	 * left hand point of the LCD
	 * Inputs: none
	 * Outputs: none
	 */
void LCD::Home(void) {
	point origin = char_index[LINE1_ORIGIN];
	move(origin);
}
/* Name: Move
 * Description: Moves te internal counter of the LCD to the lcoation specified
 * To write characters linmit the values of point to the members of the char_index array
 * in bitmaps.h. No checks are made. If you overflow the x and y counters you will get unexpected
 * results.
 * Inputs: point loc - location to move to
 * Outputs: none
 */
void LCD::Move(point loc) {
	move(loc);
}
/* Name: init_spi
 * Description: Initializes the SPI module on the 54415 to match the requirements of the
 * NOKIA 51150 with PCD8544 LCD controller
 * Inputs: none
 * Outputs: none
 */
void LCD::init_spi(void){

		LCD_CLOCK.function(PINJ2_25_DSPI1_SCK);  	// set SPI clock function on pin J2[25]
		LCD_DATA_OUT.function(PINJ2_28_DSPI1_SOUT); // set SPI Data out function on pin J2[28]
		LCD_RESET.function(PINJ2_26_GPIO);		  	// use this as a programmable reset for the LCD reset
		LCD_C_D_LINE.function(PINJ2_27_GPIO);		// use this as the D/C line 0 = command, 1 = data

		display_error("LCD::init_spi SemInit\n", OSSemInit(& DSPI_SEM, 0) );				// Use sem to be notified when transfer is finished.
		 /* Initialize DSPI options
		    void DSPIInit( BYTE SPIModule = DEFAULT_DSPI_MODLE, DWORD Baudrate = 2000000,
		                BYTE QueueBitSize = 0x8, BYTE CS = 0x0F,
		                BYTE CSPol = 0x0F, BYTE ClkPolarity = 0x0,
		                BYTE ClkPhase = 0x1, BOOL DoutHiz = TRUE,
		                BYTE csToClockDelay = 0, BYTE delayAfterTransfer = 0 ) */
		DSPIInit(DEFAULT_DSPI_MODULE, // DEFAULT is SPI 1 so OK
				2000000,// Speed
				8,		// Queue bit size = 8 bits
				0x0,	// CS set these to all off. No chip selects please.
				0x1,   	// CS Pol 1 = when CS is inactive, pin is pulled high
				0, 		// Clock polarity logic level when inactive. Set this to 0
				0,		// Clock phase to 0 means data is captured on the rising (or leading) edge of clock
				FALSE,   // DoutHiz should be true if the DOUT line needs to be in high impedance in between transfers. Both work in our case.
				0,		// Use default for now: -QCD is a value in the QDLYR register and will change the delay between the assertion of
						//the chip select and the start of the DSPI clock.  Default setting of one half DSPI clk
						// will be used if parameter is specified as 0x0 or not included.
				0		// Use default for now: DTL is a value in the QDLYR register and will change the delay following a transfer of
						//a single WORD in the DSPI queue.  Default reset value of 17/(fsys/2) will be used if
						//parameter is specified as 0x0 or not included.
				);
}
/* Name: send_data
 * Description: Sends data to the LCD, with size being the total number of bytes of data.
 * This will display it at the current location
 * Inputs: const Byte * data - data to be sent to screen in raw form. size - number of bytes to display
 * Outputs: none
 */
void LCD::send_data(const BYTE * data, WORD size) {

	LCD_C_D_LINE = 1; // 1 = data

	DSPIStart(DEFAULT_DSPI_MODULE,(BYTE *) data,
	            NULL, size, &DSPI_SEM); // send data via SPI bus
	display_error("LCD::send_data \n", OSSemPend( &DSPI_SEM, WAIT_FOREVER ));
}
/* Name: send_cmd
 * Description: Sends a command to the LCD. All possible commands are in
 * lcd.h
 * Inputs: BYTE command - valid LCD command from LCD.h
 * Outputs: none
 */
void LCD::send_cmd(BYTE command){


	LCD_C_D_LINE = 0;  // 0 = command

	// send command via the SPI bus
	//  commands are exactly 1 byte in size
	DSPIStart(DEFAULT_DSPI_MODULE, &command,
            NULL, 1, &DSPI_SEM);
	display_error("LCD::send_command \n", OSSemPend( &DSPI_SEM, WAIT_FOREVER ));
}


/* Name: init_lcd
 * Description: See cite above for sparkfun and arduino info. This method
 * sends initialization commands to the PCD 8544 LCD controller
 * Inputs: none
 * Outputs: none
 */
void LCD::init_lcd(void) {

	LCD_RESET = 0;
	OSTimeDly(1); // minimal possible delay
	LCD_RESET = 1;
	// Insert your ex 2 code modifications here
	// H = 1
	send_cmd(0x21);
	send_cmd(0xBA); // Changed contrast for LCD
	send_cmd(0x04);
	send_cmd(0x14);
	// H = 0
	send_cmd(0x20);
	send_cmd(0x0C);
	// End ex 2 modifications
	move(char_index[LINE1_ORIGIN]);
}
/* Name: move
 * Description: Set the internal counters of the LCDs to the location provided
 * non-ASCII character data:
 * point.x should be between 0 and 83
 * point.y should be between 0 and 5
 * ASCII character data:
 * point should be one of the char_index points in char_index in bitmaps.h
 * Inputs: point location - new location for internal location counters of LCD controller
 * Outputs: none
 */
void LCD::move(point loc) {
	send_cmd(0x20);
	send_cmd(0x40+loc.row);
	send_cmd(0x80+loc.col);
}
