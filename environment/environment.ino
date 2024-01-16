/*
Responsibility: 
Mari Helgesen, s231705 (60%) 
Mari Balke Fjellang, s231450 (40%)
*/

#include <dht11.h>
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

// Hotspot credentials
const char* ssid = "mariBF";
const char* pass = "marimari";

// Initiate the client
WiFiClient client;

// ThingSpeak settings
unsigned long channelID = 2399509;  
const char* APIKey = "735H239HRB9DNBFA"; 
const char* server = "api.thingspeak.com";

// Define the correct pins
#define DHT11PIN D7
#define LED_HUMID D1
#define LED_TEMP D2
#define LED_LIGHT D4
#define PHOTO_RESISTOR A0

// Initiate the humidity sensor object
dht11 DHT11;

// Set pinmodes for the LEDs and start the serial monitor.
void  setup()
{
  Serial.begin(115200);
  pinMode(LED_HUMID, OUTPUT);
  pinMode(LED_TEMP, OUTPUT);
  pinMode(LED_LIGHT, OUTPUT);
  WiFi.begin(ssid, pass);
}

// Function for translating from analog value to lux
// Collected from https://www.aranacorp.com/en/luminosity-measurement-with-a-photoresistor/
int sensorRawToPhys(int aValue){
  const int VIN = 5.0;
  const int R_light = 10000.0;
  // Conversion rule
  float Vout = float(aValue) * (VIN / float(1023));// Conversion analog to voltage
  float RLDR = (R_light * (VIN - Vout))/Vout; // Conversion voltage to resistance
  int phys=500/(RLDR/1000); // Conversion resitance to lumen
  return phys;
}

// Checks the light intensity with a photo resistor and raises alarm if value is too high.
int checkLightIntensity() {
  int value = analogRead(PHOTO_RESISTOR);
  float lux = sensorRawToPhys(value);
       
  Serial.println("Light intensity in Lux: ");
  Serial.println(lux);

  if (lux > 400 || lux < 0) {
    digitalWrite(LED_LIGHT, HIGH);
  } else {
    digitalWrite(LED_LIGHT,  LOW);
  }

  return lux;
}


// Uses the humidity sensor to check temperature. 
// Raises alarm if the value is outside acceptable interval.
int checkTemp() {
  int chk = DHT11.read(DHT11PIN);
  float temp = (float)DHT11.temperature;

  if (temp > 27 || temp < 15){
    digitalWrite(LED_TEMP, HIGH);
  } else {
    digitalWrite(LED_TEMP, LOW);
  }

  Serial.print("Temperature  (C): ");
  Serial.println((float)DHT11.temperature, 2);
  return temp;
}

// Uses the humidity sensor to check humidity. 
// Raises alarm if the value is outside acceptable interval.
int checkHumidity() {
  int chk = DHT11.read(DHT11PIN);
  float humidity = (float)DHT11.humidity;
  
  if (humidity > 65 || humidity < 40){
    digitalWrite(LED_HUMID, HIGH);
  } else {
    digitalWrite(LED_HUMID, LOW);
  }

  Serial.print("Humidity (%): ");
  Serial.println((float)DHT11.humidity, 2);
  return humidity;
}

void loop()
{
  ThingSpeak.begin(client);
  client.connect(server, 80);

  // Use functions to get measurements of environmental values
  float lux = checkLightIntensity();
  float temp = checkTemp();
  float humidity = checkHumidity();
  
  // Write measurements to ThingSpeak for visualisation
  ThingSpeak.setField(3, humidity);  
  ThingSpeak.setField(4, temp);
  ThingSpeak.setField(6, lux);
  ThingSpeak.writeFields(channelID, APIKey);
  client.stop();
  
  // Check the values every second
  delay(1000);
}
