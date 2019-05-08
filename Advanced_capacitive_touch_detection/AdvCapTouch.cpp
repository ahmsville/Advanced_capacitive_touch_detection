
#include "AdvCapTouch.h"

/*
These capacitive touch detection library is written as an extension of the Arduino capacitive touch
library, it offers two detection algorithms that solves two of the main problems that occurs when
detecting capacitive touch on the Arduino.The library also allows advance touch detections; such
as, Double tap, Short press and Long press.Also featured in the library is a haptics controller, this
allow you to connect and control a vibration motor as a feedback when you interact with the
capacitive touch surface.

....Written By Ahmed Oyenuga (Ahmsville 2019).
*/


AdvCapTouch::AdvCapTouch() {

}

void AdvCapTouch::initialize_capTouch(int numofpads) {

	pad0.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1
	pad1.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 2
	pad2.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 3
	pad3.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 4

	pinMode(sendpin, OUTPUT);

	for (int i = 0; i < numofpads; i++) {
		pinMode(receivepin[i], INPUT);
	}
	if (numofpads == 1) {
		for (int i = 0; i < averagingValue; i++) {     // calculates the average nominal value when the pad is untouched at startup
			testvalue = pad0.capacitiveSensor(30);
			basevalue[0] += (testvalue / averagingValue);
			delay(1);
		}
	}
	else if (numofpads == 2) {
		for (int i = 0; i < averagingValue; i++) {     // calculates the average nominal value when the pad is untouched at startup
			testvalue = pad0.capacitiveSensor(30);
			basevalue[0] += (testvalue / averagingValue);
			delay(1);
		}
		for (int i = 0; i < averagingValue; i++) {     // calculates the average nominal value when the pad is untouched at startup
			testvalue = pad1.capacitiveSensor(30);
			basevalue[1] += (testvalue / averagingValue);
			delay(1);
		}
	}
	else if (numofpads == 3) {
		for (int i = 0; i < averagingValue; i++) {     // calculates the average nominal value when the pad is untouched at startup
			testvalue = pad0.capacitiveSensor(30);
			basevalue[0] += (testvalue / averagingValue);
			delay(1);
		}
		for (int i = 0; i < averagingValue; i++) {     // calculates the average nominal value when the pad is untouched at startup
			testvalue = pad1.capacitiveSensor(30);
			basevalue[1] += (testvalue / averagingValue);
			delay(1);
		}
		for (int i = 0; i < averagingValue; i++) {     // calculates the average nominal value when the pad is untouched at startup
			testvalue = pad2.capacitiveSensor(30);
			basevalue[2] += (testvalue / averagingValue);
			delay(1);
		}
	}
	else if (numofpads == 4) {
		for (int i = 0; i < averagingValue; i++) {     // calculates the average nominal value when the pad is untouched at startup
			testvalue = pad0.capacitiveSensor(30);
			basevalue[0] += (testvalue / averagingValue);
			delay(1);
		}
		for (int i = 0; i < averagingValue; i++) {     // calculates the average nominal value when the pad is untouched at startup
			testvalue = pad1.capacitiveSensor(30);
			basevalue[1] += (testvalue / averagingValue);
			delay(1);
		}
		for (int i = 0; i < averagingValue; i++) {     // calculates the average nominal value when the pad is untouched at startup
			testvalue = pad2.capacitiveSensor(30);
			basevalue[2] += (testvalue / averagingValue);
			delay(1);
		}
		for (int i = 0; i < averagingValue; i++) {     // calculates the average nominal value when the pad is untouched at startup
			testvalue = pad3.capacitiveSensor(30);
			basevalue[3] += (testvalue / averagingValue);
			delay(1);
		}
	}
	
}


void AdvCapTouch::set_inputTypeThresholds(int scl, int spr, int lpr, int dcls) {  //sets the thresholds for the four input types  (singletap, shortpress, longpress, doubletapspeed)
	singleclickthresh = scl;
	shortpressthresh = spr;
	longpressthresh = lpr;
	doubleclickspeed = dcls;
	input_reset = longpressthresh + 100;
}


void AdvCapTouch::set_detectionThreshold(double dth, double rth) {   //set touch sensitivity in the form of detection, rejection thresholds values
	detectionThreshold = dth;
	rejectionThreshold = rth;
}


