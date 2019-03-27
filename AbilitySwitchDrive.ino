
// ------------------------------------------ Drive with Ability Switch with DutyCycle Input ----------------------------------------- //

void abilitySwitchDrive(int percentDuty) {
  float STOP_PULSE = 1500;
  float FORWARD_PULSE = ((percentDuty * 540) / 100.0) + 1460;
  float REVERSE_PULSE = 1460 - ((percentDuty * 460.0) / 100);
  float FORWARD_STEP = FORWARD_PULSE / 100;
  float REVERSE_STEP = REVERSE_PULSE / 100;
  float leftPulse = STOP_PULSE;
  float rightPulse = STOP_PULSE;
  
  if (digitalRead(FORWARD) == LOW || digitalRead(BACKWARD) == LOW || digitalRead(LEFT) == LOW || digitalRead(RIGHT) == LOW) {
    int timerStart = millis();
    int timerEnd = millis();
    while (timerEnd - timerStart < 300) timerEnd = millis(); // WAIT FOR FIRM BUTTON PRESS
    while (digitalRead(FORWARD) == LOW && START) { // TURN FORWARD
      Serial.println("FORWARD");
      debug("left speed", leftPulse);
      debug("right speed", rightPulse);

      for (leftPulse, rightPulse; leftPulse > REVERSE_PULSE+60, rightPulse > REVERSE_PULSE+60 ; leftPulse -= REVERSE_STEP, rightPulse -= REVERSE_STEP, delay(30)) {
        leftMotor.writeMicroseconds(leftPulse), rightMotor.writeMicroseconds(rightPulse);
      }
      leftMotor.writeMicroseconds(leftPulse), rightMotor.writeMicroseconds(rightPulse);
    }
    for (leftPulse, rightPulse; leftPulse < STOP_PULSE, rightPulse < STOP_PULSE ; leftPulse += REVERSE_STEP, rightPulse += REVERSE_STEP, delay(50)) {
      leftMotor.writeMicroseconds(leftPulse), rightMotor.writeMicroseconds(rightPulse);
    }
    while (digitalRead(BACKWARD) == LOW && START) { // TURN BACKWARD
//      debug("left speed", leftPulse);
//      debug("right speed", rightPulse);
      Serial.println("BACKWARD");
      for (leftPulse, rightPulse; leftPulse < FORWARD_PULSE, rightPulse < FORWARD_PULSE ; leftPulse += FORWARD_STEP, rightPulse += FORWARD_STEP, delay(30)) {
        leftMotor.writeMicroseconds(leftPulse), rightMotor.writeMicroseconds(rightPulse);
      }
      leftMotor.writeMicroseconds(leftPulse), rightMotor.writeMicroseconds(rightPulse);
    }
    for (leftPulse, rightPulse; leftPulse > STOP_PULSE, rightPulse > STOP_PULSE ; leftPulse -= FORWARD_STEP, rightPulse -= FORWARD_STEP, delay(50)) {
      leftMotor.writeMicroseconds(leftPulse), rightMotor.writeMicroseconds(rightPulse);
    }
    while (digitalRead(LEFT) == LOW && START) { // TURN LEFT
//      debug("left speed", leftPulse);
//      debug("right speed", rightPulse);
        Serial.println("LEFT");
      for (leftPulse, rightPulse; rightPulse > REVERSE_PULSE, leftPulse < FORWARD_PULSE ; rightPulse -= REVERSE_STEP, leftPulse += FORWARD_STEP, delay(30)) {
        leftMotor.writeMicroseconds(leftPulse), rightMotor.writeMicroseconds(rightPulse);
      }
      leftMotor.writeMicroseconds(leftPulse), rightMotor.writeMicroseconds(rightPulse);
    }
    for (leftPulse, rightPulse; rightPulse < STOP_PULSE, leftPulse > STOP_PULSE ; rightPulse += REVERSE_STEP, leftPulse -= FORWARD_STEP, delay(50)) {
      leftMotor.writeMicroseconds(leftPulse), rightMotor.writeMicroseconds(rightPulse);
    }
    while (digitalRead(RIGHT) == LOW && START) { // TURN RIGHT
      Serial.println("RIGHT");
//      debug("left speed", leftPulse);
//      debug("right speed", rightPulse);
      for (leftPulse, rightPulse; leftPulse > REVERSE_PULSE-20, rightPulse < FORWARD_PULSE+20 ; leftPulse -= REVERSE_STEP, rightPulse += FORWARD_STEP, delay(30)) {
        leftMotor.writeMicroseconds(leftPulse), rightMotor.writeMicroseconds(rightPulse);
      }
      leftMotor.writeMicroseconds(leftPulse), rightMotor.writeMicroseconds(rightPulse);
    }
    for (leftPulse, rightPulse; leftPulse < STOP_PULSE, rightPulse > STOP_PULSE ; leftPulse += REVERSE_STEP, rightPulse -= FORWARD_STEP, delay(50)) {
      leftMotor.writeMicroseconds(leftPulse), rightMotor.writeMicroseconds(rightPulse);
    }
  }
}
