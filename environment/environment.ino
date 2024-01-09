#include <dht11.h>
#define DHT11PIN 2
#define LED_humid 5
#define LED_temp 10

dht11 DHT11;

void  setup()
{
  Serial.begin(115200);
  pinMode(LED_humid, OUTPUT);
  pinMode(LED_temp, OUTPUT);
 
}

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
  checkTempAndHumidity();
  delay(2000);

}
