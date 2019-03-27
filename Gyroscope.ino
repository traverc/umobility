
Adafruit_L3GD20 gyro;
void gyroSetup() {
  if (!gyro.begin(gyro.L3DS20_RANGE_250DPS)) {
    Serial.println("Oops ... unable to initialize the L3GD20. Check your wiring!"); while (1);
  }
}

void gyroLoop() {
  gyro.read();
  Serial.print("Z: "); Serial.println((int)gyro.data.z); Serial.print(" ");
}
