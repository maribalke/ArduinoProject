int LED_output = 3;        //LED for testing
int laser_sensor_pin = 2;  //Pin for output from laser sensor
int laser_sensor_value = 0;
int laser_sensor_threshold = 700;  //Threshold for laser, lower/raise if output is wrong

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(laser_sensor_pin, INPUT);
  pinMode(LED_output, OUTPUT);
}

void laser_detector() {
  laser_sensor_value = analogRead(laser_sensor_pin);
  if (laser_sensor_value < laser_sensor_threshold) {
    //Consequences for triggering the laser sensor
    Serial.println("Alarm triggered");
    digitalWrite(alarm_led_output, HIGH);
  }

else {
  //Consequence for no alarm triggered
  Serial.println("No alarm triggered");
  digitalWrite(alarm_led_output, LOW);
}
}

void loop() {
}
