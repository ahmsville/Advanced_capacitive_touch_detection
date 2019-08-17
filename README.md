# Advanced_capacitive_touch_detection

These capacitive touch detection library is an extension of the Arduino capacitive touch library by Paul Badger, it offers two detection algorithms that solves two of the main problems associated with detecting capacitive touch on the Arduino. The library also allows advanced touch detections; such as, Double tap, Short press and Long press. Also featured in the library is a haptics controller, this allows you to connect and control a vibration motor as a feedback when you interact with the capacitive touch surface.
See Video @ https://youtu.be/atb6d4b-VYk or read the included pdf.

```C
#include <AdvCapTouch.h>
/*Example sketch for detecting singletap, doubletap, shortpress and longpress from a capacitive thouch pad
 * this sketch allows touch detection from a noisy signal using the "detect_touchFromNoise()" function
 * connect 1M resistor between pin 3 & 4 on the arduino
 * connect your conductive surface to pin 4
 * connect vibration motor(haptics) switch circuit to pin 9 (optional)
 * open serial monitor to view touchtypes.
 * 
 *....By Ahmsville...
 */

AdvCapTouch samplepad  = AdvCapTouch();  //create a new captouch instance
int touchtype;

void setup() {
  samplepad.set_capTouchPins(3,4,0,0,0);   //set arduino pins associated with the pads (sendpin, receivepin1, receivepin2, receivepin3, receivepin4) this example uses just one pad.
  /************************************************************************************************************************************************************/
  samplepad.set_adaptiveSensitivity(1, 0.7, true);  //set touch sensitivity to adaptive (very helpful for noisy signals)----"set_detectionThreshold()" can also be used instead. (pad, sensitivity(0.1 - 1), turn-on/off(true or false))
  //samplepad.set_detectionThreshold(400, 100);  //set touch sensitivity in the form of detection, rejection thresholds values (manually set touch thresholds if you don’t want to use adaptive sensitivity)
  /************************************************************************************************************************************************************/
  samplepad.set_inputTypeThresholds(20, 40, 80, 150); // set the thresholds for the four input types  (singletap, shortpress, longpress, doubletapspeed)
   samplepad.set_haptics(9,40,255);  //set haptic feedback variables (arduino pwm pin, duration of haptics(ms), pwn strength from 0-255)------(optional)
  samplepad.initialize_capTouch(1);
  Serial.begin(9600);
}
```

The code snippet above shows how you would typically setup the library.

Line 2 is used to create a new capacitive touch object; the object is named “samplepad” but you can give any other name.

In the setup section of the code, Line one is used to configure the Arduino pins connected to the resistor and the touch surfaces. The code sample uses just one pin i:e one touch pad. You can have up to 4 pads connected, all of which will share the same send pin.

Line two calls the function set_adaptiveSensitivity(), this function gives your capacitive touch the ability to adapt its sensitivity to the noise level, the function accepts three parameters (number of pads, sensitivity(0.1 - 1), turn-on/off(true or false)).

The third line set_detectionThreshold(); can be used to set the sensitivity of the capacitive touch surface. The function accepts two integers, the touch detection threshold and the touch rejection threshold, the recommended ratio of detection to rejection threshold is 4:1. The larger the detection thresholds, the lower the pad sensitivity.

The forth line calls the function set_inputTypeThresholds(), this function is used to set the different detection levels of the four detectable capacitive touch types (single tap, short press, long press and double tap). Think of the numbers as how long your finger must be on the pad for specific touch types. For example, the code above sets 20 as the single tap threshold; this means any touch input that yields a count less than 20 will be categorized as a single tap, if the touch yields a count greater than 20 but less than 40, the touch is categorized as a short press, the same logic applies to the remaining touch types.

Line five is optional, only use this function when you have a haptics circuit setup.

Line six is used to initialize the capacitive touch, the function initialize_capTouch() accepts an integer from 1 – 4, representing the number of pads you’re working with.

# Detecting touch in the main program loop.

```C
void loop() {
touchtype = samplepad.detect_touchFromNoise(0);  //function return 1-4 based on the input detected, 1 = singletap, 2 = doubletap, 3 = shortpress, 4 = longpress
if (touchtype == 1) {
  Serial.println("Singletap");   
}
else if (touchtype == 2) {
  Serial.println("Doubletap");
}
else if (touchtype == 3) {
  Serial.println("Shortpress");
}
else if (touchtype == 4) {
  Serial.println("longpress");
}
samplepad.update_basevalueFromNoise(0);
}
```

There are four main functions for running the touch detection algorithms;

 1) detect_touch(padnum) - to be used for low noise touch setups
 2) detect_touchFromNoise(padnum) - to be used for noisy setups (recommended)
 3) detect_touch_single(padnum) - use this function to quickly detect single taps (low noise setup)
 4) detect_touchFromNoise_single(padnum) - use this function to quickly detect single taps (noisy setup, recommended)

the function above will return an integer value from 0 – 4.

 •	0 means no touch was detected.
 
 •	1 means a single tap was detected.
 
 •	2 means a double tap was detected.
 
 •	3 means a short press was detected.
 
 •	4 means a long press was detected.
 
For noisy touch setups, use the detect_touchFromNoise(padnum) function instead, this function is more reliable and works in the exact same way as detect_touch(); because this algorithm involves signal sampling, it is just a little bit slower than the alternative.

The second function update_basevalue(padnum) or update_basevalueFromNoise(padnum) in the loop is used to continuously update the nominal value of the touch pad. This function is integral to making the touch detection adaptable across various touch conditions.

