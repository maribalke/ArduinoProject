#include <Servo.h>
#define joyX A0

Servo myServo;
int xValue = 0;
int camera_angle = 90;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  myServo.attach(3);
  myServo.write(90);
}
void stick_action() {
  xValue = analogRead(joyX);
  if (xValue < 200) {
    camera_angle = camera_angle - 2;
  } else if (xValue > 800) {
    camera_angle = camera_angle + 2;
  }

  myServo.write(camera_angle);
  delay(30);
}

void loop() {
  // put your main code here, to run repeatedly:
  stick_action();
}
