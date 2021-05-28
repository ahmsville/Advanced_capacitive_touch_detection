
#include "AdvCapTouch.h"

/*
These capacitive touch detection library is written as an extension of the Arduino capacitive touch
library by Paul Bagder and Paul Stoffregen, it offers two detection algorithms that solves two of the main problems associated with
detecting capacitive touch on the Arduino.The library also allows advance touch detections; such
as, float tap, Short press and Long press.Also featured in the library is a haptics controller, this
allow you to connect and control a vibration motor as a feedback when you interact with the
capacitive touch surface.

.... By Ahmed Oyenuga (Ahmsville 2019).
*/


AdvCapTouch::AdvCapTouch() {

}

void AdvCapTouch::initialize_capTouch(int numofpads) {

	set_adaptiveSensitivity(1, 1.2, true);  //set touch sensitivity to adaptive (very helpful for noisy signals)--- (number of pads, sensitivity(-1 to 1), turn-on/off(true or false))
	set_inputTypeThresholds(300, 1000, 2000, 180); // set the thresholds for the four input types  (singletap, shortpress, longpress, floattapspeed)

	pad0.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1
	pad1.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 2
	pad2.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 3
	pad3.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 4

	pinMode(sendpin, OUTPUT);

	for (int i = 0; i < numofpads; i++) {
		pinMode(receivepin[i], INPUT);
	}

}


void AdvCapTouch::set_inputTypeThresholds(int stp, int spr, int lpr, int dcls) {  //sets the thresholds for the four input types  (singletap, shortpress, longpress, floattapspeed)
	singletaptresh = stp;
	shortpressthresh = spr;
	longpressthresh = lpr;
	floatclickspeed = dcls;
}


void AdvCapTouch::set_adaptiveSensitivity(int padnum, bool act) {
	//read_valueFromNoise(padnum);
	detectionThreshold[padnum] = (maxsample[padnum] - minsample[padnum]);
	rejectionThreshold[padnum] = detectionThreshold[padnum];
	adaptsensitivity = act;
}

void AdvCapTouch::set_SensitivityRange(int numofpads, float min, float max) {
	minsense = min;
	maxsense = max;
}

void AdvCapTouch::set_adaptiveSensitivity(int numofpads, float mastersensitivity, bool act) {
	sensitivity = mastersensitivity;
	adaptsensitivity = act;
}



bool AdvCapTouch::checkTouch(int padnum) {  //checks for valid capacitive touch

	if (detectionThreshold[padnum] == 0)
	{
		return false;
	}
	
		if (detectionstate == 0) {
			if (humpmean > (mean + (sensitivity * detectionThreshold[padnum]))) {
				if (touchspikedetected)
				{
					return false;
				}
				else {
					//SerialUSB.print(standarddeviation);
			//SerialUSB.print("\t");
			//SerialUSB.println(SDs_away);
					return true;
				}
			}
			else {
				return false;
			}
		}
		else {
			if (humpmean > (treshmean + (sensitivity * detectionThreshold[padnum]))) {
				return true;
			}
			else {
				return false;
			}
		}
	
	
	
	
}

bool AdvCapTouch::checkRelease(int padnum) {  //checks for valid capacitive touch

	
	if (humpmean < (mean - (3*detectionThreshold[padnum]))) {
		return true;
	}
	else {
		return false;
	}


}

bool AdvCapTouch::repopulateSample(int padnum) {  //checks for valid capacitive touch
	for (int i = 0; i < testsignalsize; i++) {
		samplevalues[i] = 9999999;
	}
	//repopulating = true;
}

void AdvCapTouch::getstandarddeviation() {
	double sampledeviationsum = 0;
	for (int i = 0; i < testsignalsize; i++) {
		sampledeviationsum += sq((samplevalues[i] - mean));
	}
	standarddeviation = sqrt((sampledeviationsum / testsignalsize));
}

void AdvCapTouch::getmedian() {
	//calc median
	double rearrangedarray[testsignalsize];
	memset(rearrangedarray, 0, sizeof(rearrangedarray));
	for (int i = 0; i < testsignalsize; i++) {
		double currentsample = samplevalues[i];
		int pos = 0;
		for (int j = 0; j < testsignalsize; j++) {
			if (samplevalues[j] >= currentsample) {
				pos += 1;
			}
			if (rearrangedarray[j] == currentsample) {
				pos -= 1;
			}
		}
		if (pos == 0) {
			pos = testsignalsize - 1;
			rearrangedarray[pos] = currentsample;
		}
		else {
			pos = (testsignalsize - pos);
			rearrangedarray[pos] = currentsample;
		}
	}

	if (testsignalsize % 2 == 0) {
		median = (rearrangedarray[testsignalsize / 2] + rearrangedarray[(testsignalsize / 2) - 1]) / 2;
	}
	else {
		median = rearrangedarray[(testsignalsize - 1) / 2];
	}
	//calc humpmedian
	double rearrangedhumparray[humpsignalsize];
	memset(rearrangedhumparray, 0, sizeof(rearrangedhumparray));
	for (int i = 0; i < humpsignalsize; i++) {
		double currentsample = humpsamplevalues[i];
		int pos = 0;
		for (int j = 0; j < humpsignalsize; j++) {
			if (humpsamplevalues[j] >= currentsample) {
				pos += 1;
			}
			if (rearrangedhumparray[j] == currentsample) {
				pos -= 1;
			}
		}
		if (pos == 0) {
			pos = humpsignalsize - 1;
			rearrangedhumparray[pos] = currentsample;
		}
		else {
			pos = (humpsignalsize - pos);
			rearrangedhumparray[pos] = currentsample;
		}
	}

	if (humpsignalsize % 2 == 0) {
		humpmedian = (rearrangedhumparray[humpsignalsize / 2] + rearrangedhumparray[(humpsignalsize / 2) - 1]) / 2;
	}
	else {
		humpmedian = rearrangedhumparray[(humpsignalsize - 1) / 2];
	}
}

