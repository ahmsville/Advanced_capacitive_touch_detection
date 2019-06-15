#include <AdvCapTouch.h>
/*Example sketch for detecting singletap, doubletap, shortpress and longpress from a capacitive thouch pad
 * this sketch allows touch detection from a noisy signal using the "detect_touchFromNoise()" function
 * connect 1M resistor between pin 3 & 4 on the arduino
 * connect your conductive surface to pin 4
 * connect viberation motor(haptics) switch circuit to pin 9 (optional)
 * open serial monitor to view touchtypes.
 * 
 *....By Ahmsville...
 */

AdvCapTouch samplepad  = AdvCapTouch();  //create a new captouch instance
int touchtype;

void setup() {
  samplepad.set_capTouchPins(3,4,0,0,0);   //set arduino pins associated with the pads (sendpin, receivepin1, receivepin2, receivepin3, receivepin4) this example uses just one pad.
  samplepad.set_adaptiveSensitivity(0, 0.7, true);  //set touch sensitivity to adaptive (very helpful for noisy signals)----"set_detectionThreshold()" can also be used instead. (pad, sensitivity(0.1 - 1), turn-on/off(true or false))
  samplepad.set_inputTypeThresholds(20, 40, 80, 150); // set the thresholds for the four input types  (singletap, shortpress, longpress, doubletapspeed)
   samplepad.set_haptics(9,40,255);  //set haptic feedback variables (arduino pwm pin, duration of haptics(ms), pwn strength from 0-255)------(optional)
  samplepad.initialize_capTouch(1);
  Serial.begin(9600);

}
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