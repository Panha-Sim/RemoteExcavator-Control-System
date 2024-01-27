[1mdiff --git a/ClientServo/ClientServo.ino b/ClientServo/ClientServo.ino[m
[1mindex 6b68d93..f3142b3 100644[m
[1m--- a/ClientServo/ClientServo.ino[m
[1m+++ b/ClientServo/ClientServo.ino[m
[36m@@ -1,12 +1,44 @@[m
[31m-#include <WIFI.h>[m
[31m-#include <WIFIClient.h>[m
[32m+[m[32m#include <WiFi.h>[m
[32m+[m
[32m+[m[32mconst char *ssid = "ESP32-Access-Point";[m
[32m+[m[32mconst char *password = "123456789";[m
[32m+[m[32mconst char* host = "192.168.4.1";[m
[32m+[m
[32m+[m[32mWiFiClient client;[m
 [m
 void setup() {[m
[31m-  // put your setup code here, to run once:[m
[32m+[m[32m  Serial.begin(115200);[m
 [m
[32m+[m[32m  WiFi.begin(ssid, password);[m
[32m+[m[32m  while (WiFi.status() != WL_CONNECTED) {[m
[32m+[m[32m    delay(500);[m
[32m+[m[32m    Serial.print(".");[m
[32m+[m[32m  }[m
[32m+[m[32m  Serial.println("WiFi connected");[m
[32m+[m
[32m+[m[32m  while (!client.connect(host, 80)) {[m
[32m+[m[32m    delay(500);[m
[32m+[m[32m    Serial.println("Connection failed");[m
[32m+[m[32m  }[m
[32m+[m[32m  Serial.println("Host connected");[m
 }[m
 [m
 void loop() {[m
[31m-  // put your main code here, to run repeatedly:[m
[32m+[m[32m  if(!client.connected() || WiFi.status() != WL_CONNECTED){[m
[32m+[m[32m    while (WiFi.status() != WL_CONNECTED) {[m
[32m+[m[32m      delay(500);[m
[32m+[m[32m      Serial.print(".");[m
[32m+[m[32m    }[m
[32m+[m[32m    while (!client.connect(host, 80)) {[m
[32m+[m[32m      delay(500);[m
[32m+[m[32m      Serial.println(".");[m
[32m+[m[32m    }[m
[32m+[m[32m    Serial.println("Reconnected");[m
[32m+[m[32m  }else{[m
[32m+[m
[32m+[m[32m    String url = "/joystickValue";[m
[32m+[m[32m    client.print(String("GET ") + url + " HTTP/1.1\r\n");[m
[32m+[m[41m    [m
[32m+[m[32m  }[m
 [m
 }[m
[1mdiff --git a/server/server.ino b/server/server.ino[m
[1mindex c77fdc7..835c628 100644[m
[1m--- a/server/server.ino[m
[1m+++ b/server/server.ino[m
[36m@@ -44,6 +44,9 @@[m [mvoid loop() {[m
         currentLine += c;[m
         [m
         if(c == '\n'){[m
[32m+[m[32m          if(currentLine.startsWith("GET /joystickValue")){[m
[32m+[m[32m            Serial.println(client.remoteIP());[m
[32m+[m[32m          }[m
           if (currentLine.startsWith("POST /joystick")) {[m
             // Extract joystick values from the request[m
             int x1Index = currentLine.indexOf("x1=") + 3;[m
