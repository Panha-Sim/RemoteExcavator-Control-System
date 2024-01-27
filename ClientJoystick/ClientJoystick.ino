#include <WiFi.h>

const char* ssid = "ESP32-Access-Point";
const char* password = "123456789";
const char* host = "192.168.4.1"; // Server IP address

// Joystick pins
const int joystick1XPin = 34; 
const int joystick1YPin = 35; 
const int joystick2XPin = 32;
const int joystick2YPin = 33;

WiFiClient client;

void setup() {
  Serial.begin(115200);
  pinMode(joystick1XPin, INPUT);
  pinMode(joystick1YPin, INPUT);
  pinMode(joystick2XPin, INPUT);
  pinMode(joystick2YPin, INPUT);

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

    int xValue1 = analogRead(joystick1XPin);
    int yValue1 = analogRead(joystick1YPin);
    int xValue2 = analogRead(joystick2XPin);
    int yValue2 = analogRead(joystick2YPin);

    String url = "/joystick?x1=" + String(xValue1) + "&y1=" + String(yValue1) + "&x2=" + String(xValue2) + "&y2=" + String(yValue2);
    client.print(String("POST ") + url + " HTTP/1.1\r\n");

    
    Serial.println("Sending value to Server: ");
    Serial.println("Joystick 1: X = "+ String(xValue1) + ", Y = " + String (yValue1));
    Serial.println("Joystick 2: X = "+ String(xValue2) + ", Y = " + String (yValue2));
    Serial.println();
  }
}
