/*
 * LCD.h
 *
 *  Created on: Sep 22, 2016
 *      Author: nem
 */

#ifndef LCD_H_
#define LCD_H_


#include <basictypes.h>
#include <predef.h>
#include <ucos.h>
#include "point.h"
#include "bitmaps.h"

// MOD54415 pins with MOD-Dev - 70

#define LCD_C_D_LINE 	J2[27]
#define LCD_DATA_OUT	J2[28]
#define LCD_CLOCK		J2[25]
#define LCD_RESET		J2[26]

#define ASCII_BASE 0x20

#ifndef WAIT_FOREVER
#define WAIT_FOREVER 0
#endif

/* Use these constants to change the send_command calls in LCD.cpp for
 * exercise 2
 * See page 14 of the Nokia 5110 datasheet
 */

// (H = 0 or H =  1) for these commands
#define CMD_FUNCTION_SET 	0x20
// PD options for function set command
#define OPT_CHIP_ACTIVE		0x0
#define OPT_CHIP_INACTIVE	0x4

// V options for function set command
#define OPT_HOR_ADDR		0x0
#define OPT_VERT_ADDR		0x2

// H Options for function set command
#define OPT_BASIC_INSTR		0x0
#define OPT_EXT_INSTR		0x1

// H = 0 to send these commands
#define CMD_DISPLAY_CONTROL 0x8
#define OPT_DISP_BLANK		0x0
#define OPT_NORMAL			0x4
#define OPT_ALL_SEGS_ON		0x1
#define OPT_INVERSE			0x5

// H = 0 to send these commands
#define CMD_SET_X			0x80
#define CMD_SET_Y			0x40

// H = 1 to send these commands
#define CMD_TEMP_CNTRL		0x4
#define OPT_VLCD_COEFF_0	0x0
#define OPT_VLCD_COEFF_1	0x1
#define OPT_VLCD_COEFF_2	0x2
#define OPT_VLCD_COEFF_3	0x3

// H = 1 to send these commands
#define CMD_BIAS_SYSTEM		0x10
#define OPT_N_7				0x0
#define OPT_N_6				0x1
#define OPT_N_5				0x2
#define OPT_N_4				0x3
#define OPT_N_3				0x4
#define OPT_N_2				0x5
#define OPT_N_1				0x6
#define OPT_N_0				0x7

// H = 1 to send these commands
#define CMD_SET_VOP			0x80
#define OPT_CONTRAST_LIGHT	0x30
#define OPT_CONTRAST_DARK	0x3A

class LCD {
public:
	LCD();
	virtual ~LCD();

	/* Initialize LCD hardware*/
	void Init(void);

	/* Clear the display pixel by pixel */
	void Clear(void);

	/* Invert the display using LCD command. Does not change data in memory */
	void Invert(void);

	/* Params should be pointer to array 504 bytes = (84 X 48 pixels)/8 */
	void DrawBitmap(const BYTE * bitmap);

	/* Draws an ASCII string at the current location */
	void DrawString(char * str);

	/* Draws a sprite/character at the current location */
	void DrawChar(const BYTE * ch);

	/* Draws a sprite/character at the location provided */
	void DrawChar(const BYTE * ch, point loc);

	/* Draw a bar graph on a single line */
	void DrawBarGraph(BYTE line, BYTE length);

	/* Test LCD */
	void TestLCD(void);

	/* Home */
	void Home(void);

	/* Move */
	void Move(point loc);

protected:
	OS_SEM DSPI_SEM;
	void init_spi(void);
	void init_lcd(void);
	void send_data(const BYTE * data, WORD size);
	void send_cmd(BYTE command);
	void move(point loc);
};

#endif /* LCD_H_ */
