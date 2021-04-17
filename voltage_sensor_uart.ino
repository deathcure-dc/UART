
// Constants
const unsigned long eventInterval = 1000; // interval at which to send data (milliseconds)
const byte  numChars = 200;

//Variables:
unsigned long previousMillis = 0;// will store last time data was updated
char        receivedChars[numChars];
boolean     newData = false;


#include "functions.h"
void setup() {
  Serial.begin(9600);   //  setup serial
  Serial1.begin (9600); // setup serial1 for uart communication
}


void loop() {
  /* Updates frequently */
  unsigned long currentMillis = millis();

  const int analogPin[14] = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13};
  float value_volt[14] = {};  // variable to store the value read


  recvWithEndMarker();
  showNewData();
  for (int i = 0; i < 14; i++) {
    value_volt[i] = analogRead(analogPin[i]);  // read the input pin
    // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
    value_volt[i] = value_volt[i] * (5.00 / 1023.00);
    // Convert the sensor voltage (which goes from 0 - 1.3V) to battery voltage (0 - 4.2V):
    value_volt[i] = value_volt[i] * 3;
  }
  
  //Search for lowest value
  float lowVal = value_volt[0];  // initialize lowVal
  for (byte n = 0; n < 14; n++) {
    if (value_volt[n] < lowVal) {
      lowVal = value_volt[n];
    }
  }
  
  //Search for highest value
  float highVal = value_volt[0];  // initialize highVal
  for (byte n = 0; n < 14; n++) {
    if (value_volt[n] > highVal) {
      highVal = value_volt[n];
    }
  }
  
  //Find difference between highest and lowest value
  float vol_diff = highVal - lowVal;
  //set pin 10 to LOW if difference is less than 0.2
  if (vol_diff <= 0.2) {
    digitalWrite(10, LOW);
  }
  //set pin 10 to HIGH if difference is more than 0.3
  if (vol_diff >= 0.3) {
    digitalWrite(10, HIGH);
  }

  //read from port 0, send to port 1
  while (Serial.available() > 0) {

    if (currentMillis - previousMillis >= eventInterval) {
      for (int j = 0; j < 14; j++) {
        Serial1.print(value_volt[j]);
        Serial1.print("#");
      }
      Serial1.print(";");
    }
    /* Update the timing for the next time around */
    previousMillis = currentMillis;
  }

}
