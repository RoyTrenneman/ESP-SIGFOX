#include <Wire.h>
#include <WiFiClient.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>
#include <Arduino.h>
#include <SoftwareSerial.h>
#include "wisol_sigfox.h"

const char* ssid = "Livebox";
const char* password =  "******";
const int mqttPort = 1883;
const char* mqttUser = "user";
const char* mqttPassword = "****";
const char* mqttServer = "192.168.1.18";
String  inputString ;
char inChar ;
bool data = false;
// Software Serial to be used to communicate with SigFox module
// Change RX/TX pins according to your setup.
SoftwareSerial  wisol_serial(5,4) ;
WiFiClient espClient;                                                                                      
PubSubClient client(espClient);

#ifdef __cplusplus
extern "C" {
#endif

/** initialize software serial with proper timeouts */
void wisol_sigfox__serial_init() {
  wisol_serial.begin(9600);
  wisol_serial.setTimeout(1000);
}


/** adapter function for synchronous communication with module */
void wisol_sigfox__serial_sync(const char *cmd, char *p_response_buf, int sz_response_buf, int timeout) {
  wisol_serial.setTimeout(timeout);
  wisol_serial.print(cmd); wisol_serial.print("\n");
delay(1000);
}

#ifdef __cplusplus
}
#endif

void UpgradeOTA() {
  ArduinoOTA.onStart([]() {
      //////Serial.println("Start");
    });
  ArduinoOTA.onEnd([]() {
      //////Serial.println("End");
    });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
     ////Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
  ArduinoOTA.onError([](ota_error_t error) {
         ////Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR) Serial.println("End Failed");
    ESP.restart();
  });
  ArduinoOTA.begin();
}

void StartingWifi() {

  // Starting Wifi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
    }
}

bool sigfoxSend(const void* data, uint8_t len) {

 return wisol_sigfox__send_frame((const uint8_t*)data, 12, true) ;

}

                                     
void setup() {
 
 Serial.begin(9600);
 pinMode(14, OUTPUT); // RED LED
  // Set console baud rate
 digitalWrite(14, LOW); 
 struct gpscoord {
  float a_latitude;  // 4 bytes
  float a_longitude; // 4 bytes
  int   a_altitude;  // 4 bytes
 };
 // Simulate GPS data
 float longitude  = 1.58;
 float latitude = 42.755;
 int   altitude  = 0;
 gpscoord coords = {latitude, longitude, altitude};

 StartingWifi();
 UpgradeOTA(); 
 delay(1000);
 wisol_sigfox__serial_init();
 //wisol_sigfox__ready(); 
 client.setServer(mqttServer, mqttPort);
 client.connect("ESP8266Client", mqttUser, mqttPassword);
 client.publish("sigfox/TEST", "SEND_DATA");
 sigfoxSend(&coords, sizeof(gpscoord))  ;
}

void sendmqtt() {
  inputString.trim();
  String MyByte = inputString.substring(21);
  client.connect("ESP8266Client", mqttUser, mqttPassword);
  client.publish("sigfox/RECEIVE_DATA", String(MyByte).c_str() );
  client.connect("ESP8266Client", mqttUser, mqttPassword);
  client.publish("sigfox/RECEIVE_DATA", "END" );

}

void loop() {
  static int i = 0;
  ArduinoOTA.handle();
  if (wisol_serial.available() > 0 ) {
   delay(1000);
   while ( wisol_serial.available() > 0) {
    // get the new byte:
    delay(1000);
    char inChar = (char)wisol_serial.read();
    Serial.print(inChar);
    if (inChar == '\n') {
     while ( wisol_serial.available() > 0) {
      delay(500);
      char inChar = (char)wisol_serial.read();
      i++;
      if ( i > 3) inputString += inChar;
      data = true;
      digitalWrite(14, HIGH); 
      }
    }
   }
  }
 if (data) { 
 sendmqtt();
 data = false; 
 }

}

