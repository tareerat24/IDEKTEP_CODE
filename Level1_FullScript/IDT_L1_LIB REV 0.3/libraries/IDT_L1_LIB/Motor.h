#ifndef MOTOR_H
#define MOTOR_H

class Motor
{

public:

  void motorInit();
  static void forward();
  static void backward();
  static void slide_left();
  static void slide_right();
  static void slide_left_front();
  static void slide_right_front();
  static void slide_left_back();
  static void slide_right_back();
  static void turn_left();
  static void turn_right();
  static void stop();

  void adjectMotorSpeed(int LA, int LB, int RA, int RB);

  
};
#endif