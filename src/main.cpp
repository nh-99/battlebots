#include <IBusBM.h>
#include <Servo.h>

#include "L298N.h"
#include "mecanumdrive.h"

// Telementry for iBUS
#define IBUS_SENSOR_TYPE_FUEL 0x06

// Stick values for iBUS
#define TX_CHANNEL_HIGH 2012
#define TX_CHANNEL_LOW 988
#define TX_CHANNEL_MIDDLE 1500
#define STICK_DEADZONE 10

// Drivetrain motor PWM
#define FLM_L_PWM 11
#define FLM_R_PWM 8

#define FRM_L_PWM 7
#define FRM_R_PWM 6

#define BLM_L_PWM 13
#define BLM_R_PWM 12

#define BRM_L_PWM 5
#define BRM_R_PWM 4

/**
 * Controls are:
 * - Movement
 * - Arm up/down
 * - Weapon on/off
 * - Overall arm/disarm
 * - Weapon motor direction
 * 
 * - Curve for throttle - configurable via remote??
 * 
 * 
 * Channel 01 - aileron - Movement right/left
 * Channel 02 - elevon - Movement forward/back
 * Channel 03 - throttle - arm position ???
 * Channel 04 - rudder - Movement rotation ???
 * Channel 05 - switch - arm/disarm
 * Channel 06 - switch - weapon arm/disarm
 * Channel 07 - right potentiometer - weapon direction
 * Channel 08 - switch 3pos - speed curve (3 choices)
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

Servo armServo;
Servo weaponEsc;

/**
 * Global lock on robot. Prevents all parts of robot from functioning until enabled.
*/
bool robotLocked = true;

int lastRightLeftMoveVal, lastForwardBackwardMoveVal = 0;
bool weaponValueChanged = false;
bool weaponArmed, escArmed = false;

/**
 * Debug
 */
void testFunctionality() {
  // TEST DRIVE
  // frontLeftMotor->TurnRight(100);
  // frontRightMotor->TurnRight(100);
  // backLeftMotor->TurnRight(100);
  // backRightMotor.TurnRight(100);
  // delay(2000);
  // frontLeftMotor.Stop();
  // frontRightMotor.Stop();
  // backLeftMotor.Stop();
  // backRightMotor.Stop();
  // delay(1000);
  // frontLeftMotor.TurnLeft(100);
  // frontRightMotor.TurnLeft(100);
  // backLeftMotor.TurnLeft(100);
  // backRightMotor.TurnLeft(100);
  // delay(2000);
  // frontLeftMotor.Stop();
  // frontRightMotor.Stop();
  // backLeftMotor.Stop();
  // backRightMotor.Stop();
  // delay(1000);

  // TEST ARM
//   int i = 0;
//   int pos = 0;
//   while (i < 10) {
//     for (pos = 0; pos <= 20; pos += 1) { // goes from 0 degrees to 180 degrees
//       // in steps of 1 degree
//       armServo.write(pos);              // tell servo to go to position in variable 'pos'
//       delay(15);                       // waits 15 ms for the servo to reach the position
//     }
//     for (pos = 20; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
//       armServo.write(pos);              // tell servo to go to position in variable 'pos'
//       delay(15);                       // waits 15 ms for the servo to reach the position
//     }
//     i++;
//   }

//   // TEST WEAPON
//   if (!weaponValueChanged) {
//     int pwmVal = map(300, 0, 1023, 1100, 1900);
//     weaponEsc.writeMicroseconds(pwmVal);
//     weaponValueChanged = true;
//   }

//   delay(5000);

//   int pwmVal = map(0, 0, 1023, 1100, 1900);
//   weaponEsc.writeMicroseconds(pwmVal);
//   weaponValueChanged = false;

//   delay(5000);
}

void disarmWeapon() {
  if (weaponArmed) {
    int pwmVal = map(0, 0, 1023, 1100, 1900);
    weaponEsc.writeMicroseconds(pwmVal); // Send signal to ESC.
    delay(2000);

    weaponArmed = false;
  }
}

void armWeaponEsc() {
  if (!escArmed) {
    // arm the esc
    int pwmVal = map(512, 0, 1023, 1100, 1900);
    weaponEsc.writeMicroseconds(pwmVal);
    delay(1000);
    pwmVal = map(0, 0, 1023, 1100, 1900);
    weaponEsc.writeMicroseconds(pwmVal); // Send signal to ESC.
    delay(2000);
    escArmed = true;
  }

  disarmWeapon();
}

void armWeapon() {
  weaponArmed = true;
}

void setup() {
  Serial.begin(115200);

  // Setup drivetrain motors
  frontLeftMotor->Enable();
  frontRightMotor->Enable();
  backLeftMotor->Enable();
  backRightMotor->Enable();

  armServo.attach(9);
  weaponEsc.attach(10);

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
void loop() {
  int turn, y1, strafe, forward, arm, weaponArm, weaponSpeed;

  // Read channel inputs
  turn = ibus.readChannel(3);
  y1 = ibus.readChannel(2);
  strafe = ibus.readChannel(0);
  forward = ibus.readChannel(1);
  arm = ibus.readChannel(4);
  weaponArm = ibus.readChannel(5);
  weaponSpeed = ibus.readChannel(6);

  //
  // GLOBAL ARM
  //
  if (arm > TX_CHANNEL_MIDDLE) {
    robotLocked = false;
  } else {
    robotLocked = true;
  }

  // Check robot lock at this point, run disarm functions, and prevent more code from running
  if (robotLocked) {
    disarmWeapon();
    mecanumDrive.Stop();
    return;
  }

  //
  // WEAPON ARMING
  //
  // if (weaponArm == TX_CHANNEL_MIDDLE) {
  //   armWeaponEsc();
  // } else if (weaponArm > TX_CHANNEL_MIDDLE && !weaponArmed) {
  //   armWeapon();
  // } else if (weaponArm < TX_CHANNEL_MIDDLE && weaponArmed) {
  //   disarmWeapon();
  // }

  //
  // MOVEMENT
  //
  mecanumDrive.HandleStickInput(turn, strafe, forward);

  //
  // WEAPON CONTROL
  //
  if ((!weaponValueChanged || weaponSpeed != previousWeaponSpeed) && weaponArmed) {
    int pwmVal = map(weaponSpeed, TX_CHANNEL_LOW, TX_CHANNEL_HIGH, 1100, 1900);
    weaponEsc.writeMicroseconds(pwmVal);
    weaponValueChanged = true;
    previousWeaponSpeed = weaponSpeed;
  }
}