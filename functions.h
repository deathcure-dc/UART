void recvWithEndMarker() {
  static byte ndx = 0;
  char endMarker = ';';
  char rc;
  
  //Read data from port 1, send to port 0
  while (Serial1.available() > 0 && newData == false) {
    rc = Serial1.read();

    if (rc != endMarker) {
      receivedChars[ndx] = rc;
      ndx++;
      if (ndx >= numChars) {
        ndx = numChars - 1;
      }
    }
    else {
      receivedChars[ndx] = '\0'; // terminate the string
      ndx = 0;
      newData = true;
    }
  }
}

void showNewData() {
  if (newData == true) {

    Serial.println(receivedChars);
    newData = false;
  }
}
