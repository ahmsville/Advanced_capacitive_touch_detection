#include <AdvCapTouch.h>
/*Example sketch for toggling and blinking an LED by single tap and double tap
 * 
 * connect 1M resistor between pin 3 & 4 on the arduino
 * connect your conductive surface to pin 4
 * connect LED to pin 5
 * connect viberation motor(haptics) switch circuit to pin 9 (optional)
 *
 *....Written By Ahmed Oyenuga (Ahmsville 2019).
 */

#define LED 5  //define led pin

AdvCapTouch samplepad  = AdvCapTouch();  //create a new captouch instance
int touchtype;
bool ledstate = 0;

void setup() {
  samplepad.set_inputTypeThresholds(20, 40, 70, 150); // set the thresholds for the four input types  (singletap, shortpress, longpress, doubletapspeed)
  samplepad.set_detectionThreshold(80, 20);  //set touch sensitivity in the form of detection, rejection thresholds values
  samplepad.set_capTouchPins(3,4,0,0,0);   //set arduino pins associated with the pads (sendpin, receivepin1, receivepin2, receivepin3, receivepin4) this example uses just one pad.
  samplepad.initialize_capTouch(1);
  samplepad.set_haptics(9,40,255);  //set haptic feedback variables (arduino pwm pin, duration of haptics(ms), pwn strength from 0-255)
  pinMode(LED, OUTPUT);

}
void loop() {
  touchtype = samplepad.detect_touch(0);
  if (touchtype == 1){ //single tap to toggle led on and off
    ledstate = !ledstate;
    digitalWrite(LED, ledstate);
  }
else if (touchtype == 2) {  //double tap to blink led
   for (int i = 0; i < 4; i++){
   ledstate = !ledstate;
    digitalWrite(LED, ledstate);
    delay(400);
   }
   digitalWrite(LED, LOW);
}
samplepad.update_basevalue(0);
}
