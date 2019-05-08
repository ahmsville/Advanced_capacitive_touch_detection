#ifndef AdvCapTouch_h
#define AdvCapTouch_h

/*
These capacitive touch detection library is written as an extension of the Arduino capacitive touch
library, it offers two detection algorithms that solves two of the main problems that occurs when
detecting capacitive touch on the Arduino.The library also allows advance touch detections; such
as, Double tap, Short press and Long press.Also featured in the library is a haptics controller, this
allow you to connect and control a vibration motor as a feedback when you interact with the
capacitive touch surface.

....Written By Ahmed Oyenuga (Ahmsville 2019).
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


	int input_type, input_duration = 0, input_reset = 0, averagingValue = 1000;
	int debounce = 2, doubleclickspeed, testsignalsize = 5;
	int singleclickthresh, shortpressthresh, longpressthresh;
	
	bool doubleclick;

	double detectionThreshold;
	double rejectionThreshold;

	double testvalue, changevalue, basevalue[4];
	int haptics_pin = 0, haptics_duration = 0, haptics_strength = 0;
	



public:

	AdvCapTouch();
	void initialize_capTouch (int numofpads);
	void set_inputTypeThresholds (int scl, int spr, int lpr, int dcls);   //sets the thresholds for the four input types  (singletap, shortpress, longpress, doubletapspeed)

	void set_detectionThreshold (double dth, double rth);  //set touch sensitivity in the form of detection, rejection thresholds values

	bool input_condition (int padnum);   //checks for valid capacitive touch

	void set_capTouchPins (int sp, int rp0, int rp1, int rp2, int rp3);  //sets the arduino pins used for the capacitive touch detection (sendpin, receivepin1, receivepin2, receivepin3, receivepin4)

	int detect_touch (int padnum);   //touch detection function
	int detect_touchFromNoise(int padnum);  //touch detection function for noisy signals

	double read_value (int padnum);
	double read_valueFromNoise (int padnum);  
	void update_basevalue (int padnum);

	void resetvalues ();
	void set_haptics(int pin, int duration, int strength);  //use to set haptics variables (arduino pwm pin, duration of haptics(ms), strength from 0-255)
	void haptics ();

	


};



#endif
