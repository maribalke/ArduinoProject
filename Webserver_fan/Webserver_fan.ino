#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>   // Include the Wi-Fi-Multi library
#include <ESP8266WebServer.h>   // Include the WebServer library
#include <ESP8266mDNS.h>        // Include the mDNS library
#include <Stepper.h>

ESP8266WiFiMulti wifiMulti;
// Create an instance of the server
ESP8266WebServer server(80);

const int stepsPerRevolution = 80;  // change this to fit the number of steps per revolution
// for your motor
int count = 0;
bool on = false;

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, D2, D3, D4, D5);

void handleRoot();  
void handleNotFound();
void setStateFan();

void setup() {
  Serial.begin(115200);
  delay(10);

  myStepper.setSpeed(200);
  
  // Connect to WiFi network
  Serial.println();
  wifiMulti.addAP("mariBF", "marimari");  // add Wi-Fi networks you want to connect to
  //wifiMulti.addAP("<ssid2>", "<password>");  
  
  Serial.println();
  Serial.print("Connecting ...");
  //WiFi.begin(ssid, password);
 
  while (wifiMulti.run() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected to ");
  Serial.println(WiFi.SSID());
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
    
  // Start the server
  server.begin();
  Serial.println("Server started"); 
}

void loop() {
  // Check if a client has connected
  
  if(on){
    myStepper.step(1);
  } else{
    myStepper.step(0);
  }

  server.handleClient();
}

void handleRoot() {
  String page = "<!DOCTYPE html>"
                "<html>"
                "<head>"
                "<title>Turn fan on and off</title>"
                "<style>"
                "body { font-family: Arial, sans-serif; text-align: center; }"
                ".container { margin-top: 50px; }"
                ".btn { width: 200px; height: 60px; font-size: 18px; background-color: #4CAF50; color: white; border: none; border-radius: 5px; cursor: pointer; }"
                ".btn:hover { background-color: #45a049; }"
                "</style>"
                "</head>"
                "<body>"
                "<div class='container'>"
                "<h1>Turn fan on and off</h1>"
                "<p>In order to lower the temperature in the art gallery, we have installed a fan that can manually be turned on and off.</p>"
                "<p>Click the button below to turn the fan on or off</p>"
                "<form action='/FAN' method='POST'>"
                "<input type='submit' value='Change Fan State' class='btn'>"
                "</form>"
                "</div>"
                "</body>"
                "</html>";

  server.send(200, "text/html", page);
}


void setStateFan(){
  /*
  Function that set the state of the fan to the opposite of its previous state by using a count variable
  */

  count ++;
  if(count%2 == 0){
    on = false;
  }else{
    on = true;
  }
  server.sendHeader("Location","/");        // Add a header to respond with a new location for the browser to go to the home page again
  server.send(303);    // Send it back to the browser with an HTTP status 303 (See Other) to redirect          
}

void handleNotFound(){
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}
  
