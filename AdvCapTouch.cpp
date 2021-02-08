
#include "AdvCapTouch.h"

/*
These capacitive touch detection library is written as an extension of the Arduino capacitive touch
library by Paul Bagder and Paul Stoffregen, it offers two detection algorithms that solves two of the main problems associated with
detecting capacitive touch on the Arduino.The library also allows advance touch detections; such
as, Double tap, Short press and Long press.Also featured in the library is a haptics controller, this
allow you to connect and control a vibration motor as a feedback when you interact with the
capacitive touch surface.

.... By Ahmed Oyenuga (Ahmsville 2019).
*/


AdvCapTouch::AdvCapTouch() {

}

void AdvCapTouch::initialize_capTouch(int numofpads) {

	set_adaptiveSensitivity(1, 0.00, true);  //set touch sensitivity to adaptive (very helpful for noisy signals)--- (number of pads, sensitivity(-1 to 1), turn-on/off(true or false))
	set_inputTypeThresholds(300, 700, 1100, 180); // set the thresholds for the four input types  (singletap, shortpress, longpress, doubletapspeed)

	pad0.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1
	pad1.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 2
	pad2.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 3
	pad3.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 4

	pinMode(sendpin, OUTPUT);

	for (int i = 0; i < numofpads; i++) {
		pinMode(receivepin[i], INPUT);
	}

}


void AdvCapTouch::set_inputTypeThresholds(int stp, int spr, int lpr, int dcls) {  //sets the thresholds for the four input types  (singletap, shortpress, longpress, doubletapspeed)
	singletaptresh = stp;
	shortpressthresh = spr;
	longpressthresh = lpr;
	doubleclickspeed = dcls;
}


void AdvCapTouch::set_adaptiveSensitivity(int padnum, bool act) {
	//read_valueFromNoise(padnum);
	detectionThreshold[padnum] = (maxsample[padnum] - minsample[padnum]);
	rejectionThreshold[padnum] = detectionThreshold[padnum];
	adaptsensitivity = act;
}

void AdvCapTouch::set_adaptiveSensitivity(int numofpads, float mastersensitivity, bool act) {
	//read_valueFromNoise(padnum);
	int padnum;
	if (numofpads == 1) {
		padnum = 0;
		detectionThreshold[padnum] = (maxsample[padnum] - minsample[padnum]);
		rejectionThreshold[padnum] = detectionThreshold[padnum];
	}
	else if (numofpads == 2) {
		padnum = 0;
		detectionThreshold[padnum] = (maxsample[padnum] - minsample[padnum]);
		rejectionThreshold[padnum] = detectionThreshold[padnum];
		padnum = 1;
		detectionThreshold[padnum] = (maxsample[padnum] - minsample[padnum]);
		rejectionThreshold[padnum] = detectionThreshold[padnum];
	}
	else if (numofpads == 3) {
		padnum = 0;
		detectionThreshold[padnum] = (maxsample[padnum] - minsample[padnum]);
		rejectionThreshold[padnum] = detectionThreshold[padnum];
		padnum = 1;
		detectionThreshold[padnum] = (maxsample[padnum] - minsample[padnum]);
		rejectionThreshold[padnum] = detectionThreshold[padnum];
		padnum = 2;
		detectionThreshold[padnum] = (maxsample[padnum] - minsample[padnum]);
		rejectionThreshold[padnum] = detectionThreshold[padnum];
	}
	else if (numofpads == 4) {
		padnum = 0;
		detectionThreshold[padnum] = (maxsample[padnum] - minsample[padnum]);
		rejectionThreshold[padnum] = detectionThreshold[padnum];
		padnum = 1;
		detectionThreshold[padnum] = (maxsample[padnum] - minsample[padnum]);
		rejectionThreshold[padnum] = detectionThreshold[padnum];
		padnum = 2;
		detectionThreshold[padnum] = (maxsample[padnum] - minsample[padnum]);
		rejectionThreshold[padnum] = detectionThreshold[padnum];
		padnum = 3;
		detectionThreshold[padnum] = (maxsample[padnum] - minsample[padnum]);
		rejectionThreshold[padnum] = detectionThreshold[padnum];
	}
	sensitivity = mastersensitivity;
	adaptsensitivity = act;
}

