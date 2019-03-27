
// ------------------------ Ultrasonic Code with Interrupt Use -----------------------//

void timerIsr() {
  trigger_pulse();
}

void trigger_pulse() {
  static volatile int state = 0;
  if (!(--trigger_time_count)) {
    trigger_time_count = TICK_COUNTS;
    state = 1;
  }

  switch (state) {
    case 0: break;
    case 1:
      digitalWrite(TRIG_PIN, LOW);
      delayMicroseconds(5);
      digitalWrite(TRIG_PIN, HIGH);
      delayMicroseconds(5);
      state = 2;
      break;
    case 2:
    default:
      digitalWrite(TRIG_PIN, LOW);
      state = 0;
      break;
  }
}

void echo_interrupt() {
  switch (digitalRead(ECHO_PIN)) {
    case HIGH:
      echo_end = 0;
      echo_start = micros();
      break;
    case LOW:
      echo_end = micros();
      echo_duration = echo_end - echo_start;
      distance = echo_duration / 58;
      if (distance<7) START = false;
      else START = true;
  }
}


// ------------------------------ Non-Interrupt Get Distance Code ----------------------------//

int getFrontDistance() {
  long duration;
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  int distanceInCM = duration * 0.034 / 2;
  return distanceInCM;
}

int getBackDistance() {
  long duration;
  digitalWrite(TRIG_PIN2, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN2, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN2, LOW);
  duration = pulseIn(ECHO_PIN2, HIGH);
  int distanceInCM = duration * 0.034 / 2;
  return distanceInCM;
}