bool AdvCapTouch::input_condition(int padnum) {  //checks for valid capacitive touch
	if (changevalue > (basevalue[padnum] + detectionThreshold)) {
		return true;
	}
	else if (changevalue <= (basevalue[padnum] + rejectionThreshold)) {
		return false;
	}
}
void AdvCapTouch::set_capTouchPins(int sp, int rp0, int rp1, int rp2, int rp3) {  //sets the arduino pins used for the capacitive touch detection (sendpin, receivepin1, receivepin2, receivepin3, receivepin4)
	sendpin = sp;
	receivepin[0] = rp0;
	receivepin[1] = rp1;
	receivepin[2] = rp2;
	receivepin[3] = rp3;
	pad0 = CapacitiveSensor(sendpin, receivepin[0]);
	pad1 = CapacitiveSensor(sendpin, receivepin[1]);
	pad2 = CapacitiveSensor(sendpin, receivepin[2]);
	pad3 = CapacitiveSensor(sendpin, receivepin[3]);
}



int AdvCapTouch::detect_touch(int padnum) {  //touch type detection function
	read_value(padnum);
	if (input_condition(padnum) == true) {   //check for touch
		delay(debounce);   // debounce
		read_value(padnum);
		if (input_condition(padnum) == true) {   //check again
			haptics();   //turn on viberation motor to acknowledge touch
			input_type = 1;   //set input type as single tap but proceed to check for second touch
			delay(doubleclickspeed);  // double tap speed
			for (int i = 0; i < 50; i++) {  // detect second touch
				read_value(padnum);
				if (input_condition(padnum) == false) {
					delay(debounce);
					read_value(padnum);
					if (input_condition(padnum) == false) {
						doubleclick = true;
					}
				}
				else if (input_condition(padnum) == true && doubleclick == true) {  // set input type as double tap if second touch was detected.
					input_type = 2;
				}
			}
			read_value(padnum);
			if (input_condition(padnum) == true) {    // if touch pad is still being pressed
				read_value(padnum);
				if (input_condition(padnum) == true && doubleclick == true) {  // set input type as double tap if second touch was detected.
					input_type = 2;  //doubleclick	
				}
				else if (input_condition(padnum) == true && doubleclick != true) {  //else enter long press detection
					while (input_condition(padnum) == true && input_duration < input_reset) {  //calculate press lenght
						delay(debounce);
						input_duration++;
						read_value(padnum);
						
					}
					if (input_duration <= singleclickthresh && input_duration > 0) {  //touch is still a single tap
						input_type = 1;
					}
					else if (input_duration > singleclickthresh && input_duration <= shortpressthresh) {  //touch is a shortpress
						input_type = 3;
					}
					else if (input_duration > longpressthresh + 20) {    //touch is a longpress
						input_type = 4;
					}
					else {
						input_type = 0;   //invalid touch
					}
				
				}
			}
			else {
				///
			}
			return input_type;  // do something based on the input type
			
		}
		else {  // invalid input
	   ////
		}
	}
	else { 
		input_type = 0;   //invalid touch
		return input_type;
	}
}


int AdvCapTouch::detect_touchFromNoise(int padnum) {  //touch type detection function
	read_valueFromNoise(padnum);
	if (input_condition(padnum) == true) {   //check for touch
		delay(debounce);   // debounce
		read_valueFromNoise(padnum);
		if (input_condition(padnum) == true) {   //check again
			haptics();   //turn on viberation motor to acknowledge touch
			input_type = 1;   //set input type as single tap but proceed to check for second touch
			delay(doubleclickspeed);  // double tap speed
			for (int i = 0; i < 50; i++) {  // detect second touch
				read_valueFromNoise(padnum);
				if (input_condition(padnum) == false) {
					delay(debounce);
					read_valueFromNoise(padnum);
					if (input_condition(padnum) == false) {
						doubleclick = true;
					}
				}
				else if (input_condition(padnum) == true && doubleclick == true) {  // set input type as double tap if second touch was detected.
					input_type = 2;
				}
			}
			read_valueFromNoise(padnum);
			if (input_condition(padnum) == true) {    // if touch pad is still being pressed
				read_valueFromNoise(padnum);
				if (input_condition(padnum) == true && doubleclick == true) {  // set input type as double tap if second touch was detected.
					input_type = 2;  //doubleclick	
				}
				else if (input_condition(padnum) == true && doubleclick != true) {  //else enter long press detection
					while (input_condition(padnum) == true && input_duration < input_reset) {  //calculate press lenght
						delay(debounce);
						input_duration += 3;
						read_valueFromNoise(padnum);
						
					}
					if (input_duration <= singleclickthresh && input_duration > 0) {  //touch is still a single tap
						input_type = 1;
					}
					else if (input_duration > singleclickthresh && input_duration <= shortpressthresh) {  //touch is a shortpress
						input_type = 3;
					}
					else if (input_duration > longpressthresh + 20) {    //touch is a longpress
						input_type = 4;
					}
					else {
						input_type = 0;   //invalid touch
					}

				}
			}
			else {
				///
			}
			return input_type;  // do something based on the input type

		}
		else {  // invalid input
	   ////
		}
	}
	else {
		input_type = 0;   //invalid touch
		return input_type;
	}
}