bool AdvCapTouch::input_condition(int padnum) {  //checks for valid capacitive touch
	
	if (changevalue[padnum] > (basevalue[padnum] - (detectionThreshold[padnum] * sensitivity))) {
		return true;
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




int AdvCapTouch::detect_touchFromNoise(int padnum) {  //touch type detection function
	
	if (detectionstate == 0) {
		read_valueFromNoise(padnum);
		if (input_condition(padnum) == true) {
			for (int i = 0; i < debounce; i++) { //debounce
				read_valueFromNoise(padnum);
			}
			read_valueFromNoise(padnum);
			if (input_condition(padnum) == true) {   //check again
				haptics(1);   //turn on viberation motor to acknowledge touch
				input_type = 1;   //set input type as single tap but proceed to check for second touch
				detectionstate = 1;
				time_touched = millis();
				//changevalue[padnum] += detectionThreshold[padnum];
			}
		}
	}
	else if (detectionstate == 1) {
		read_valueFromNoise(padnum);
		if (input_condition(padnum) == true) {   //check again
			haptics(0);
			long temptime = (millis() - time_touched);
			if (temptime > shortpressthresh) {  //shortpress
				input_type = 3;
			}
			if (temptime > longpressthresh && temptime < 4000) {
				input_type = 4;
				if (!longpresshaptics) {
					haptics(1); //turn on viberation to indicate long pressthresh
					longpresshaptics = true;
				}
				//returntouchtype = true;
			}
			if (temptime >= 4000) {  //extra long touch input for special operations
				input_type = 5;
				returntouchtype = true;
			}
		}
		else { //detected release
			if ((millis() - time_touched) > doubleclickspeed) {
				returntouchtype = true;
			}
			//Serial.println((millis() - time_touched));
			detectionstate = 2;
			changevalue[padnum] = minsample[padnum];
		}
	}

	else if (detectionstate == 2) {
		if (input_type > 2) {  //already set to shortpress or longpress
			returntouchtype = true;
		}
		else {
			haptics(0);
			read_valueFromNoise(padnum);
			if (input_condition(padnum) == true) {   
				for (int i = 0; i < debounce; i++) { //debounce
					read_valueFromNoise(padnum);
					haptics(0);
				}
				read_valueFromNoise(padnum);
				if (input_condition(padnum) == true) {   //check again
					//Serial.println((millis() - time_touched));
					if ((millis() - time_touched) > doubleclickspeed) {
						haptics(1);
						input_type = 2;
						returntouchtype = true;
					}
				}
			}
			else {
				if ((millis() - time_touched) > singletaptresh) { //double tap timeout
					returntouchtype = true;
				}
			}
		}
	}

	if (returntouchtype == true) {
		longpresshaptics = false;
		samplecounter = testsignalsize;
		changevalue[padnum] = minsample[padnum];  // avoid repititive detection
		returntouchtype = false;
		detectionstate = 0;
		tempdetectedinput = input_type;
		update_basevalueFromNoise(padnum);
		//Serial.println((millis() - time_touched));
		return tempdetectedinput;
	}
	else {
		if (detectionstate == 1 || detectionstate == 2) { //detection has started
			update_basevalueFromNoise(padnum);
			return -1;
		}
		update_basevalueFromNoise(padnum);
		return 0;
	}
	
}


int AdvCapTouch::detect_touchFromNoise_single(int padnum) {  //single tap detection function
	
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
	double rate = capvalueupdate_rate;  //cap value change rate
	
	if (padnum == 0) {
		tempread = pad0.capacitiveSensor(speed_noise);
		if (samplecounter < testsignalsize) {
			samplevalues[samplecounter] = tempread;   // save a small sample to the sample array
			samplecounter += 1;
		}
		
		

		//set  the changevalue(damping drastic changes)
		if (changevalue[padnum] > tempread) {
			changevalue[padnum] -= ((changevalue[padnum] - tempread) * rate);
		}
		else if (changevalue[padnum] < tempread) {
			changevalue[padnum] += ((tempread - changevalue[padnum]) * rate);
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
	//SerialUSB.print(basevalue[0]);
	//SerialUSB.print("\t");
	//SerialUSB.println(changevalue[padnum]);
	return changevalue[padnum];
}


void AdvCapTouch::update_basevalueFromNoise(int padnum) {  // keeps both the basevalue and the touch thresholds updated
	if (detectionstate == 0) {
		haptics(0);
		update_basevalueSmooth(padnum);
		resetvalues();

		//update touch thresholds (adaptive touch sensitivity)
		if (adaptsensitivity == true) {
			detectionThreshold[padnum] = (maxsample[padnum] - minsample[padnum]);			
			rejectionThreshold[padnum] = detectionThreshold[padnum];
			//minsample[padnum] = maxsample[padnum];
		}
	}
	
}

void AdvCapTouch::update_basevalueSmooth(int padnum) {   //Function to gradually update basevalue
	//
	if (samplecounter < testsignalsize) {
		samplevalues[samplecounter] = pad0.capacitiveSensor(speed_noise);    // save a small sample to the sample array
		samplecounter += 1;
	}
	else if (samplecounter == testsignalsize) { //find new max and min values
		samplecounter = 0;
		maxsample[padnum] = 0;
		minsample[padnum] = 9999999;
		for (int i = 0; i < testsignalsize; i++) {
			if (samplevalues[i] != 0) {  //ignore remaining samples if sample collection is terminated early
				if (samplevalues[i] > maxsample[padnum]) {
					maxsample[padnum] = samplevalues[i];
				}
				if (samplevalues[i] < minsample[padnum]) {
					minsample[padnum] = samplevalues[i];
				}
				samplevalues[i] = 0;
			}
			
		}
		basevalue[padnum] = maxsample[padnum];
	}
}

void AdvCapTouch::resetvalues() {
	if (detectionstate == 0) {
		input_type = 0;
	}
	
}

void AdvCapTouch::set_haptics(int pin, int duration, int strength) {  //use to set haptics variables (arduino pwm pin, duration of haptics(ms), strength from 0-255)
	haptics_pin = pin;
	haptics_duration = duration;
	haptics_strength = strength;
}

void AdvCapTouch::haptics(int state) {  //function for viberation feedback
	if (state == 1) {
		analogWrite(haptics_pin, haptics_strength);
		haptics_ontime = millis();
		haptics_state = 1;
	}
	else if (state == 3) {
		analogWrite(haptics_pin, (haptics_strength * 0.7));
	}
	else {
		haptics_offtime = millis();
		if (((haptics_offtime - haptics_ontime) >= haptics_duration) && haptics_state == 1) {
			analogWrite(haptics_pin, 0);
			haptics_state = 0;
		}
		
	}
	/*
	analogWrite(haptics_pin, haptics_strength);
	delay(haptics_duration);
	analogWrite(haptics_pin, 0);
	*/
}



void AdvCapTouch::show_levels(int padnum) {

	//detect_touchFromNoise(padnum);
	//input_condition(padnum);
	
	//Serial.print(detect_touchFromNoise(padnum));
	//Serial.print("\t");
	//Serial.print(input_condition(padnum));
	//detect_touchFromNoise(padnum);
	//Serial.print("\t");
	/*
	Serial.print(basevalue[padnum]);
	Serial.print("\t");
	Serial.print(detectionThreshold[padnum]);
	Serial.print("\t");
	Serial.println(read_valueFromNoise(padnum));
	*/
	
	update_basevalueFromNoise(0);
	//delay(20);
}



