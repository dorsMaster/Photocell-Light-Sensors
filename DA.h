/*
 * DA.h
 *
 *  Created on: Feb 9, 2017
 *      Author: nem1
 */

#ifndef DA_H_
#define DA_H_

#include <basictypes.h>
#include <predef.h>
#include <ctype.h>
#include <startnet.h>
#include "error_wrapper.h"

// Sim1 Miscellaneous Control Register
#define MISCCR2_DAC1SEL 0x0040   // Enable DAC1 drive output
#define MISCCR2_DAC0SEL 0x0020   // Enable DAC0 drive output
#define MISCCR2_ADCEN   0x0010   // Enable ADC 6-4 and 2-0
#define MISCCR2_ADC7EN  0x0008   // Enable ADC 7
#define MISCCR2_ADC3EN  0x0004   // Enable ADC 3

// ADC Calibration Register
#define ADC_CAL_DAC1  0x0002     // Selects the source of the ADCA7 input as DAC1 output.
#define ADC_CAL_DAC0  0x0001     // Selects the source of the ADCA3 input as DAC0 output.

// DAC Control Register
#define DAC_CR_RESET 0x1101
#define DAC_CR_UP    0x0020
#define DAC_CR_DOWN  0x0010
#define DAC_CR_AUTO  0x0008
#define DAC_CR_SYNC  0x0004
#define DAC_CR_FMT   0x0002
#define DAC_CR_PDN   0x0001       // Power down. 0 = power on, 1 = power down

// DAC Trigger Select Register DACTSR
// Channels
#define DAC1_CH0  0x0000               // Source channel 0
#define DAC1_CH1  0x0800               // Source channel 1
#define DAC1_CH2  0x1000               // Source channel 2
#define DAC1_CH3  0x1800               // Source channel 3

#define DAC0_CH0  0x0000               // Source channel 0
#define DAC0_CH1  0x0008               // Source channel 1
#define DAC0_CH2  0x0010               // Source channel 2
#define DAC0_CH3  0x0018               // Source channel 3

// Sources
#define DAC1_SRC_PWMA      0x0000      // PWMs
#define DAC1_SRC_PWMB      0x0100
#define DAC1_SRC_PWMX      0x0200
#define DAC1_SRC_PWMTRIG1  0x0300
#define DAC1_SRC_PWMTRIG0  0x0400
#define DAC1_SRC_TnOUT     0x0500      // Timers
#define DAC1_SRC_TnIN      0x0600

#define DAC0_SRC_PWMA      0x0000      // PWMs
#define DAC0_SRC_PWMB      0x0001
#define DAC0_SRC_PWMX      0x0002
#define DAC0_SRC_PWMTRIG1  0x0003
#define DAC0_SRC_PWMTRIG0  0x0004
#define DAC0_SRC_TnOUT     0x0005      // Timers
#define DAC0_SRC_TnIN      0x0006

#ifndef WAIT_FOREVER
#define WAIT_FOREVER 0
#endif

class DA {
public:
	DA();
	virtual ~DA();
	void Init(void);				// Initializes the DA class
	void DACOutput(float volts);	// Outputs to DAC0
	void Lock(void);				// Lock grabs the DA for use.
	void Unlock(void);				// Unlocks the DA for use by another task
private:
	OS_SEM DA_lock_sem;
};

#endif /* DA_H_ */
