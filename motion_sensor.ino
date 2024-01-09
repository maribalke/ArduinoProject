int motion_sensor_pin = 2; //Pin connected to motion sensors output
int alarm_led_output = 3; //Not important, output pin for testing

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(motion_sensor_pin, INPUT_PULLUP);
  pinMode(alarm_led_output, OUTPUT);
}
void motion_detector() {
  if (digitalRead(motion_sensor_pin) == HIGH) {
    //Put consequences for activated motion sensor here
    Serial.println("Movement detected");
    digitalWrite(alarm_led_output, HIGH);}
   else {
    Serial.println("Movement not detected");
    digitalWrite(alarm_led_output, LOW);
  }
}

void loop() {
  motion_detector();
  delay(1000);
}
