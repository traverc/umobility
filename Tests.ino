


void buzzerTest() {
  tone(BUZZER, 1000);
}


void abilitySwitchDriveTest() {
  if (ABILITY_SWITCHES) {                        // Ability Switch Drive Test
    abilitySwitchDrive(SPEED_LEVEL);
  }
}

void abilitySwitchTest() {
  while (digitalRead(FORWARD) == LOW) {
    //    tone(BUZZER, 1000);
    Serial.println("FORWARD");
  }
  while (digitalRead(BACKWARD) == LOW) {
    //    tone(BUZZER, 1000);
    Serial.println("BACKWARD");
  }
  while (digitalRead(LEFT) == LOW) {
    //    tone(BUZZER, 1000);
    Serial.println("LEFT");
  }
  while (digitalRead(RIGHT) == LOW) {
    //    tone(BUZZER, 1000);
    Serial.println("RIGHT");
  }
}


void joystickTest() {
  int x = analogRead(JOYSTICK_X);
  int y = analogRead(JOYSTICK_Y);
  debug("x", x);
  debug("y", y);
}

void joyStickDriveTest() {
  if (JOYSTICK_ANALOG || JOYSTICK_DIGITAL) {
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
}


void frontSensorTest() {
  int currDistInCM = getFrontDistance();
  Serial.print ("Distance in cm: ");
  Serial.println(currDistInCM);
}

void backSensorTest() {
  int currDistInCM = getBackDistance();
  Serial.print ("Distance in cm: ");
  Serial.println(currDistInCM);
}


void gyroTest() {
  gyroLoop();
}

void PIDDriveTest() {
  goStraight();
}
