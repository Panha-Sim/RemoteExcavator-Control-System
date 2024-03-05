#include <WiFi.h>

const char* ssid = "Come to RemoteExecavator Controller System";
const char* password = "234512345234@3#$123";
const char* host = "192.168.4.1"; // Server IP address

// ==========Joystick pins==========
const int joystick1XPin = 34;     //
const int joystick1YPin = 35;     //
const int joystick2XPin = 36;     //
const int joystick2YPin = 39;     //
// ========End Joystick Pins========

WiFiClient client;

void setup() {
  Serial.begin(115200);

  // Setup Pin
  pinMode(joystick1XPin, INPUT);
  pinMode(joystick1YPin, INPUT);
  pinMode(joystick2XPin, INPUT);
  pinMode(joystick2YPin, INPUT);

  // ================ Network ================
  WiFi.begin(ssid, password);               //
  // Connect to Wifi                        //
  while (WiFi.status() != WL_CONNECTED) {   //
    delay(500);                             //  
    Serial.print(".");                      //
  }                                         //
  Serial.println("WiFi connected");         //
  // Connect to the Server/Host             //
  while (!client.connect(host, 80)) {       //
    delay(500);                             //
    Serial.println("Connection failed");    //
  }                                         //
  // ============== End Network ==============

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

    // Grab value from all Joystick
    int xValue1 = analogRead(joystick1XPin);
    int yValue1 = analogRead(joystick1YPin);
    int xValue2 = analogRead(joystick2XPin);
    int yValue2 = analogRead(joystick2YPin);

    // Send a Post request to the server
    String url = "/joystick?x1=" + String(xValue1) + "&y1=" + String(yValue1) + "&x2=" + String(xValue2) + "&y2=" + String(yValue2);
    client.print(String("POST ") + url + " HTTP/1.1\r\n");

    
    Serial.println("Sending value to Server: ");
    Serial.println("Joystick 1: X = "+ String(xValue1) + ", Y = " + String (yValue1));
    Serial.println("Joystick 2: X = "+ String(xValue2) + ", Y = " + String (yValue2));
    Serial.println();
  }
}
