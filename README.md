# Advanced_capacitive_touch_detection

These capacitive touch detection library is an extension of the Arduino capacitive touch library, it offers two detection algorithms that solves two of the main problems associated with detecting capacitive touch on the Arduino. The library also allows advanced touch detections; such as, Double tap, Short press and Long press. Also featured in the library is a haptics controller, this allows you to connect and control a vibration motor as a feedback when you interact with the capacitive touch surface.

```C
#include <AdvCapTouch.h>

AdvCapTouch samplepad  = AdvCapTouch();  //create a new captouch object
int touchtype;

void setup() {
 samplepad.set_capTouchPins(3,4,0,0,0);   //set arduino pins associated with the pads (sendpin, receivepin1, receivepin2, receivepin3, receivepin4) this example uses just one pad.
  samplepad.set_haptics(4, 40, 255); //set haptic feedback variables (arduino pwm pin, duration of haptics(ms), pwn strength from 0-255)------(optional)
  /************************************************************************************************************************************************************/
  samplepad.initialize_capTouch(1);
  Serial.begin(115200);
}
```

The code snippet above shows how you would typically setup the library.


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
}
```

You call the detect_touchFromNoise() function to detect touch inputs, the function will return an integer value from 0 – 4.

 •	0 means no touch was detected.
 
 •	1 means a single tap was detected.
 
 •	2 means a double tap was detected.
 
 •	3 means a short press was detected.
 
 •	4 means a long press was detected.
 
The function detect_touch() should always be called with a integer value from 0 – 3, this value represents the position of the capacitive touch pad you want to run a touch detection on.


