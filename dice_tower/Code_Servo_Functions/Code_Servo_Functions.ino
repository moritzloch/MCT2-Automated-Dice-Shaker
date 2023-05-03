#include <Servo.h>

Servo bottomServo;    // Servo to push dice into car
Servo topServo;       // Servo to lift dice into tower

bool var = false;

void setup() {
  /*  Call interrupt func if button on pin 2 is pressed */
  pinMode(2, INPUT_PULLUP);
  pinMode(10, INPUT);
  attachInterrupt(digitalPinToInterrupt(2), FuncIsr, FALLING);

  /*  Attach servos to pin 8 and 9*/
  bottomServo.attach(8);
  topServo.attach(9);

  /*  Move servos to initial position */
  stepServo(1, 160);                                      
  stepServo(2, 175);                                      
}

void loop() {
  if (var) {
    roll_the_dice();
    var = false;
  }
}

void roll_the_dice() {
  /* Dice needs to be placed in car
   * Lift the dice and throw into tower
   * to roll the dice */
  stepServo(2, 0);
  delay(1000);
  jumpServo(2, 130);
  delay(200);
  stepServo(2, 175);


  /* Eject the lower servo three times
   * to ensure the dice is placed in the car */
  jumpServo(2, 180);    //place car closer to outlet

  stepServo(1, 90);     //extend #1
  delay(500);
  stepServo(1, 130);
  delay(200);
  stepServo(1, 90);    //extend #2
  delay(200);
  stepServo(1, 130);
  delay(200);
  stepServo(1, 90);    //extend #3
  stepServo(2, 175);
  delay(200);
  stepServo(1, 160);
  delay(1200);
}

void stepServo(int number, int pos) 
/* Servo no.1 = bottomServo
 * Servo no.2 = topServo
 * Increase/Decrease angle of servo slowly */
{
  int j;
  switch (number) {
    case 1:
      while ( bottomServo.read() != pos ) {
        j = bottomServo.read();
        bottomServo.write(j + ((pos - j) / abs(pos - j)));  //current position +- 1
        delay(3);
      }
      break;
    case 2:
      while ( topServo.read() != pos ) {
        j = topServo.read();
        topServo.write(j + ((pos - j) / abs(pos - j)));     //current position +- 1
        delay(4);
      }
      break;
  }
}

void jumpServo(int number, int pos) 
/* Servo no.1 = bottomServo
 * Servo no.2 = topServo
 * Jump to angle */
{                 
  if (number == 1) bottomServo.write(pos);
  else topServo.write(pos);
  delay(100);
}

void FuncIsr() 
/* Interrupt Service Routine */
{
  var = true;
}
