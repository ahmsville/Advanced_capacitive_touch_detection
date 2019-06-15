#ifndef AdvCapTouch_h
#define AdvCapTouch_h

/*
These capacitive touch detection library is written as an extension of the Arduino capacitive touch
library, it offers two detection algorithms that solves two of the main problems associated with
detecting capacitive touch on the Arduino.The library also allows advance touch detections; such
as, Double tap, Short press and Long press.Also featured in the library is a haptics controller, this
allow you to connect and control a vibration motor as a feedback when you interact with the
capacitive touch surface.

....By Ahmed Oyenuga (Ahmsville 2019).
*/

#include "Arduino.h"
#include <CapacitiveSensor.h>



class AdvCapTouch {
private:
	int sendpin, receivepin[4];
	CapacitiveSensor   pad0 = CapacitiveSensor(sendpin, receivepin[0]);        // 2M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil
	CapacitiveSensor   pad1 = CapacitiveSensor(sendpin, receivepin[1]);        // 2M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil
	CapacitiveSensor   pad2 = CapacitiveSensor(sendpin, receivepin[2]);        // 2M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil
	CapacitiveSensor   pad3 = CapacitiveSensor(sendpin, receivepin[3]);        // 2M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil


	int input_type, input_duration = 0, input_reset = 0, averagingValue = 50;
	int debounce = 2, doubleclickspeed, testsignalsize = 10, speed_noise = 5, speed_normal = 20, updatetimer = 0;
	int singleclickthresh, shortpressthresh, longpressthresh;
	int haptics_pin = 0, haptics_duration = 0, haptics_strength = 0;


	bool doubleclick, adaptsensitivity = false;

	double sensitivity = 1;  //controls master sensitivity in adaptive sensitivity mode.  range-(0.1 - 1)
	double detectionThreshold = 0.1;
	double rejectionThreshold;
	double changevalue[4] = { 0,0,0,0 }, basevalue[4] = {0,0,0,0}, maxsample[4], minsample[4];

public:

	AdvCapTouch();
	void initialize_capTouch (int numofpads);
	void set_inputTypeThresholds (int scl, int spr, int lpr, int dcls);   //sets the thresholds for the four input types  (singletap, shortpress, longpress, doubletapspeed)
	void set_detectionThreshold (double dth, double rth);  //set touch sensitivity in the form of detection, rejection thresholds values
	void set_adaptiveSensitivity(int padnum, bool act);  //adapts touch sensitivity depending on the noise level 
	void set_adaptiveSensitivity(int padnum, double mastersensitivity, bool act);  //adapts touch sensitivity depending on the noise level 
	bool input_condition (int padnum);   //checks for valid capacitive touch
	void set_capTouchPins (int sp, int rp0, int rp1, int rp2, int rp3);  //sets the arduino pins used for the capacitive touch detection (sendpin, receivepin1, receivepin2, receivepin3, receivepin4)

	/*--------------------------------------Capacitive Touch Detection Functions-------------------------------------------------------*/

	int detect_touch (int padnum);   //touch detection function
	int detect_touchFromNoise(int padnum);  //touch detection function for noisy signals
	int detect_touch_single(int padnum);  //single tap detection function
	int detect_touchFromNoise_single(int padnum);  //single tap detection function (noisy signals)
	double read_value(int padnum);  //read cap touchpad values (unprocessed signal)
	double read_valueFromNoise(int padnum); //read cap touchpad values (processed signal)

	/*---------------------------------------------------------------------------------------------------------------------------------*/

	void update_basevalue (int padnum);
	void update_basevalueFromNoise(int padnum);
	void update_basevalueSmooth(int padnum, double rate);
	void resetvalues ();
	void set_haptics(int pin, int duration, int strength);  //use to set haptics variables (arduino pwm pin, duration of haptics(ms), strength from 0-255)
	void haptics ();
	
	
	


};



#endif
