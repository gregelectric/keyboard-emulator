/*
 Keyboard test

 For the Arduino Leonardo, Micro or Due

 Reads a byte from the serial port, sends a keystroke back.
 The sent keystroke is one higher than what's received, e.g.
 if you send a, you get b, send A you get B, and so forth.

 * RX is digital pin 9 (connect to TX of other device)
 * TX is digital pin 10 (connect to RX of other device)

 */

#include <SoftwareSerial.h>
#include "Keyboard.h"

SoftwareSerial mySerial(10, 9); // RX, TX

void setup() 
{
  // Open serial communications and wait for port to open:
  mySerial.begin(57600);
  Serial.begin(5700);
  
  // initialize control over the keyboard:
  Keyboard.begin();
}

void loop()  
{
  // check for incoming serial data:
  if (mySerial.available() > 0) 
  {
    // read incoming serial data:
    char inChar = mySerial.read();
    // Type the next ASCII value from what you received:
    Keyboard.write(inChar);
  }
}

