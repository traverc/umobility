
// -------------------------------- PID Library SETUP --------------------------- //

#include <PID_v1.h>
double Setpoint, Input, Output;
double Kp = 1, Ki = 0.05, Kd = 0.25;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, P_ON_M, DIRECT);

void pidSetup() {
  Setpoint = 0;
  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(1370, 1390);
}

void pidLoop() {
  gyro.read();
  Input = (int)gyro.data.z;
  myPID.Compute();
}



// -------------------------------- PID Drive Implementation  --------------------------- //

void goStraight() {
  double straightSpeed = 1380;
  int target = 0;
  gyro.read();
  int absolute = (int)gyro.data.z;
  while (digitalRead(FORWARD) == LOW) {
    gyro.read();
    absolute = (int)gyro.data.z;
    double leftSpeed = straightSpeed - (((double)absolute - target));
    double rightSpeed = straightSpeed + (((double)absolute - target));
    leftMotor.writeMicroseconds(leftSpeed), rightMotor.writeMicroseconds(rightSpeed);
    Serial.print("absolute: "), Serial.print(absolute), Serial.print(" | Left: "), Serial.print(leftSpeed), Serial.print(" | Right: "), Serial.println(rightSpeed);
  }
  leftMotor.writeMicroseconds(1500), rightMotor.writeMicroseconds(1500);
}




void goStraight2() {
  while (digitalRead(FORWARD) == LOW) {
    double straightSpeed = 1380;
    gyro.read();
    Input = (int)gyro.data.z;
    myPID.Compute();
    leftMotor.writeMicroseconds(Output), rightMotor.writeMicroseconds(1380);
    //    Serial.print("Input: "), Serial.print(Input), Serial.print(" | Output: "), Serial.println(Output);
  }
  leftMotor.writeMicroseconds(1500), rightMotor.writeMicroseconds(1500);
}
