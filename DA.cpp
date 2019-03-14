/*
 * DA.cpp
 *
 *  Created on: Feb 9, 2017
 *      Author: nem1
 */

#include "DA.h"
#include <sim.h>
#include <stdio.h>
#include <iosys.h>

DA::DA() {
	// TODO Auto-generated constructor stub

}

DA::~DA() {
	// TODO Auto-generated destructor stub
}

/* Name:Init
* Description: Initializes the DA converters
* Inputs: none
* Outputs: none
*/
void DA::Init(void){

	BYTE err = OS_NO_ERR;
	err = display_error("DA::Init Sem", OSSemInit(&DA_lock_sem,1));

	sim1.ccm.dactsr = 0x0;        // DAC trigger select register in CCM
	sim2.adc.cal = ADC_CAL_DAC0;  // Selects the source of the ADCA3 input as DAC0 output.
	sim2.dac[0].data = 0;         // Init data value to 0

	sim1.ccm.misccr2 &= ~(MISCCR2_ADC3EN);   // Disable ADC3 function on J2.9
	sim1.ccm.misccr2 |= MISCCR2_DAC0SEL;     // Enable DAC0 output drive on J2.9/Pin 4 of the
											 // AD/DA 10 pin connector

	sim2.dac[0].cr &= ~DAC_CR_PDN;           // Enable DAC0 output

}

/* Name:DACOutput
* Description: Method for sending output to the DA converter.
* Use DAC0
* Inputs: none
* Outputs: none
*/
void DA::DACOutput(float volts)
{
	// Modify the data register to send data out
	// Max unsigned 12 bit number
	// sim2.dac[X].data
	// Convert votls to maximum volt reading
	int dac_data = (int) (4096/3.3) * volts;
	// set data to unsigned volt data
	sim2.dac[0].data = (unsigned int) dac_data;

}

/* Name:Lock
* Description: Manages resource sharing of DAC0 to make sure
* only one task can use the DAC at any time. Call lock to make use of the
* DA converter
* Call Unlock once the DAC has finished writing.
* Inputs: none
* Outputs: none
*/
void DA::Lock(void){
	display_error("DA::Lock ", OSSemPend(&DA_lock_sem, WAIT_FOREVER));
}

/* Name:Unlock
* Description: Manages resource sharing of DAC0 to make sure
* only one task can use the DAC at any time. Call lock to make use of the
* DA converter
* Call Unlock once the DAC has finished writing.
* Inputs: none
* Outputs: none
*/
void DA::Unlock(void){
	display_error("DA::Unlock ", OSSemPost(&DA_lock_sem));
}
