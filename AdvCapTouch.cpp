
#include "AdvCapTouch.h"

/*
These capacitive touch detection library is written as an extension of the Arduino capacitive touch
library, it offers two detection algorithms that solves two of the main problems associated with
detecting capacitive touch on the Arduino.The library also allows advance touch detections; such
as, Double tap, Short press and Long press.Also featured in the library is a haptics controller, this
allow you to connect and control a vibration motor as a feedback when you interact with the
capacitive touch surface.

.... By Ahmed Oyenuga (Ahmsville 2019).
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

	int pad, temp;
	double testvalues[averagingValue];

	int hapticsdurationtemp = haptics_duration;
	int hapticsstrengthtemp = haptics_strength;
	set_haptics(haptics_pin, 100, 127);

	if (numofpads == 1) {
		pad = 0;
		maxsample[pad] = 0;
		minsample[pad] = 99999;
		temp = 1;
		while (temp != 0){
			temp = detect_touchFromNoise(pad);
			update_basevalueFromNoise(pad);
		}
	}
	else if (numofpads == 2) {
		pad = 0;
		maxsample[pad] = 0;
		minsample[pad] = 99999;
		temp = 1;
		while (temp != 0) {
			temp = detect_touchFromNoise(pad);
			update_basevalueFromNoise(pad);
		}
		pad = 1;
		maxsample[pad] = 0;
		minsample[pad] = 99999;
		temp = 1;
		while (temp != 0) {
			temp = detect_touchFromNoise(pad);
			update_basevalueFromNoise(pad);
		}
	}
	else if (numofpads == 3) {
		pad = 0;
		maxsample[pad] = 0;
		minsample[pad] = 99999;
		temp = 1;
		while (temp != 0) {
			temp = detect_touchFromNoise(pad);
			update_basevalueFromNoise(pad);
		}
		pad = 1;
		maxsample[pad] = 0;
		minsample[pad] = 99999;
		temp = 1;
		while (temp != 0) {
			temp = detect_touchFromNoise(pad);
			update_basevalueFromNoise(pad);
		}
		pad = 2;
		maxsample[pad] = 0;
		minsample[pad] = 99999;
		temp = 1;
		while (temp != 0) {
			temp = detect_touchFromNoise(pad);
			update_basevalueFromNoise(pad);
		}
	}
	else if (numofpads == 4) {
		pad = 0;
		maxsample[pad] = 0;
		minsample[pad] = 99999;
		temp = 1;
		while (temp != 0) {
			temp = detect_touchFromNoise(pad);
			update_basevalueFromNoise(pad);
		}
		pad = 1;
		maxsample[pad] = 0;
		minsample[pad] = 99999;
		temp = 1;
		while (temp != 0) {
			temp = detect_touchFromNoise(pad);
			update_basevalueFromNoise(pad);
		}
		pad = 2;
		maxsample[pad] = 0;
		minsample[pad] = 99999;
		temp = 1;
		while (temp != 0) {
			temp = detect_touchFromNoise(pad);
			update_basevalueFromNoise(pad);
		}
		pad = 3;
		maxsample[pad] = 0;
		minsample[pad] = 99999;
		temp = 1;
		while (temp != 0) {
			temp = detect_touchFromNoise(pad);
			update_basevalueFromNoise(pad);
		}
	}
	set_haptics(haptics_pin, hapticsdurationtemp, hapticsstrengthtemp);
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

void AdvCapTouch::set_adaptiveSensitivity(int padnum, bool act) {
	//read_valueFromNoise(padnum);
	detectionThreshold = (maxsample[padnum] - minsample[padnum]);
	rejectionThreshold = detectionThreshold;
	adaptsensitivity = act;
}

void AdvCapTouch::set_adaptiveSensitivity(int padnum, double mastersensitivity, bool act) {
	//read_valueFromNoise(padnum);
	detectionThreshold = (maxsample[padnum] - minsample[padnum]);
	rejectionThreshold = detectionThreshold;
	sensitivity = mastersensitivity;
	adaptsensitivity = act;
}

bool AdvCapTouch::input_condition(int padnum) {  //checks for valid capacitive touch
	if (changevalue[padnum] > (basevalue[padnum] + (detectionThreshold * sensitivity))) {
		return true;
	}
	else if (changevalue[padnum] < (basevalue[padnum] + (rejectionThreshold * sensitivity)) && changevalue[padnum] > (basevalue[padnum] - (rejectionThreshold * sensitivity))) {
		return false;
	}
	else {
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
					if ((input_duration <= singleclickthresh && input_duration > 0) || (input_duration < shortpressthresh && input_duration > singleclickthresh)) {  //touch is still a single tap
						input_type = 1;
					}
					else if (input_duration >= shortpressthresh && input_duration <= longpressthresh || (input_duration < (longpressthresh + 20) && input_duration > longpressthresh)) {  //touch is a shortpress
						input_type = 3;
					}
					else if (input_duration >= longpressthresh + 20) {    //touch is a longpress
						input_type = 4;
						basevalue[padnum] = read_value(padnum);
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
						input_duration += 1;
						read_valueFromNoise(padnum);

					}
					if ((input_duration <= singleclickthresh && input_duration > 0) || (input_duration < shortpressthresh && input_duration > singleclickthresh) ) {  //touch is still a single tap
						input_type = 1;
					}
					else if (input_duration >= shortpressthresh && input_duration <= longpressthresh || (input_duration < (longpressthresh + 20) && input_duration > longpressthresh)) {  //touch is a shortpress
						input_type = 3;
					}
					else if (input_duration >= longpressthresh + 20) {    //touch is a longpress
						input_type = 4;
						basevalue[padnum] = read_valueFromNoise(padnum);
					}
					else {
						input_type = 0;   //invalid touch
					}

				}
			}
			else {
				changevalue[padnum] = basevalue[padnum] - detectionThreshold;
				return input_type;
			}
			changevalue[padnum] = basevalue[padnum] - detectionThreshold;
			return input_type; 
		}
		else {  // invalid input
			input_type = 0;   //invalid touch
			return input_type;
		}
	}
	else {
		input_type = 0;   //invalid touch
		return input_type;
	}
}

int AdvCapTouch::detect_touch_single(int padnum) {  //single tap detection function
	read_value(padnum);
	if (input_condition(padnum) == true) {   //check for touch
		delay(debounce);   // debounce
		read_value(padnum);
		if (input_condition(padnum) == true) {   //check again
			haptics();   //turn on viberation motor to acknowledge touch
			input_type = 1;   //set input type as single tap 
			return input_type;  // return touch type (single tap)

		}
		else {  // invalid input
			input_type = 0;   //invalid touch
			return input_type;
		}
	}
	else {
		input_type = 0;   //invalid touch
		return input_type;
	}
}

int AdvCapTouch::detect_touchFromNoise_single(int padnum) {  //single tap detection function
	read_valueFromNoise(padnum);
	if (input_condition(padnum) == true) {   //check for touch
		delay(debounce);   // debounce
		read_valueFromNoise(padnum);
		if (input_condition(padnum) == true) {   //check again
			haptics();   //turn on viberation motor to acknowledge touch
			input_type = 1;   //set input type as single tap 
			return input_type;  // return touch type (single tap)

		}
		else {  // invalid input
			input_type = 0;   //invalid touch
			return input_type;
		}
	}
	else {
		input_type = 0;   //invalid touch
		return input_type;
	}
}

double AdvCapTouch::read_value(int padnum) {
	if (padnum == 0) {
		changevalue[padnum] = pad0.capacitiveSensor(speed_normal);
	}
	else if (padnum == 1) {
		changevalue[padnum] = pad1.capacitiveSensor(speed_normal);
	}
	else if (padnum == 2) {
		changevalue[padnum] = pad2.capacitiveSensor(speed_normal);
	}
	else if (padnum == 3) {
		changevalue[padnum] = pad3.capacitiveSensor(speed_normal);
	}
	return changevalue[padnum];
}

double AdvCapTouch::read_valueFromNoise(int padnum) {   //returns only peak value from a small sample of the signal
	double rate = 0.5;
	double samplevalues[testsignalsize];
	if (padnum == 0) {
		maxsample[padnum] = 0;
		for (int i = 0; i < testsignalsize; i++) {
			samplevalues[i] = pad0.capacitiveSensor(speed_noise);    // save a small sample to the sample array

		  //find the maximum and minimum values from those samples
			if (samplevalues[i] == 0) {

			}
			else if (samplevalues[i] > maxsample[padnum]) {
				maxsample[padnum] = samplevalues[i];
			}
			else if (samplevalues[i] < minsample[padnum]) {
				minsample[padnum] = samplevalues[i];
			}
		}
		//set maximum value as the changevalue(damping drastic changes)
		if (changevalue[padnum] > maxsample[padnum]) {
			changevalue[padnum] -= ((changevalue[padnum] - maxsample[padnum]) * rate);
		}
		else if (changevalue[padnum] < maxsample[padnum]) {
			changevalue[padnum] += ((maxsample[padnum] - changevalue[padnum]) * rate);
		}
		else {

		}
	}
	else if (padnum == 1) {
		maxsample[padnum] = 0;
		for (int i = 0; i < testsignalsize; i++) {
			samplevalues[i] = pad1.capacitiveSensor(speed_noise);    // save a small sample to the sample array

		  //find the maximum and minimum values from those samples
			if (samplevalues[i] == 0) {

			}
			else if (samplevalues[i] > maxsample[padnum]) {
				maxsample[padnum] = samplevalues[i];
			}
			else if (samplevalues[i] < minsample[padnum]) {
				minsample[padnum] = samplevalues[i];
			}
		}
		//set maximum value as the changevalue(damping drastic changes)
		if (changevalue[padnum] > maxsample[padnum]) {
			changevalue[padnum] -= ((changevalue[padnum] - maxsample[padnum]) * rate);
		}
		else if (changevalue[padnum] < maxsample[padnum]) {
			changevalue[padnum] += ((maxsample[padnum] - changevalue[padnum]) * rate);
		}
		else {

		}
	}
	else if (padnum == 2) {
		maxsample[padnum] = 0;
		for (int i = 0; i < testsignalsize; i++) {
			samplevalues[i] = pad2.capacitiveSensor(speed_noise);    // save a small sample to the sample array

		  //find the maximum and minimum values from those samples
			if (samplevalues[i] == 0) {

			}
			else if (samplevalues[i] > maxsample[padnum]) {
				maxsample[padnum] = samplevalues[i];
			}
			else if (samplevalues[i] < minsample[padnum]) {
				minsample[padnum] = samplevalues[i];
			}
		}
		//set maximum value as the changevalue(damping drastic changes)
		if (changevalue[padnum] > maxsample[padnum]) {
			changevalue[padnum] -= ((changevalue[padnum] - maxsample[padnum]) * rate);
		}
		else if (changevalue[padnum] < maxsample[padnum]) {
			changevalue[padnum] += ((maxsample[padnum] - changevalue[padnum]) * rate);
		}
		else {

		}
	}
	else if (padnum == 3) {
		maxsample[padnum] = 0;
		for (int i = 0; i < testsignalsize; i++) {
			samplevalues[i] = pad3.capacitiveSensor(speed_noise);    // save a small sample to the sample array

		  //find the maximum and minimum values from those samples
			if (samplevalues[i] == 0) {

			}
			else if (samplevalues[i] > maxsample[padnum]) {
				maxsample[padnum] = samplevalues[i];
			}
			else if (samplevalues[i] < minsample[padnum]) {
				minsample[padnum] = samplevalues[i];
			}
		}
		//set maximum value as the changevalue(damping drastic changes)
		if (changevalue[padnum] > maxsample[padnum]) {
			changevalue[padnum] -= ((changevalue[padnum] - maxsample[padnum]) * rate);
		}
		else if (changevalue[padnum] < maxsample[padnum]) {
			changevalue[padnum] += ((maxsample[padnum] - changevalue[padnum]) * rate);
		}
		else {

		}
	}
	return changevalue[padnum];
}

void AdvCapTouch::update_basevalue(int padnum) {  // keeps base value updated by averaging
	basevalue[padnum] = (basevalue[padnum] + read_value(padnum)) * 0.5;
	resetvalues();
}

void AdvCapTouch::update_basevalueFromNoise(int padnum) {  // keeps both the basevalue and the touch thresholds updated
	double rate = 0.1;
	update_basevalueSmooth(padnum, rate);
	resetvalues();

	//update touch thresholds (adaptive touch sensitivity)
	if (adaptsensitivity == true) {
		double temp = (maxsample[padnum] - minsample[padnum]);
		if (temp > detectionThreshold) {
			detectionThreshold += ((temp - detectionThreshold) * rate);
		}
		else if (temp < detectionThreshold) {
			detectionThreshold -= ((detectionThreshold - temp) * rate);
		}
		else {
			detectionThreshold = temp;
		}
		rejectionThreshold = detectionThreshold;
		minsample[padnum] = maxsample[padnum];
	}
	/*
	Serial.print(basevalue[padnum]);
	Serial.print("\t");
	Serial.print(detectionThreshold);
	Serial.print("\t");
	*/
}

void AdvCapTouch::update_basevalueSmooth(int padnum, double rate) {   //Function to gradually update basevalue
	maxsample[padnum] = 0;
	double samplevalues[testsignalsize];
	for (int i = 0; i < testsignalsize; i++) {
		samplevalues[i] = pad0.capacitiveSensor(speed_noise);    // save a small sample to the sample array

	  //find the maximum and minimum values from those samples
		if (samplevalues[i] == 0) {

		}
		else if (samplevalues[i] > maxsample[padnum]) {
			maxsample[padnum] = samplevalues[i];
		}
		else if (samplevalues[i] < minsample[padnum]) {
			minsample[padnum] = samplevalues[i];
		}
	}

	//calculate direction of change and update basevalue
	if (basevalue[padnum] > maxsample[padnum]) {
		basevalue[padnum] -= ((basevalue[padnum] - maxsample[padnum]) * rate);
	}
	else if (basevalue[padnum] < maxsample[padnum]) {
		basevalue[padnum] += ((maxsample[padnum] - basevalue[padnum]) * rate);
	}
	else {
		basevalue[padnum] = maxsample[padnum];
	}
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



