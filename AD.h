/*
 * AD.h
 *
 *  Created on: Feb 9, 2017
 *      Author: nem1
 */

#ifndef AD_H_
#define AD_H_

#include <basictypes.h>

#define ADC_CR1_STOP            0x4000
#define ADC_CR1_START           0x2000
#define ADC_CR1_LOOP            0x0002
#define ADC_SR_DONE				0x0001
#define ADC_SR_START            0x0000
#define ADC_CR2_INIT_OFF        0x0000
#define ADC_ZCCR_OFF            0x0000
#define ADC_SDIS_ENABLE_ALL     0x0000
#define ADC_SDIS_DISABLE_ALL    0x0000

class AD {
public:
	AD();
	virtual ~AD();
	void Init(void);     		// Setup the A/D converter to be ready to run
	void StartAD(void);  		// Start A/D conversion set.
	void StopAD(void);   		// Stop A/D conversion set.
	bool ADDone(void);			// Check the status of the conversion
	WORD GetADResult(int ch);   // Return the AD Result
};

#endif /* AD_H_ */
