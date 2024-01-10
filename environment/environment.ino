// Remember to check the ports
// Make sure the 3V3 and GND wires are connected

#include <dht11.h>

// Define the correct ports
#define DHT11PIN D7
#define LED_HUMID D1
#define LED_TEMP D2
#define LED_LIGHT D4
#define PHOTO_RESISTOR A0

dht11 DHT11;

// Set pinmodes for the LEDs and start the serial monitor.
void  setup()
{
  Serial.begin(115200);
  pinMode(LED_HUMID, OUTPUT);
  pinMode(LED_TEMP, OUTPUT);
  pinMode(LED_LIGHT, OUTPUT);
}

// Checks the light intensity with a photo resistor and raises alarm if value is too high.
void checkLightIntensity() {
  int value = analogRead(PHOTO_RESISTOR);

  Serial.println("Analog  Value: ");
  Serial.println(value);
    
  if (value > 900) {
    digitalWrite(LED_LIGHT, HIGH);
  } else {
    digitalWrite(LED_LIGHT,  LOW);
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
    digitalWrite(LED_HUMID, HIGH);
  } else {
    digitalWrite(LED_HUMID, LOW);
  }

  if (temp > 27 || temp < 15){
    digitalWrite(LED_TEMP, HIGH);
  } else {
    digitalWrite(LED_TEMP, LOW);
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
