#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>   // Include the Wi-Fi-Multi library
#include <ESP8266WebServer.h>   // Include the WebServer library
#include <ESP8266mDNS.h>        // Include the mDNS library
#include <Stepper.h>
#include <EEPROM.h>

ESP8266WiFiMulti wifiMulti;
// Create an instance of the server
ESP8266WebServer server(80);

// Variables for controlling the fan
const int stepsPerRevolution = 80; 
const int rpm = 400;
int fanCount = 0;
bool fanOn = false;
String btnTxt = "Turn fan on";
String btnColor = "green";

// Variables for putting the system to sleep
int eepromAddress = 0;
int btnCount = 0;

// initialize the stepper library on pins D0 through D3:
Stepper myStepper(stepsPerRevolution, D0, D2, D1, D3);

void handleRoot();  
void handleNotFound();
void setStateFan();

void setup() {
  Serial.begin(115200);
  delay(10);

  // Setting speed of stepMotor  
  myStepper.setSpeed(rpm);
  
  /* 
  Connecting to WiFi network and set up web server
  */
  wifiMulti.addAP("mariBF", "marimari"); 
 
  Serial.println();
  Serial.print("Connecting ...");
 
  while (wifiMulti.run() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("iot")) {              // Start the mDNS responder for esp8266.local
    Serial.println("mDNS responder started");
  } else {
    Serial.println("Error setting up MDNS responder!");
  }

  server.on("/", HTTP_GET, handleRoot);
  server.on("/FAN", HTTP_POST, setStateFan);
  server.onNotFound(handleNotFound);
  server.begin();

  /*
  EEPROM
  Write and read from EEPROM memory. The memory is read each time the program is started and the variable 'btnCount' is updated.
  We increment the variable and overwrite the memory with the updated variable. The variable 'btnCount' is to keep track of wether the system should
  go into sleep or wake up from sleep. 
  */
  
  // Allocates 12 bytes for writing to EEPROM memory
  EEPROM.begin(12);

  EEPROM.get(eepromAddress, btnCount);
  btnCount++;

  EEPROM.put(eepromAddress, btnCount);
  EEPROM.commit();
  EEPROM.end();

  if (btnCount%2 == 0){
    Serial.println("System is in deep sleep mode");
    // Deep sleep mode until RESET is triggered again
    ESP.deepSleep(0); 
  }
}

void loop() {

  /*
  The fan is controlled using the global variable 'fanOn'. The motor will move as long as this variable is true.
  */

  if(fanOn){
    myStepper.step(1);
  } else{
    myStepper.step(0);
  }
  // Check if a client has connected
  server.handleClient();
}

void handleRoot() {

  /*
  Function with html-code to create the web-server
  */

  String page = "<!DOCTYPE html>"
                "<html>"
                "<head>"
                "<title>Turn fan on and off</title>"
                "<style>"
                "body { font-family: Arial, sans-serif; text-align: center; }"
                ".container { margin-top: 50px; }"
                ".btn { width: 200px; height: 60px; font-size: 18px; background-color:" +btnColor+ "; color: white; border: none; border-radius: 5px; cursor: pointer; }"
                ".btn:hover { background-color:"+btnColor+"; }"
                "</style>"
                "</head>"
                "<body>"
                "<div class='container'>"
                "<h1>Turn fan on and off</h1>"
                "<p>In order to lower the temperature in the art gallery, we have installed a fan that can manually be turned on and off.</p>"
                "<p>Click the button below to turn the fan on or off</p>"
                "<form action='/FAN' method='POST'>"
                "<input type='submit' value='"+btnTxt+"'class='btn'>"
                "</form>"
                "</div>"
                "</body>"
                "</html>";

  server.send(200, "text/html", page);
}


void setStateFan(){
  /*
  Function that set the state of the fan to the opposite of its previous state by using a count variable 'fanCount'. 
  The function is also changing the variables 'btnText' and 'btnColor' which are used for the appearance of the web server. 
  */

  fanCount ++;
  if(fanCount%2 == 0){ 
    fanOn = false;
    btnTxt = "Turn fan on";
    btnColor = "green";
  }else{
    fanOn = true;
    btnTxt = "Turn fan off";
    btnColor = "red";
  }
  server.sendHeader("Location","/"); // Add a header to respond with a new location for the browser to go to the home page again
  server.send(303);   // Send it back to the browser with an HTTP status 303 (See Other) to redirect          
}

void handleNotFound(){
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}
  
