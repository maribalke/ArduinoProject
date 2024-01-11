// Remember to check the ports
// Make sure the 3V3 and GND wires are connected

#include <dht11.h>
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

const char* ssid = "Iphonemari";
const char* pass = "mari1234";
WiFiClient client;
unsigned long channelID = 2399509;  //your TS channal
const char* APIKey = "735H239HRB9DNBFA"; // Replace with your Write API Keyconst char* APIKeyRead = "CYB6IXO89M7E5RFD";   //your reading API
const char* server = "api.thingspeak.com";

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
<<<<<<< HEAD
  WiFi.begin(ssid, pass);
}

const int VIN = 5.0;
const int R_light = 10000.0;

int sensorRawToPhys(int aValue){
  // Code from https://www.aranacorp.com/en/luminosity-measurement-with-a-photoresistor/
  // Conversion rule
  float Vout = float(aValue) * (VIN / float(1023));// Conversion analog to voltage
  float RLDR = (R_light * (VIN - Vout))/Vout; // Conversion voltage to resistance
  int phys=500/(RLDR/1000); // Conversion resitance to lumen
  return phys;
=======
>>>>>>> 135e296618deba5328bfe0e2a93004308c3bc022
}

// Checks the light intensity with a photo resistor and raises alarm if value is too high.
void checkLightIntensity() {
  int value = analogRead(PHOTO_RESISTOR);

  float lux = sensorRawToPhys(value);     
  Serial.println("Light intensity in Lux: ");
  Serial.println(lux);

  ThingSpeak.setField(6, lux);
    
<<<<<<< HEAD
  if (lux > 400 || lux < 0) {
=======
  if (value > 900) {
>>>>>>> 135e296618deba5328bfe0e2a93004308c3bc022
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
  ThingSpeak.setField(3, humidity);  
  ThingSpeak.setField(4, temp);

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

  ThingSpeak.begin(client);
  client.connect(server, 80);

  checkLightIntensity();
  checkTempAndHumidity();
  
  ThingSpeak.writeFields(channelID, APIKey);
  client.stop();
  
  // Check the values every second.
  delay(1000);
}
