// Remember to check the ports

#include <dht11.h>

// Define the correct ports
#define DHT11PIN 2
#define LED_humid 5
#define LED_temp 10
#define LED_light 3
#define PHOTO_RESISTOR A0

dht11 DHT11;

// Set pinmodes for the LEDs and start the serial monitor.
void  setup()
{
  Serial.begin(115200);
  pinMode(LED_humid, OUTPUT);
  pinMode(LED_temp, OUTPUT);
  pinMode(LED_light, OUTPUT);
}

// Checks the light intensity with a photo resistor and raises alarm if value is too high.
void checkLightIntensity() {
  int value = analogRead(PHOTO_RESISTOR);

  Serial.println("Analog  Value: ");
  Serial.println(value);
    
  if (value > 900) {
    digitalWrite(LED_light, HIGH);
  } else {
    digitalWrite(LED_light,  LOW);
  }
}

// Uses the humidity sensor to check both temperature and humidity. 
// Raises alarm if either is outside acceptable interval.
void checkTempAndHumidity() {
  Serial.println();
  int chk = DHT11.read(DHT11PIN);
  float humidity = (float)DHT11.humidity;
  float temp = (float)DHT11.temperature;

  if (humidity > 65 || humidity < 40){
    digitalWrite(LED_humid, HIGH);
  } else {
    digitalWrite(LED_humid, LOW);
  }

  if (temp > 25 || temp < 20){
    digitalWrite(LED_temp, HIGH);
  } else {
    digitalWrite(LED_temp, LOW);
  }

  Serial.print("Humidity (%): ");
  Serial.println((float)DHT11.humidity, 2);

  Serial.print("Temperature  (C): ");
  Serial.println((float)DHT11.temperature, 2);
}

void loop()
{
  checkLightIntensity();
  checkTempAndHumidity();
  
  // Check the values every second.
  delay(1000);
}
