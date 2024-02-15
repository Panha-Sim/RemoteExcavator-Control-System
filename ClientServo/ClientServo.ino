#include <WiFi.h>
#include <ESP32Servo.h>

const char *ssid = "ESP32-Access-Point";
const char *password = "123456789";
const char* host = "192.168.4.1";

WiFiClient client;

// int joystickX1;
// int joystickY1;
// int joystickX2;
// int joystickY2;

int joystickVal[4] = {1840, 1840, 1840, 1840};

unsigned long curMillis;
unsigned long readIntervalMillis = 100;
unsigned long lastReadMillis;

// int servoMax = 180;
// int servoMin = 0;
int servoMove = 5;  // number of degrees per movement

int potCentre = 1840; // adjust to suit your joystick
int potDeadRange = 300; // movements by this much either side of centre are ignored

Servo servos[4];
int servoPin[] = {32, 33, 25, 26};
int servoPos[] = {90, 90, 90, 90};
int servoMax[] = {180, 180, 180, 140}; //140
int servoMin[] = {0, 0, 0, 75}; //75

void setup() {
  Serial.begin(115200);

  //Attach Servo
  for(int i=0;i<sizeof(servos)/sizeof(Servo); i++){
    servos[i].attach(servoPin[i]);
  }

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
    joystickVal[0] = currentLine.substring(x1Index, currentLine.indexOf('&', x1Index)).toInt();
    joystickVal[1] = currentLine.substring(y1Index, currentLine.indexOf('&', y1Index)).toInt();
    joystickVal[2] = currentLine.substring(x2Index, currentLine.indexOf('&', x2Index)).toInt();
    joystickVal[3] = currentLine.substring(y2Index, currentLine.indexOf(' ', y2Index)).toInt();

    // joystickVal[0] = joystickX1;
    // joystickVal[1] = joystickY1;
    // joystickVal[2] = joystickX2;
    // joystickVal[3] = joystickY2;

    Serial.println("Receiving Joystick Value");
    Serial.println("Joystick 1: X = "+ String(joystickVal[0]) + ", Y = " + String (joystickVal[1]));
    Serial.println("Joystick 2: X = "+ String(joystickVal[2]) + ", Y = " + String (joystickVal[3]));
    Serial.println(servoPos[3]);
    Serial.println();


    
    
    
    curMillis = millis();
    if (curMillis - lastReadMillis >= readIntervalMillis) {
      lastReadMillis += readIntervalMillis;

    for(int i=0;i<sizeof(servos)/sizeof(Servo); i++){
      if (joystickVal[i] > potCentre + potDeadRange) {
        servoPos[i] += servoMove;
      }
      if (joystickVal[i] < potCentre - potDeadRange) {
        servoPos[i] -= servoMove;
      }
    }
      
      checkDeadZone();

	  } 

    moveServo();
    
  }

}

void moveServo() {
	for(int i = 0; i<sizeof(servos)/sizeof(Servo); i++){
    servos[i].write(servoPos[i]);
  }
}

void checkDeadZone(){
 // check that the values are within limits
  for(int i=0; i < sizeof(servoPos)/sizeof(int); i++){
    if (servoPos[i] > servoMax[i]) {
      servoPos[i] = servoMax[i];
    }
    if(servoPos[i] < servoMin[i]) {
      servoPos[i] = servoMin[i];
    }
  }
}

