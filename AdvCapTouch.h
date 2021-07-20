#ifndef AdvCapTouch_h
#define AdvCapTouch_h

/*
These capacitive touch detection library is written as an extension of the Arduino capacitive touch
library by Paul Bagder, it offers two detection algorithms that solves two of the main problems associated with
detecting capacitive touch on the Arduino.The library also allows advance touch detections; such
as, float tap, Short press and Long press.Also featured in the library is a haptics controller, this
allow you to connect and control a vibration motor as a feedback when you interact with the
capacitive touch surface.

....By Ahmed Oyenuga (Ahmsville 2019).
*/

#include "Arduino.h"
#include <CapacitiveSensor.h>

#define detectmode 1 //damping detection
//#define detectmode 2 //standard deviation detection

/**********************************************************************************************************/

/**********************************************************************************************************/


class AdvCapTouch {
private:
	int sendpin, receivepin[4];
	CapacitiveSensor   pad0 = CapacitiveSensor(sendpin, receivepin[0]);        // 2M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil
	CapacitiveSensor   pad1 = CapacitiveSensor(sendpin, receivepin[1]);        // 2M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil
	CapacitiveSensor   pad2 = CapacitiveSensor(sendpin, receivepin[2]);        // 2M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil
	CapacitiveSensor   pad3 = CapacitiveSensor(sendpin, receivepin[3]);        // 2M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil


	int input_type, tempdetectedinput;
	int floatclickspeed, singletaptresh, samplecounter = 0, humpsamplecounter = 0, speed_noise = 3, speed_normal = speed_noise;
	int shortpressthresh, longpressthresh;
	int haptics_pin = 0, haptics_duration = 0, haptics_strength = 0, haptics_ontime, haptics_offtime, haptics_state;

	long spikereleasetime = 1000, spikedetecttime = 0, debounce = 50;

	float mean = 0, humpmean = 0, treshmean = 0, median = 0, humpmedian = 0, standarddeviation = 0;
	bool touchspikedetected = false;
	bool interruptValue_aquisition = false;
	float minsense = 5, maxsense = 100;

	float maxtouchrange = 100;

	float SDs_away = 0;
	float treshbasevalue = 0;

	float prevsignalsample = 0, prevhumpsample = 0;

	int detectionstate = 0;
	long time_touched = 0;
	bool returntouchtype = false;
	bool floatclick, adaptsensitivity = false;
	int testsignalsize = 30;
	float samplevalues[30];
	float sensitivity = 1.2, capvalueupdate_rate = 10;  //controls master sensitivity in adaptive sensitivity mode.  range-(0.1 - 1)
	float detectionThreshold[4] = { 0,0.1,0.1,0.1 };
	float rejectionThreshold[4] = {0,0,0,0};
	float changevalue[4] = { 0,0,0,0 }, basevalue[4] = { 0,0,0,0 }, maxsample[4] = {99999,99999,99999,99999}, minsample[4];
	bool longpresshaptics = false, shortpresshaptics = false, extralongpresshaptics = false;
	float tempread;

	bool repopulating = false;
	bool startWith_interrupt = true;

public:

	AdvCapTouch();
	AdvCapTouch(bool useint);
	void initialize_capTouch (int numofpads);
	void set_inputTypeThresholds (int stp, int spr, int lpr, int dcls);   //sets the thresholds for the four input types  (singletap, shortpress, longpress, floattapspeed)
	void set_adaptiveSensitivity(int padnum, bool act);  //adapts touch sensitivity depending on the noise level 
	void set_adaptiveSensitivity(int numofpads, float mastersensitivity, bool act);  //adapts touch sensitivity depending on the noise level 
	void set_SensitivityRange(int numofpads, float min, float max); //sensitivity range for standard deviation sensing
	
	void set_capTouchPins (int sp, int rp0, int rp1, int rp2, int rp3);  //sets the arduino pins used for the capacitive touch detection (sendpin, receivepin1, receivepin2, receivepin3, receivepin4)

	/*--------------------------------------Capacitive Touch Detection Functions-------------------------------------------------------*/

	int detect_touchFromNoise(int padnum);  //touch detection function for noisy signals
	int detect_touchFromNoise_single(int padnum);  //single tap detection function (noisy signals)
	float read_value(int padnum);  //read cap touchpad values (unprocessed signal)
	float read_valueFromNoise(int padnum); //read cap touchpad values (processed signal)
	void read_valueFromNoise_interrupt();

	/*---------------------------------------------------------------------------------------------------------------------------------*/

	void update_basevalueFromNoise(int padnum);
	void update_basevalueSmooth(int padnum);
	void resetvalues ();
	void set_haptics(int pin, int duration, int strength);  //use to set haptics variables (arduino pwm pin, duration of haptics(ms), strength from 0-255)
	void haptics (int state);
	void show_levels(int padnum);

	void getmean();
	void getmedian();
	void getstandarddeviation();
	bool standarddeviationtest(float testsample);

	void addSample(int padnum, float value);
	void fillhumparray(float samplevalue);
	bool checkRelease(int padnum);
	bool checkTouch(int padnum);
	bool repopulateSample(int padnum);


	
	
	


};



#endif
