#include "L298N.h"

L298N::L298N() {}

L298N::L298N(uint8_t L_PWM, uint8_t R_PWM){
  _R_PWM = R_PWM;
  _L_PWM = L_PWM;
  pinMode(_R_PWM, OUTPUT);
  pinMode(_L_PWM, OUTPUT);
}

void L298N::TurnRight(uint8_t pwm){
   analogWrite(_L_PWM, 0);
   delayMicroseconds(100);
   analogWrite(_R_PWM, pwm);
}

void L298N::TurnLeft(uint8_t pwm){
   analogWrite(_R_PWM, 0);
   delayMicroseconds(100);
   analogWrite(_L_PWM, pwm);
}

void L298N::Enable(){
}

void L298N::Disable(){
}

void L298N::Stop(){
  analogWrite(_L_PWM, LOW);
  analogWrite(_R_PWM, LOW);
}
