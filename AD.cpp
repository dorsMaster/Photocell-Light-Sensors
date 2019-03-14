/*
 * AD.cpp
 *
 *  Created on: Feb 9, 2017
 *      Author: nem1
 */

#include "AD.h"
#include <basictypes.h>
#include <sim.h>

AD::AD() {
	// TODO Auto-generated constructor stub

}

AD::~AD() {
	// TODO Auto-generated destructor stub
}

/* Name:Init
* Description: Initializes the entire register map
* of the AD converters
* Inputs: none
* Outputs: none
*/
void AD::Init(void){

	   volatile WORD vw;

	   //See MCF5441X RM Chapter 29
	   sim2.adc.cr1 = 0;
	   sim2.adc.cr2 = 0;
	   sim2.adc.zccr = 0;
	   sim2.adc.lst1 = 0x3210; //Ch 0....
	   sim2.adc.lst2 = 0x7654; //ch 7 in result 0..7
	   sim2.adc.sdis = 0; //All channels enabled
	   sim2.adc.sr = 0xFFFF;
	   for (int i = 0; i < 8; i++)
	   {
	      vw = sim2.adc.rslt[i];
	      sim2.adc.ofs[i] = 0;
	   }

	   sim2.adc.lsr = 0xFFFF;
	   sim2.adc.zcsr = 0xFFFF;

	   sim2.adc.pwr = 0; //Everything is turned on
	   sim2.adc.cal = 0x0000;
	   sim2.adc.pwr2 = 0x0005;
	   sim2.adc.div = 0x505;
	   sim2.adc.asdiv = 0x13;
}

/* Name:StartAD
* Description: Starts the AD conversion. Only a single sample is
* converted
* Inputs: none
* Outputs: none
*/
void AD::StartAD(void){
	 // Registers to modify for ex2
	//  sim2.adc.sr
	// Set start bit for SR
	sim2.adc.sr = ADC_SR_START;
	// 	sim2.adc.cr1
	// Set start bit for CR1
	sim2.adc.cr1 = ADC_CR1_START;

}
/* Name:StopAD
* Description: Turn off the AD converter
* Inputs: none
* Outputs: none
*/
void AD::StopAD(void)
{
	// Registers to modify for ex2
    //  sim2.adc.cr1 ;
	// Set stop bit for CR1
	sim2.adc.cr1 = ADC_CR1_STOP;
}
/* Name:ADDone
* Description: This method checks the status of
* the AD conversion.
* Inputs: none
* Outputs: Should return a boolean: true if the AD
* conversion is done and false if not done
*/
bool AD::ADDone(void)
{
	// Register to check for ex2
	//  sim2.adc.sr
	// Check status of sample0 if ready
	return (sim2.adc.sr & ADC_SR_DONE);
}
/* Name: GetADResult
* Description: Reads the result of the AD conversion once
* the status register indicates that the conversion is done is ADDone
* Inputs: none
* Outputs: none
*/
WORD AD::GetADResult(int ch) //Get the AD Result
{
	// Register to modify for ex2
	// sim2.adc.rslt[ch]

	// Shift result 3 bits to the right
	// to remove reserved R bits
	return sim2.adc.rslt[ch] >> 3;

}




