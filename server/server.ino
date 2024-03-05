#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>

#define LED 2
#define MAXCLIENT 2

const char *ssid = "Come to RemoteExecavator Controller System";
const char *password = "234512345234@3#$123";

int joystickX1 = 90;
int joystickY1= 90;
int joystickX2= 90;
int joystickY2= 90;

WiFiServer server(80);
WiFiClient clients[MAXCLIENT];

long lastCommunicationTime[MAXCLIENT];
const long communicationTimeout = 2000;

void setup() {
  Serial.begin(115200);

  pinMode(LED,OUTPUT);

  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP Address: ");
  Serial.println(IP);

  server.begin();
}

void loop() {

  // When There is no Client connect 
  // Turn off the LED
  if(!clients[0] && !clients[1]){
    turnOffLED();
  }

  // ================= Accept Client =================
  if (WiFiClient newClient = server.available()) {  //
    for(int i=0; i < MAXCLIENT; i++){               //
      if(!clients[i] || !clients[i].connected()){   //
        turnOnLED();                                //
        clients[i] = newClient;                     //
        break;                                      //
      }                                             //
    }                                               //
  }                                                 //
  // ============= End Accept Client =================

  // Handle Client
   for (int i = 0; i < MAXCLIENT; i++) {

    // If client is still connecteed
    if (clients[i] && clients[i].connected()) {

      // If client is getting request
      if (clients[i].available()) {
        lastCommunicationTime[i] = millis(); // Update the last communication time of the client
        String currentLine = clients[i].readStringUntil('\n'); 
        handleClient(clients[i], currentLine); 

      } else if(millis()-lastCommunicationTime[i] > communicationTimeout) { // If the client stop sending request for 2 second

        //Remove Client Through Timeout
        Serial.print(clients[i].remoteIP());
        Serial.println(" has Dissconnected");
        clients[i].stop();
      }
    }
  }

}

// Handling the Client Logic
// Process (POST and GET) and 
// send out data
void handleClient(WiFiClient &client, String currentLine){

  if(currentLine.startsWith("GET /joystickValue")){
    Serial.println(currentLine);
    client.print("POST /joystickValue?x1=" + String(joystickX1) + "&y1=" + String(joystickY1) + "&x2=" + String(joystickX2) + "&y2=" + String(joystickY2) + " HTTP/1.1\r\n");
    Serial.println();
  }

  if (currentLine.startsWith("POST /joystick")) {
    // Extract joystick values from the request
    int x1Index = currentLine.indexOf("x1=") + 3;
    int y1Index = currentLine.indexOf("&y1=") + 4;
    int x2Index = currentLine.indexOf("&x2=") + 4;
    int y2Index = currentLine.indexOf("&y2=") + 4;
    
    joystickX1 = currentLine.substring(x1Index, currentLine.indexOf('&', x1Index)).toInt();
    joystickY1 = currentLine.substring(y1Index, currentLine.indexOf('&', y1Index)).toInt();
    joystickX2 = currentLine.substring(x2Index, currentLine.indexOf('&', x2Index)).toInt();
    joystickY2 = currentLine.substring(y2Index, currentLine.indexOf(' ', y2Index)).toInt();
  
    Serial.println("Joystick 1: X = "+ String(joystickX1) + ", Y = " + String (joystickY1));
    Serial.println("Joystick 2: X = "+ String(joystickX2) + ", Y = " + String (joystickY2));
    Serial.println();
  }

}

void turnOnLED(){
  digitalWrite(LED,HIGH);
}

void turnOffLED(){
  digitalWrite(LED,LOW);
}