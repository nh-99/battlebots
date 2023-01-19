#include <IBusBM.h>

#include "DShotTimer2.h"
#include "motor_driver_L298N.h"
#include "mecanumdrive.h"

// Stick values for iBUS
#define TX_CHANNEL_HIGH 2012
#define TX_CHANNEL_LOW 988
#define TX_CHANNEL_MIDDLE 1500
#define TX_FAILSAFE_MEASUREMENT_INTERVAL_MILLIS 200
#define TX_FAILSAFE_MEASUREMENT_COUNT 2
#define STICK_DEADZONE 10

// Drivetrain motor PWM
#define FLM_L_PWM 5
#define FLM_R_PWM 4

#define FRM_L_PWM 11
#define FRM_R_PWM 8

#define BLM_L_PWM 7
#define BLM_R_PWM 6

#define BRM_L_PWM 12
#define BRM_R_PWM 13 

/**
 * Controls are:
 * - movement (horizontal, lateral, rotational)
 * - arm up/down
 * - robot arm/disarm
 * - weapon arm/disarm
 * - weapon speed
 * 
 * Channel 01 - aileron - Movement right/left
 * Channel 02 - elevon - Movement forward/back
 * Channel 03 - throttle - arm position ???
 * Channel 04 - rudder - Movement rotation ???
 * Channel 05 - switch - arm/disarm
 * Channel 06 - switch - weapon arm/disarm
 * Channel 07 - right potentiometer - weapon speed
 * Channel 08 - failsafe
 * 
 *   --- CHANNELS ONLY EXIST ON IBUS - IM ON IBUS ---
 * Channel 09 - ???
 * Channel 10 - ???
 */

IBusBM ibus; // IBus object

// Motors for swerve drive
MotorDriver *frontLeftMotor = new L298N(FLM_L_PWM, FLM_R_PWM);
MotorDriver *frontRightMotor = new L298N(FRM_L_PWM, FRM_R_PWM);
MotorDriver *backLeftMotor = new L298N(BLM_L_PWM, BLM_R_PWM);
MotorDriver *backRightMotor = new L298N(BRM_L_PWM, BRM_R_PWM);

MecanumDrive mecanumDrive(frontLeftMotor, frontRightMotor, backLeftMotor, backRightMotor);

DShot weaponEsc;

/**
 * Global lock on robot & failsafe lock. Prevents all parts of robot from functioning until enabled.
*/
bool robotLocked = true;
bool failsafe = true;

int lastRightLeftMoveVal, lastForwardBackwardMoveVal = 0;
bool weaponValueChanged = false;
bool weaponArmed, escArmed = false;

void disarmWeapon() {
  if (weaponArmed) {
    weaponEsc.setThrottle(0);
    weaponArmed = false;
  }
}

void armWeapon() {
  weaponArmed = true;
}

void ibusLoop() {
  ibus.loop();
}

void setup() {
  // Serial.begin(9600);
  // Setup motor pins as outputs
  pinMode(FLM_L_PWM, OUTPUT);
  pinMode(FLM_R_PWM, OUTPUT);
  pinMode(FRM_L_PWM, OUTPUT);
  pinMode(FRM_R_PWM, OUTPUT);
  pinMode(BLM_L_PWM, OUTPUT);
  pinMode(BLM_R_PWM, OUTPUT);
  pinMode(BRM_L_PWM, OUTPUT);
  pinMode(BRM_R_PWM, OUTPUT);

  weaponEsc.attach(10);
  weaponEsc.setThrottle(0);

  // iBUS connected to Serial1: 19 (RX) and 18 (TX)
  // (TX) not in use bc FS-RX2A does not support telemtry
  ibus.begin(Serial1);
  
  // We have to wait for the receiver to receive data from the transmitter (transmitter needs to be turned on)
  // as the channel values all read 0 as long as the transmitter is turned off at boot time.
  // We do not want the car to drive full speed backwards out of control.
  Serial.println("Wait for receiver");
  while (ibus.cnt_rec==0) delay(100);
  Serial.println("Init done");
}

int previousWeaponSpeed = -1;
int failsafeCounter = 0;
uint8_t previousCntRec = 0;
unsigned long previousMillis;
void loop() {
  int x1, y1, x2, y2, arm, weaponArm, weaponSpeed;

  // Read channel inputs
  x1 = ibus.readChannel(3);
  y1 = ibus.readChannel(2);
  x2 = ibus.readChannel(0);
  y2 = ibus.readChannel(1);
  arm = ibus.readChannel(4);
  weaponArm = ibus.readChannel(5);
  weaponSpeed = ibus.readChannel(6);

  // Implement failsafe. When RX loses connection, the cnt will output a consistent 49.
  // If we see that for 4 data points in a row (unlikely in normal use), we failsafe.
  if (millis() > previousMillis + TX_FAILSAFE_MEASUREMENT_INTERVAL_MILLIS) {
    if (previousCntRec == ibus.cnt_rec) {
      failsafeCounter++;
    } else if (failsafeCounter < 0) {
      failsafeCounter = 0;
    } else {
      failsafeCounter--;
    }
    if (failsafeCounter > TX_FAILSAFE_MEASUREMENT_COUNT) {
      failsafe = true;
      failsafeCounter = TX_FAILSAFE_MEASUREMENT_COUNT;
    } else {
      failsafe = false;
    }
    previousCntRec = ibus.cnt_rec;

    previousMillis = millis();
  }

  //
  // GLOBAL ARM / ROBOT LOCK
  //
  if (arm > TX_CHANNEL_MIDDLE) {
    robotLocked = false;
  } else {
    robotLocked = true;
  }
  // Check robot lock & failsafe at this point, run disarm functions, and prevent more code from running
  if (robotLocked || failsafe) {
    disarmWeapon();
    mecanumDrive.Stop();
    return;
  }

  //
  // WEAPON ARMING
  //
  if (!weaponArmed && weaponArm >= TX_CHANNEL_HIGH - STICK_DEADZONE) {
    armWeapon();
  } else if (weaponArmed &&weaponArm < TX_CHANNEL_HIGH - STICK_DEADZONE) {
    disarmWeapon();
  }

  //
  // MOVEMENT
  //
  mecanumDrive.HandleStickInput(x1, x2, y2);

  //
  // WEAPON CONTROL
  //
  if (!robotLocked && weaponArmed) {
    int throttleVal = map(weaponSpeed, TX_CHANNEL_LOW, TX_CHANNEL_HIGH, 48, 2047);
    weaponEsc.setThrottle(throttleVal);
    weaponValueChanged = true;
    previousWeaponSpeed = weaponSpeed;
  }

  //
  // ARTICULATING ARM POSITION
  //
  // y1 = y1 < 1000 ? 1000 : y1;
  // y1 = y1 > 2000 ? 2000 : y1;
  // armServo.write(map(y1, 1000, 2000, 0, 179));
}