void AdvCapTouch::getmean() {
	float samplesum = 0;
	float humpsamplesum = 0;
	if (mean >= 0) {
		for (int i = 0; i < testsignalsize; i++) {
			samplesum += samplevalues[i];
		}
		for (int i = 0; i < humpsignalsize; i++) {
			humpsamplesum += humpsamplevalues[i];
		}
		mean = (samplesum - humpsamplesum) / (testsignalsize - humpsignalsize);
		humpmean = humpsamplesum / humpsignalsize;
	}
	else {
		if ((samplecounter - 1) >= 0) {
			mean = ((mean * (testsignalsize - humpsignalsize)) - prevsignalsample + samplevalues[samplecounter - 1]) / (testsignalsize - humpsignalsize);
		}
		else {
			mean = ((mean * (testsignalsize - humpsignalsize)) - prevsignalsample + samplevalues[testsignalsize - 1]) / (testsignalsize - humpsignalsize);
		}
		if ((humpsamplecounter - 1) >= 0) {
			humpmean = ((humpmean * humpsignalsize) - prevhumpsample + humpsamplevalues[humpsamplecounter - 1]) / humpsignalsize;
		}
		else {
			humpmean = ((humpmean * humpsignalsize) - prevhumpsample + humpsamplevalues[humpsignalsize - 1]) / humpsignalsize;
		}	

	}
	
	
}

void AdvCapTouch::addSample(int padnum, float value) {
	if (samplecounter == touchspikesampleposition) {
		touchspikedetected = false;
	}

	if (samplecounter < testsignalsize) {
		prevsignalsample = samplevalues[samplecounter];
		samplevalues[samplecounter] = value;
		fillhumparray(value);
		samplecounter += 1;
	}
	if (samplecounter >= testsignalsize) {
		samplecounter = 0;
	}

	

}

void AdvCapTouch::fillhumparray(float samplevalue) {
	if (humpsamplecounter < humpsignalsize) {
		prevhumpsample = humpsamplevalues[humpsamplecounter];
		humpsamplevalues[humpsamplecounter] = samplevalue;
		humpsamplecounter += 1;
	}
	if (humpsamplecounter >= humpsignalsize) {
		humpsamplecounter = 0;
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
		if (checkTouch(padnum) == true) {
			for (int i = 0; i < debounce; i++) { //debounce
				read_valueFromNoise(padnum);
			}
			read_valueFromNoise(padnum);
			if (checkTouch(padnum) == true) {   //check again
				haptics(1);   //turn on viberation motor to acknowledge touch
				input_type = 1;   //set input type as single tap but proceed to check for second touch
				detectionstate = 1;
				treshmean = mean;
				time_touched = millis();
				//changevalue[padnum] += detectionThreshold[padnum];
			}
		}
	}
	else if (detectionstate == 1) {
		read_valueFromNoise(padnum);
		if (checkTouch(padnum) == true) {   //check again
			
			long temptime = (millis() - time_touched);
			if (temptime < 4000) {
				haptics(0);
			}
			if (temptime > shortpressthresh && temptime < longpressthresh) {  //shortpress
				input_type = 3;
				if (!shortpresshaptics) {
					haptics(1); //turn on viberation to indicate long pressthresh
					shortpresshaptics = true;
				}
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
				if (!extralongpresshaptics) {
					haptics(1); //turn on viberation to indicate long pressthresh
					extralongpresshaptics = true;
				}
				if (checkRelease(padnum)) {
					
					returntouchtype = true;
					haptics(0);
				}
				else {
					if (temptime >= 5000) {
						input_type = 0;
						returntouchtype = true;
					}
				}
			}
		}
		else { //detected release
			if ((millis() - time_touched) > floatclickspeed) {
				returntouchtype = true;
			}
			//Serial.println((millis() - time_touched));
			detectionstate = 2;
			//changevalue[padnum] = minsample[padnum];
		}
	}

	else if (detectionstate == 2) {
		if (input_type > 2) {  //already set to shortpress or longpress
			returntouchtype = true;
		}
		else {
			haptics(0);
			read_valueFromNoise(padnum);
			if (checkTouch(padnum) == true) {
				for (int i = 0; i < debounce; i++) { //debounce
					read_valueFromNoise(padnum);
					haptics(0);
				}
				read_valueFromNoise(padnum);
				if (checkTouch(padnum) == true) {   //check again
					//Serial.println((millis() - time_touched));
					if ((millis() - time_touched) > floatclickspeed) {
						haptics(1);
						input_type = 2;
						returntouchtype = true;
					}
				}
			}
			else {
				if ((millis() - time_touched) > singletaptresh) { //float tap timeout
					returntouchtype = true;
				}
			}
		}
	}

	if (returntouchtype == true) {
		longpresshaptics = false;
		shortpresshaptics = false;
		extralongpresshaptics = false;
		//samplecounter = testsignalsize;
		//changevalue[padnum] = minsample[padnum];  // avoid repititive detection
		detectionThreshold[padnum] = 999999;
		returntouchtype = false;
		detectionstate = 0;
		tempdetectedinput = input_type;
		update_basevalueFromNoise(padnum);
		//Serial.println((millis() - time_touched));
		return tempdetectedinput;
	}
	else {
		if (detectionstate != 0)
		{
			if (input_type < 3) { //detection has started
				update_basevalueFromNoise(padnum);
				return -1;
			}
		}
		update_basevalueFromNoise(padnum);
		return 0;
	}



}


