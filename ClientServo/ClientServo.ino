#include <WiFi.h>
#include <ESP32Servo.h>

const char *ssid = "ESP32-Access-Point";
const char *password = "123456789";
const char* host = "192.168.4.1";

WiFiClient client;

int joystickX1;
int joystickY1;
int joystickX2;
int joystickY2;

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");

  while (!client.connect(host, 80)) {
    delay(500);
    Serial.println("Connection failed");
  }
  Serial.println("Host connected");
}

void loop() {

  // Check for wifi and Server Connection
  if(!client.connected() || WiFi.status() != WL_CONNECTED){
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    while (!client.connect(host, 80)) {
      delay(500);
      Serial.println(".");
    }
    Serial.println("Reconnected");
  }else{

    //Send Request to server
    String url = "/joystickValue";
    client.print(String("GET ") + url + " HTTP/1.1\r\n");

    //Read Respones from Server
    String currentLine = client.readStringUntil('\n');

    int x1Index = currentLine.indexOf("x1=") + 3;
    int y1Index = currentLine.indexOf("&y1=") + 4;
    int x2Index = currentLine.indexOf("&x2=") + 4;
    int y2Index = currentLine.indexOf("&y2=") + 4;
    
    //Grab Joystick Value
    joystickX1 = currentLine.substring(x1Index, currentLine.indexOf('&', x1Index)).toInt();
    joystickY1 = currentLine.substring(y1Index, currentLine.indexOf('&', y1Index)).toInt();
    joystickX2 = currentLine.substring(x2Index, currentLine.indexOf('&', x2Index)).toInt();
    joystickY2 = currentLine.substring(y2Index, currentLine.indexOf(' ', y2Index)).toInt();
  
    Serial.println("Receiving Joystick Value");
    Serial.println("Joystick 1: X = "+ String(joystickX1) + ", Y = " + String (joystickY1));
    Serial.println("Joystick 2: X = "+ String(joystickX2) + ", Y = " + String (joystickY2));
    Serial.println();

    
  }

}
