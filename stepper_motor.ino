#include <Stepper.h>

const int stepsPerRevolution = 100;  // change this to fit the number of steps per revolution
// for your motor

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 2, 3, 4, 5);

int stepCount = 0;         // number of steps the motor has taken

void setup() {
  myStepper.setSpeed(200);
  Serial.begin(115200);
}

void loop() {
  // step one step:
  myStepper.step(stepsPerRevolution);

  delay(100);
}