double AdvCapTouch::read_value(int padnum) {  
	if (padnum == 0) {
		changevalue = pad0.capacitiveSensor(30);
	}
	else if (padnum == 1) {
		changevalue = pad1.capacitiveSensor(30);
	}
	else if (padnum == 2) {
		changevalue = pad2.capacitiveSensor(30);
	}
	else if (padnum == 3) {
		changevalue = pad3.capacitiveSensor(30);
	}
	return changevalue;
}

double AdvCapTouch::read_valueFromNoise(int padnum) {   //returns only peak value from a small sample of the signal
	if (padnum == 0) {
		int samplesize = testsignalsize;
		double samplevalues[samplesize];
		for (int i = 0; i < samplesize; i++) {
			samplevalues[i] = pad0.capacitiveSensor(30);    // save 10 sample values to the sample array
		}
		double maxsample = samplevalues[0];
		for (int j = 1; j < samplesize; j++) {    //find the maximum sample from those samples
			if (samplevalues[j] > maxsample) {
				maxsample = samplevalues[j];
			}
		}
		changevalue = maxsample;    //set maximum value as the changevalue
	}
	else if (padnum == 1) {
		int samplesize = testsignalsize;
		double samplevalues[samplesize];
		for (int i = 0; i < samplesize; i++) {
			samplevalues[i] = pad1.capacitiveSensor(30);    // save 10 sample values to the sample array
		}
		double maxsample = samplevalues[0];
		for (int j = 1; j < samplesize; j++) {    //find the maximum sample from those samples
			if (samplevalues[j] > maxsample) {
				maxsample = samplevalues[j];
			}
		}
		changevalue = maxsample;    //set maximum value as the changevalue
	}
	else if (padnum == 2) {
		int samplesize = testsignalsize;
		double samplevalues[samplesize];
		for (int i = 0; i < samplesize; i++) {
			samplevalues[i] = pad2.capacitiveSensor(30);    // save 10 sample values to the sample array
		}
		double maxsample = samplevalues[0];
		for (int j = 1; j < samplesize; j++) {    //find the maximum sample from those samples
			if (samplevalues[j] > maxsample) {
				maxsample = samplevalues[j];
			}
		}
		changevalue = maxsample;    //set maximum value as the changevalue
	}
	else if (padnum == 3) {
		int samplesize = testsignalsize;
		double samplevalues[samplesize];
		for (int i = 0; i < samplesize; i++) {
			samplevalues[i] = pad3.capacitiveSensor(30);    // save 10 sample values to the sample array
		}
		double maxsample = samplevalues[0];
		for (int j = 1; j < samplesize; j++) {    //find the maximum sample from those samples
			if (samplevalues[j] > maxsample) {
				maxsample = samplevalues[j];
			}
		}
		changevalue = maxsample;    //set maximum value as the changevalue
	}
	return changevalue;
}

void AdvCapTouch::update_basevalue(int padnum) {  // keeps base value updated by averaging
	basevalue[padnum] = (basevalue[padnum] + read_value(padnum)) * 0.5;
	resetvalues();
}


void AdvCapTouch::resetvalues() {
	input_type = 0;
	doubleclick = false;
	input_duration = 0;
}

void AdvCapTouch::set_haptics(int pin, int duration, int strength) {  //use to set haptics variables (arduino pwm pin, duration of haptics(ms), strength from 0-255)
	haptics_pin = pin;
	haptics_duration = duration;
	haptics_strength = strength;
}

void AdvCapTouch::haptics() {  //function for viberation feedback
	analogWrite(haptics_pin, haptics_strength);
	delay(haptics_duration);
	analogWrite(haptics_pin, 0);
}


