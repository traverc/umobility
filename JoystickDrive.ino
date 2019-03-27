
void joystickDrive(int moveValue, int rotateValue) {
  int leftMotorSpeed = 0;
  int rightMotorSpeed = 0;
  if (moveValue > 0.0) {
    if (rotateValue > 0.0) {
      leftMotorSpeed = moveValue - rotateValue;
      rightMotorSpeed = max(moveValue, rotateValue);
    } else {
      leftMotorSpeed = max(moveValue, -rotateValue);
      rightMotorSpeed = moveValue + rotateValue;
    }
  } else {
    if (rotateValue > 0.0) {
      leftMotorSpeed = -max(-moveValue, rotateValue);
      rightMotorSpeed = moveValue + rotateValue;
    } else {
      leftMotorSpeed = moveValue - rotateValue;
      rightMotorSpeed = -max(-moveValue, -rotateValue);
    }
  }
  drive(map(leftMotorSpeed, -512, 512, 0, 1023), map(rightMotorSpeed, -512, 512, 0, 1023));
}

int prevLeft = 500;
int prevRight = 500;

void drive(int left, int right) {
  int speed1 = map(left, 0, 1023, 0, FORWARD_PULSE - REVERSE_PULSE);
  if (speed1 > prevLeft + ACCEL) speed1 = speed1 + ACCEL;
  else if (speed1 < prevLeft - ACCEL) speed1 = speed1 - ACCEL;
  if (INVERT_1) leftMotor.writeMicroseconds(FORWARD_PULSE - speed1);
  else leftMotor.writeMicroseconds(REVERSE_PULSE + speed1);
  prevLeft = speed1;

  int speed2 = map(right, 0, 1023, 0, FORWARD_PULSE - REVERSE_PULSE);
  if (speed2 > prevLeft + ACCEL) speed2 = speed2 + ACCEL;
  else if (speed2 < prevLeft - ACCEL) speed2 = speed2 - ACCEL;
  if (INVERT_2) rightMotor.writeMicroseconds(FORWARD_PULSE - speed2);
  else rightMotor.writeMicroseconds(REVERSE_PULSE + speed2);
  prevRight = speed2;
}



void joystickDigitalDrive(int percentDuty) {

  float STOP_PULSE = 1500;
  float FORWARD_PULSE = ((percentDuty * 540) / 100.0) + 1460;
  float REVERSE_PULSE = 1460 - ((percentDuty * 460.0) / 100);
  float FORWARD_STEP = FORWARD_PULSE / 100;
  float REVERSE_STEP = REVERSE_PULSE / 100;
  float leftPulse = STOP_PULSE;
  float rightPulse = STOP_PULSE;

  int x = analogRead(JOYSTICK_X);
  int y = analogRead(JOYSTICK_Y);

  //  if ((x < 10 && y > 700) || (x == 1 && y == 0) || (x == 0 && y == 1) || (x > 700 && y < 10)) {
  if ((analogRead(JOYSTICK_X) < 10 && analogRead(JOYSTICK_Y) > 700) || (analogRead(JOYSTICK_X) == 1 && analogRead(JOYSTICK_Y) == 0) || (analogRead(JOYSTICK_X) == 0 && analogRead(JOYSTICK_Y) == 1) || (analogRead(JOYSTICK_X) > 700 && y < 10)) {
    int timerStart = millis();
    int timerEnd = millis();
    while (timerEnd - timerStart < 300) timerEnd = millis(); // WAIT FOR FIRM BUTTON PRESS
    while ((analogRead(JOYSTICK_X) < 10 && analogRead(JOYSTICK_Y) > 700) && START) { // TURN FORWARD
      Serial.println("FORWARD");
      debug("left speed", leftPulse);
      debug("right speed", rightPulse);

      for (leftPulse, rightPulse; leftPulse > REVERSE_PULSE + 60, rightPulse > REVERSE_PULSE + 60 ; leftPulse -= REVERSE_STEP, rightPulse -= REVERSE_STEP, delay(30)) {
        leftMotor.writeMicroseconds(leftPulse), rightMotor.writeMicroseconds(rightPulse);
      }
      leftMotor.writeMicroseconds(leftPulse), rightMotor.writeMicroseconds(rightPulse);
    }
    for (leftPulse, rightPulse; leftPulse < STOP_PULSE, rightPulse < STOP_PULSE ; leftPulse += REVERSE_STEP, rightPulse += REVERSE_STEP, delay(50)) {
      leftMotor.writeMicroseconds(leftPulse), rightMotor.writeMicroseconds(rightPulse);
    }
    while ((analogRead(JOYSTICK_X) == 1 && analogRead(JOYSTICK_Y) == 0) && START) { // TURN BACKWARD
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
    while ((analogRead(JOYSTICK_X) > 700 && analogRead(JOYSTICK_Y) < 10) && START) { // TURN LEFT
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
    while ((analogRead(JOYSTICK_X) == 0 && analogRead(JOYSTICK_Y) == 1) && START) { // TURN RIGHT
      Serial.println("RIGHT");
      //      debug("left speed", leftPulse);
      //      debug("right speed", rightPulse);
      for (leftPulse, rightPulse; leftPulse > REVERSE_PULSE - 20, rightPulse < FORWARD_PULSE + 20 ; leftPulse -= REVERSE_STEP, rightPulse += FORWARD_STEP, delay(30)) {
        leftMotor.writeMicroseconds(leftPulse), rightMotor.writeMicroseconds(rightPulse);
      }
      leftMotor.writeMicroseconds(leftPulse), rightMotor.writeMicroseconds(rightPulse);
    }
    for (leftPulse, rightPulse; leftPulse < STOP_PULSE, rightPulse > STOP_PULSE ; leftPulse += REVERSE_STEP, rightPulse -= FORWARD_STEP, delay(50)) {
      leftMotor.writeMicroseconds(leftPulse), rightMotor.writeMicroseconds(rightPulse);
    }
  }
}

void joystickAnalogDrive() {
  
  int x = analogRead(JOYSTICK_X);
  int y = analogRead(JOYSTICK_Y);
  debug("Raw X", x);
  debug("Raw Y", y);

  //Zero values within deadband
  if (abs(512 - x) < DEADBAND) x = 512;
  if (abs(512 - y) < DEADBAND) y = 512;

  //Map values outside deadband to inside deadband
  if (x > 512) x = map(x, 512 + DEADBAND, 1023, 512, 1023);
  else if (x < 512) x = map(x, 0, 512 - DEADBAND, 0, 512);
  if (y > 512) y = map(y, 512 + DEADBAND, 1023, 512, 1023);
  else if (y < 512) y = map(y, 0, 512 - DEADBAND, 0, 512);

  //Establish a speed limit
  int limit = SPEED_LIMIT;
  if (SPEED_POTENTIOMETER) limit = map(analogRead(SPEED_POT), 0, 1023, 0, SPEED_LIMIT);
  //    debug("LIMIT", limit);

  //Map speeds to within speed limit
  x = map(x, 0, 1023, 512 - limit, 512 + limit);
  y = map(y, 0, 1023, 512 - limit, 512 + limit);

  //    debug("X", x);
  //    debug("Y", y);

  int moveValue = 0;
  if (y > 512) moveValue = y - 512;
  else moveValue = -(512 - y);

  int rotateValue = 0;
  if (x > 512) rotateValue = x - 512;
  else rotateValue = -(512 - x);

  joystickDrive(moveValue, rotateValue);

}
