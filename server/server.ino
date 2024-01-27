#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>

#define LED 2
#define MAXCLIENT 2

const char *ssid = "ESP32-Access-Point";
const char *password = "123456789";

long lastCommunicationTime[MAXCLIENT];
const long communicationTimeout = 2000;

int joystickX1 = 90;
int joystickY1= 90;
int joystickX2= 90;
int joystickY2= 90;

WiFiServer server(80);
WiFiClient clients[MAXCLIENT];

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

  if(!clients[0] && !clients[1]){
    turnOffLED();
  }

  //Accept Client
  if (WiFiClient newClient = server.available()) {
    for(int i=0; i < MAXCLIENT; i++){
      if(!clients[i] || !clients[i].connected()){
        turnOnLED();
        clients[i] = newClient;
        break;
      }
    }
  }

  //Handle Client
   for (int i = 0; i < MAXCLIENT; i++) {

    if (clients[i] && clients[i].connected()) {

      if (clients[i].available()) {
        lastCommunicationTime[i] = millis();
        String currentLine = clients[i].readStringUntil('\n');
        handleClient(clients[i], currentLine);

      } else if(millis()-lastCommunicationTime[i] > communicationTimeout) {

        //Remove Client Through Timeout
        Serial.print(clients[i].remoteIP());
        Serial.println(" has Dissconnected");
        clients[i].stop();
      }
    }
  }

}

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