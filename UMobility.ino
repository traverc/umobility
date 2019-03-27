#include "Adafruit_BLE_UART.h"
#include <Adafruit_L3GD20.h>
#include <TimerOne.h>
#include <PID_v1.h>
#include <Servo.h>
#include <SPI.h>

long duration;
int distance1;

// Select system settings
boolean SPEED_POTENTIOMETER = false;
boolean ABILITY_SWITCHES    = true;
boolean JOYSTICK_DIGITAL    = true;
boolean JOYSTICK_ANALOG     = false;
boolean BLUETOOTH           = false;
boolean SAFEMODE            = true;
boolean DEBUG               = false;
boolean GYRO                = false;
boolean START               = true;


// Invert one or both motors
boolean INVERT_1 = false;
boolean INVERT_2 = true;

// Constants
int SPEED_LEVEL = 30;                  // Input 20, 25, or 30 percent duty cycle. 30 Max for safety
int SPEED_LIMIT = 256;                 // Between 0-512
int DEADBAND = 150;
int ACCEL = 2;
int WARNING_DISTANCE = 14;             // Distance in inches to sound buzzer
int REVERSE_PULSE    = 1000;           // Talon SR is 1000
int FORWARD_PULSE    = 2000;           // Talon SR is 2000

// Pins
int BATTERY     = A0;                  // voltage divider Vout pin
int JOYSTICK_Y  = A1;
int JOYSTICK_X  = A2;
int SPEED_POT   = A3;
int FORWARD     = 4;                   // ability switches (red pins)
int BACKWARD    = 5;
int LEFT        = 6;
int RIGHT       = 7;
int PWM_L       = 10;
int PWM_R       = 11;
int ECHO_PIN    = 3;                   // Must be interrupt pin (Mega): 2, 3, 18, 19, 20, 21
int TRIG_PIN    = 9;                   // ultrasonicSensor Pin
int ECHO_PIN2   = 2;                   // Interrupt pin
int TRIG_PIN2   = 22;
int BUZZER      = 8;
int ADAFRUITBLE_RDY = 18;              // Must be interrupt pin (Mega): 2, 3, 18, 19, 20, 21
int ADAFRUITBLE_REQ = 40;
int ADAFRUITBLE_RST = 42;

Servo leftMotor;
Servo rightMotor;

// Ultrasonic Sensor
int distance = 0;
#define STOP_DISTANCE 10
#define echo_int 0                    // Interrupt id for echo pulse
#define TIMER_US 50                   // 50 uS timer duration
#define TICK_COUNTS 4000              // 200 mS worth of timer ticks
volatile long echo_start = 0;         // Records start of echo pulse
volatile long echo_end = 0;           // Records end of echo pulse
volatile long echo_duration = 0;      // Duration - difference between end and start
volatile int trigger_time_count = 0;  // Count down counter to trigger pulse time



// ---------------------------------------- Setup ----------------------------------- //
void setup() {

  pinMode(BATTERY, INPUT);

  leftMotor.attach(PWM_L);
  rightMotor.attach(PWM_R);
  leftMotor.writeMicroseconds(1500);
  rightMotor.writeMicroseconds(1500);

  if (DEBUG) Serial.begin(9600);

  if (SAFEMODE) {
    pinMode(BUZZER, OUTPUT);
    pinMode(TRIG_PIN, OUTPUT);                          // Trigger pin set to output
    pinMode(ECHO_PIN, INPUT);                           // Echo pin set to input
    pinMode(TRIG_PIN2, OUTPUT);                          // Trigger pin set to output
    pinMode(ECHO_PIN2, INPUT);                           // Echo pin set to input
    Timer1.initialize(TIMER_US);                        // Initialise timer 1
    Timer1.attachInterrupt(timerIsr);                   // Attach interrupt to the timer service routine
    attachInterrupt(echo_int, echo_interrupt, CHANGE);  // Attach interrupt to the sensor echo input
  }

  if (JOYSTICK_ANALOG || JOYSTICK_DIGITAL) {
    pinMode(JOYSTICK_X, OUTPUT);
    pinMode(JOYSTICK_Y, OUTPUT);
  }

  if (ABILITY_SWITCHES) {
    pinMode(FORWARD, INPUT_PULLUP);
    pinMode(BACKWARD, INPUT_PULLUP);
    pinMode(LEFT, INPUT_PULLUP);
    pinMode(RIGHT, INPUT_PULLUP);
  }

  if (GYRO) {
    gyroSetup();
    pidSetup();
  }

  if (BLUETOOTH) {
    Adafruit_BLE_UART BTLEserial = Adafruit_BLE_UART(ADAFRUITBLE_REQ, ADAFRUITBLE_RDY, ADAFRUITBLE_RST);
  }

}



// -------------------------------- Main Loop ----------------------------------------- //

void loop() {

  while (!START) {
    delay(200);
    int cm = getFrontDistance();
    if (cm > 10) START = true;
  }

  while (START) {

    if (ABILITY_SWITCHES && START) {
      abilitySwitchDrive(SPEED_LEVEL);
    }

    if ((JOYSTICK_DIGITAL) && START) {
      joystickDigitalDrive(SPEED_LEVEL);
    }

    if ((JOYSTICK_ANALOG) && START) {
      joystickAnalogDrive();

    }

    if (SAFEMODE) {
      int cm = getFrontDistance();
      //      Serial.print ("Distance in cm: ");
      //      Serial.println(cm);
      if (cm < 10) START = false;
    }
  }


//  if (SAFEMODE) {
//    int cm = getFrontDistance();
//    Serial.print ("Distance in cm: ");
//    Serial.println(cm);
//    if (cm < 10) START = false;
//    
//    while (getFrontDistance() < 10) {
//      START = false;
//      Serial.println("HERE");
//    }
//    
//    if (getFrontDistance() > 10) START = true;
//    Serial.println("OUT");
//  }


  //  delay(100);


  // ------------------------------ Component Test --------------------------------- //
  //  buzzerTest();
  //  abilitySwitchTest();
  //  abilitySwitchDriveTest();
  //  joystickTest();
  //  joyStickDriveTest();
  //  frontSensorTest();
  //  backSensorTest();           // CAUTION: Make sure the shield is properly attached to avoid intermittent connections (esp. Pin22)
  //  gyroTest();
    PIDDriveTest();
    PIDDrive2Test();

}



// --------------------------- DEBUG Helper Method -------------------------------- //

void debug(String x, int val) {
  if (DEBUG) Serial.print(x), Serial.print(": "), Serial.println(val);
}