int AdvCapTouch::detect_touchFromNoise_single(int padnum) {  //single tap detection function
	
}

float AdvCapTouch::read_value(int padnum) {
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

bool AdvCapTouch::standarddeviationtest(float testsample) {
	
	
	SDs_away = (testsample - mean) / standarddeviation;
	
	if (SDs_away > 15)
	{
		if (!touchspikedetected)
		{
			//SerialUSB.print(standarddeviation);
			//SerialUSB.print("\t");
			//SerialUSB.println(SDs_away);
			//delay(50);
			touchspikedetected = true;
			touchspikesampleposition = samplecounter;
		}
		
	}
    
	return true;
}


float AdvCapTouch::read_valueFromNoise(int padnum) {   //returns only peak value from a small sample of the signal

	float rate = capvalueupdate_rate;  //cap value change rate

	if (padnum == 0) {
		tempread = pad0.capacitiveSensor(speed_noise);
		
		//set  the changevalue(damping drastic changes)
		
		if (changevalue[padnum] > tempread) {
			//rate = 10/(changevalue[padnum] - tempread);
			
			changevalue[padnum] -= ((changevalue[padnum] - tempread) * rate);
			
		}
		else if (changevalue[padnum] < tempread) {
			//rate = 10/ (tempread - changevalue[padnum]);
			
			changevalue[padnum] += ((tempread - changevalue[padnum]) * rate);
			
		}
		standarddeviationtest(tempread);
		/*
		SerialUSB.print(tempread);
		SerialUSB.print("\t");
		SerialUSB.print(changevalue[padnum]);
		SerialUSB.print("\t");
		SerialUSB.println(rate);
		*/
		

	}

	//SerialUSB.print(basevalue[0]);
	//SerialUSB.print("\t");
	//SerialUSB.println(changevalue[padnum]);
	addSample(padnum, changevalue[padnum]);
	getmean();
	getstandarddeviation();

	

	//getmedian();
	return changevalue[padnum];

	
}


void AdvCapTouch::update_basevalueFromNoise(int padnum) {  // keeps both the basevalue and the touch thresholds updated
	if (detectionstate == 0) {
		//getmean();
		//getstandarddeviation();
		haptics(0);
		update_basevalueSmooth(padnum);
		resetvalues();
	}

}

void AdvCapTouch::update_basevalueSmooth(int padnum) {   //Function to gradually update basevalue
	//
	
		//tempread = pad0.capacitiveSensor(speed_noise);    // save a small sample to the sample array
		

	if (samplecounter == (testsignalsize-1)) { //find new max and min values
		
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
				
			}
			
		}
		//update touch thresholds (adaptive touch sensitivity)
		if (adaptsensitivity == true) {
			detectionThreshold[padnum] = (maxsample[padnum] - minsample[padnum]);
			rejectionThreshold[padnum] = detectionThreshold[padnum];
			//minsample[padnum] = maxsample[padnum];
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


	//read_valueFromNoise(padnum);
	//getmean();
	detect_touchFromNoise(padnum);
	//SerialUSB.print(changevalue[padnum]);
	//SerialUSB.print("\t");
	//SerialUSB.println(standarddeviation);
	//checkTouch(0);
	/*
	SerialUSB.print(mean);
	SerialUSB.print("\t");
	SerialUSB.print(humpmean);
	SerialUSB.print("\t");
	SerialUSB.println(changevalue[padnum]);
	*/
	
	//delay(50);
	
}


