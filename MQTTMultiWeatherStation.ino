//VERSION!!
const char* fwVersion = "2.5.7";

//timeClient.getFormattedTime() could be used?

//GROOVE HUMAN SENSOR!
//#include "Grove_Human_Presence_Sensor.h" 
////#include <Grove_Human_Presence_Sensor.h>
//AK9753 movementSensor;
//
//float ir1, ir2, ir3, ir4;
//float diff13, diff24;
//float tempGroove;

//QWIIC HUMAN SENSOR! https://learn.sparkfun.com/tutorials/qwiic-human-presence-sensor-ak9753-hookup-guide/all
#include "SparkFun_AK975X_Arduino_Library.h" //Use Library Manager or download here: https://github.com/sparkfun/SparkFun_AK975X_Arduino_Library

AK975X movementSensor; //Hook object to the library

int ir1, ir2, ir3, ir4, ir1old = 0, ir2old = 0, ir3old = 0, ir4old = 0, temperature = 0;
int diff13, diff24, diff1324, diff13old = 0, diff24old = 0, diff1324old = 0;

//ROOM WHERE SENSOR BELONGS
//#define BAD       //54
//#define GARAGE    //55
//#define STUE      //56 (146)
//#define MOBILE    //57 (172)
//NOT USEABLE YET!//#define TOILET    //58 (172)
#define LOFT    //59 (172)
//#define WEBTEST   //234 (197)

#ifdef BAD
#define ID "bad" ;      //54
#elif defined GARAGE
#define ID "garage" ;   //55
#elif defined BRYGGERS
#define ID "bryggers" ; //59
#elif defined STUE
#define ID "stue" ;     //56
#elif defined MOBILE
#define ID "mobile" ;   //57
#elif defined TOILET
#define ID "toilet" ;   //58
#elif defined LOFT
#define ID "loft" ;     //59
#elif defined WEBTEST
#define ID "webtest" ;  //234
#endif

//includes, defines and variables for WIFI and Wire readout
#include <ESP8266WiFi.h>      //wifi for ESP8266
#include <stdio.h>            //for generelle C
#include <Wire.h>             //for i2c
#include <ESP8266mDNS.h>      //for at vise multicast DNS hostname
WiFiClient espClient;

//includes, defines and variables for multi WiFi connects
#include <ESP8266WiFiMulti.h>
ESP8266WiFiMulti wifiMulti;

//includes, defines and variables for MQTT
#include <PubSubClient.h>     //for mqtt
PubSubClient client(espClient);

//includes, defines and variables for webserver
#include <ESP8266WebServer.h> //webserver for ESP8266
ESP8266WebServer server(80);

//includes, defines and variables for OLED monitor //https://github.com/G6EJD/ESP8266-NTP-time-on-an-OLED-with-WiFi-Manager/blob/master/Adafruit_SSD1306.cpp
#include <Adafruit_GFX.h>
#include "Adafruit_SSD1306.h" //lokal ved siden af .ino filen for 48x64 opløsning
#define OLED_RESET 0  // GPIO0
Adafruit_SSD1306 display(OLED_RESET);
#define XPOS 0      //måske ligemeget?
#define YPOS 1      //måske ligemeget?
#define DELTAY 2    //måske ligemeget?

#if (SSD1306_LCDHEIGHT != 64) //(48 for lille, 64 for mellem og stor)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

time_t prevDisplay = 0;

//includes, defines and variables for DHT sensor
#include <DHT.h>          // from https://github.com/markruys/arduino-DHT
#define DHT22_PIN 13      // Data wire from DHT-22 to arduino
DHT dht;

//includes, defines and variables for BME280 sensor (temperature, humidity and pressure)
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C

//includes, defines and variables for WiFi UDP
#include <WiFiUdp.h>
WiFiUDP ntpUDP;

//includes, defines and variables for ntp
#include <NTPClient.h>
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 15000); // default 0.dk.europe.pool.ntp.org 0.dk. //60.000 normalt

#include "TimeLib.h"
time_t ntpSyncProvider() {
  return timeClient.getEpochTime();
}

//includes, defindes and variables for OTA
#include <ArduinoOTA.h>

//includes, defines and variables for HTTP client (posting data)
#include <ESP8266HTTPClient.h>
HTTPClient http;

//includes, defines and variables for Websocket use
#include <WebSocketsServer.h>

//includes, defines and variables for MAX31855 thermocupler unit
#include <SPI.h>
#include "Adafruit_MAX31855.h"

// Example creating a thermocouple instance with software SPI on any three
// digital IO pins.
//#define MAXDO   3
//#define MAXCS   4
//#define MAXCLK  5

// initialize the Thermocouple
//Adafruit_MAX31855 thermocouple(MAXCLK, MAXCS, MAXDO);

// Example creating a thermocouple instance with hardware SPI
// on a given CS pin.
//#define MAXCS   10
//Adafruit_MAX31855 thermocouple(MAXCS);

//EVENT HANDLERS, IE BUTTON PUSHES AND SUCH
WiFiEventHandler gotIpEventHandler, disconnectedEventHandler;

ADC_MODE(ADC_VCC); // to enable source voltage to be measured

float temp;
float hum;
float lastTemp;
float lastHum;
float lastSentTemp;
float lastSentHum;
float deltaTemp;
float deltaHum;

float bmeTemp;
float bmeHum;
float bmePres;
float bmeAlti;
float lastBMETemp;
float lastBMEHum;
float lastBMEPres;
float lastBMEAlti;
float lastSentBMETemp;
float lastSentBMEHum;
float lastSentBMEPres;
float lastSentBMEAlti;
float bmeDeltaTemp;
float bmeDeltaHum;
float bmeDeltaPres;
float bmeDeltaAlti;

float ESPVoltage;

const char* ssidOne = "xxx";
const char* passwordOne = "xxx";

const char* ssidTwo = "xxx";
const char* passwordTwo = "xxx";

const char* mqtt_server = "xxx";
const char* MQTTUser = "xxx";
const char* MQTTPass = "xxx";

const char* text = "";

const char* wifiStatus = "";
const char* mqttStatus = "";
const char* deviceId = ID; //

char payloadTemp[50] = "";
char payloadHum[50] = "";
char payloadDeltaTemp[50] = "";
char payloadDeltaHum[50] = "";
char payloadDeltaBMETemp[50] = "";
char payloadDeltaBMEHum[50] = "";
char payloadDeltaBMEPres[50];

char payloadNTPTime[50] = "";
char payloadIP[50] = "";
char payloadRSSI[50] = "";
char payloadSSID[50] = "";
char payloadAPMAC[50] = "";
char payloadMQTTReconnects[50] = "0";
char payloadWIFIReconnects[50] = "0";
char payloadUptime[50] = "";
char payloadReason[50] = "";
char toLogArray[150] = "";

char logInfo[50] = "";
char logTime[50] = "";

char addressMac[50] = "";
char initialIP[50] = "";

char payloadBMETemp[50] = "";
char payloadBMEHum[50] = "";
char payloadBMEPres[50] = "";
char payloadBMEAlti[50] = "";

char topic[100];
char topicBME[100];
char topicStatus[100];
char topicDetector[100];

char payload[300]; 
char payloadBME[100]; // for BME280 sensor
char payloadStatus[300]; 
char payloadDetector[300];

char data[300];

String networkSSID;
//String routerBSSID;
String addressMacString = "";

String dataArray[10];
String statusArray[10];
String realLogArray[25]; //[0]

bool firstMQTTMessageSent = false;
bool sendStatusMQTT = true;
bool enableOLED = true;
bool OLEDLarge = false;
bool ntpTimeSynced = false;
bool bmeSensorOnline = false;
bool doorState = false;
bool lastDoorState = true;
bool sensorForceUpdate = false;

int i = 0; //counter for data array (write)
int e = 0; //counter for data array (read)
int a = 0; //counter for status array (write)
int u = 0; //counter for status array (read)

int timeHour = 0;
int timeMinute = 0;
int timeSecond = 0;
int timeDay = 0;
int timeMonth = 0;
int timeYear = 0;
int rollOverCount = 0;

int httpResponseCode = 0;

unsigned long rollOverTime;
unsigned long lastRollOverTime;

long lastMsg = 0;
long lastStatusUpdate = 0;
long lastThreeSecTimerRead = 0;
long lastFiveMinTimerRead = 0;
long lastFifteenMinTimerRead = 0;
long lastRead = 0;
long lastOLEDUpdate = 0;
long sentMQTTMessages = 0;
long wifiReconnects = 0;
long mqttReconnects = 0;
long wifiGotIP = 0;
long syncTime = 0;
long startTime = 0;
long endTime = 0;
long loopTime = 0;
long lastLoopTime = 0;
long shortestLoopTime = 1;
long longestLoopTime = 0;
long longestLoopTimeThisPeriod = 0;

//Common interupt routine for button
void ICACHE_RAM_ATTR ProcessInterupt(bool state) { //int knap,
  if (state == true ) {
    addLogEntry("Button state false"); //false then contact is pressed down - inverted compared to the state, as a pullup resistor is used
    enableOLED = !enableOLED;
  } else if (state == false) {
    addLogEntry("Button state true"); //true then contact is pressed down - inverted compared to the state, as a pullup resistor is used
  }
  Serial.println(state);
  if (enableOLED == true) {
    addLogEntry("Button Toggle OLED ON pressed (physical)");
  } else if (enableOLED == false) {
    addLogEntry("Button Toggle OLED OFF pressed (physical)");
  }
}

//Common interupt routine for Reed
void ICACHE_RAM_ATTR ProcessInteruptReed(bool state) { //REED ,
  if (state == true ) {
    addLogEntry("Reed state false"); //false then contact is pressed down - inverted compared to the state, as a pullup resistor is used
    //CODE TO EXECUTE WHEN STATE = TRUE
    Serial.println("REED: DOOR OPEN");
    Serial.print(state);
    Serial.print(" ");
    Serial.println(doorState);
    doorState = state;
  } else if (state == false) {
    addLogEntry("Reed state true"); //true then contact is pressed down - inverted compared to the state, as a pullup resistor is used
    //CODE TO EXECUTE WHEN STATE = FALSE
    Serial.println("REED: DOOR CLOSED");
    Serial.print(state);
    Serial.print(" ");
    Serial.println(doorState);
    doorState = state;
  }
}

// Interupt rutine for stateChangeButton
void ICACHE_RAM_ATTR stateChangeButton() {
  int buttonPin = 14;
  bool state = digitalRead(buttonPin);
  ProcessInterupt(state);
  //Serial.println(state);
}

// Interupt rutine for stateChangeReed
void ICACHE_RAM_ATTR stateChangeReed() {
  int reedPin = 2;
  bool state = digitalRead(reedPin);
  ProcessInteruptReed(state);
  //Serial.println(state);
}

void setup() {
  //WEMOS D1 PINLAYOUT!
  //pinMode(5, OUTPUT);           //D1 USED TO I2C OLED/BME280
  //pinMode(4, OUTPUT);           //D2 USED TO I2C OLED/BME280
  //pinMode(0, INPUT);            //D3
  pinMode(2, INPUT_PULLUP);       //D4 USED TO REED SWITCH, pulled to ground when active
  pinMode(14, INPUT_PULLUP);      //D5 USED TO BUTTON, pulled to ground when active
  //pinMode(12, INPUT_PULLUP);    //D6
  //pinMode(13, INPUT_PULLUP);    //D7 USED TO DHT22
  //pinMode(15, INPUT_PULLUP);    //D8
  //pinMode(9, INPUT_PULLUP);     //SD2
  //pinMode(10, INPUT_PULLUP);    //SD3
  //pinMode(16, INPUT);           //D0
  //pinMode(2, ad?);              //A0

  Serial.begin(115200);
  setup_arrays();
  setup_oled();
  delay(200);
  setup_wifi();
  delay(200);
  setup_mqtt();
  delay(200);
  setup_webserver();
  setup_ntp();
  delay(200);
  setup_sensors();
  delay(200);
  setup_ota();
  delay(200);
  setup_human_detection();
  delay(200);

  //add interrupts
  attachInterrupt(digitalPinToInterrupt(2), stateChangeReed, CHANGE); //buttonPin = 2
  attachInterrupt(digitalPinToInterrupt(14), stateChangeButton, CHANGE); //buttonPin = 14
  //Just showing off that it can detect a connect (obtained IP). Should it do anything?
  gotIpEventHandler = WiFi.onStationModeGotIP([](const WiFiEventStationModeGotIP & event) {
    Serial.print("Station connected, IP: ");
    wifiGotIP++;
    Serial.println(WiFi.localIP());
    sprintf(payloadReason, "GotIP");
    addLogEntry("WiFI got IP");
    sendMQTTStatus();
  });

  //Just showing off that it can detect disconnect. Should it handle the reconnect thing?
  disconnectedEventHandler = WiFi.onStationModeDisconnected([](const WiFiEventStationModeDisconnected & event) {
    Serial.println("Station disconnected");
    sprintf(payloadReason, "Disconnected");
    addLogEntry("WIFI disconnected");
    sendMQTTStatus();
  });

  Serial.println("Setup done, executing code...");
}

void loop()
{
  startTime = micros();
  
  rollOverTime = millis();
  if (lastRollOverTime > rollOverTime) {
    rollOverCount = rollOverCount + 1;
  }
  lastRollOverTime = rollOverTime;
  
  //make wifi status readable
  if (WiFi.status() == 0) {
    wifiStatus = "Idle";
  } else if (WiFi.status() == 1)  {
    wifiStatus = "SSID N/A";
  } else if (WiFi.status() == 2)  {
    wifiStatus = "Scan completed";
  } else if (WiFi.status() == 3)  {
    wifiStatus = "Connected";
  } else if (WiFi.status() == 4)  {
    wifiStatus = "Connect failed";
  } else if (WiFi.status() == 5)  {
    wifiStatus = "Conn. lost";
  } else if (WiFi.status() == 6)  {
    wifiStatus = "Disconnected";
  } else {
    wifiStatus = "Hakuna?";
  }

  //make MQTT status readable
  if (client.state() == 0) {
    mqttStatus = "Connected";
  } else if (client.state() == 1)  {
    mqttStatus = "Wrong MQTT protocol version";
  } else if (client.state() == 2)  {
    mqttStatus = "Bad client ID";
  } else if (client.state() == 3)  {
    mqttStatus = "Server cant accept conn.";
  } else if (client.state() == 4)  {
    mqttStatus = "User/pass rejected";
  } else if (client.state() == 5)  {
    mqttStatus = "Not authed";
  } else if (client.state() == -4)  {
    mqttStatus = "Server not responding";
  } else if (client.state() == -3)  {
    mqttStatus = "Connection broken";
  } else if (client.state() == -2)  {
    mqttStatus = "Network failed";
  } else if (client.state() == -1)  {
    mqttStatus = "Disconnected";
  } else {
    mqttStatus = "Hakuna?";
  }

  if (WiFi.status() != WL_CONNECTED) {
    wifiReconnect();
  }

  if (!client.connected()) {
    MQTTReconnect();
  }

  getDateTime();

  delay(500);
  ArduinoOTA.handle();

  long now = millis();

  if (ntpTimeSynced == false) {
    ntpSyncCheck();
  }

  //REED DOOR SWITCH
  if (doorState != lastDoorState ) {
    sprintf(topic, "home/%s/door", deviceId);
    //    if (doorState == true) {
    //      int doorStatus = 1
    //    } else {
    //      int doorStatus = 0
    //    }
    sprintf(payload, "{\"DOOR\":%d,\"Timestamp\":\"%s\"}", doorState, payloadNTPTime); // payloadTimestampNew //c virker lidt..
    //    //http://www.cplusplus.com/reference/cstdio/printf/
    client.publish(topic, payload);
    Serial.println(payload);
    lastDoorState = doorState;
  }

  //1 SECOND TIMER
  if (now - lastOLEDUpdate > 999) {
    lastOLEDUpdate = now;
    if (enableOLED == true) {
      if (OLEDLarge == true) {
        valuesToDisplayOledLarge();
      } else {
        valuesToDisplayOled();
      }
    } else {
      display.clearDisplay();
      display.display();
    }
    getWIFIInfo();
  }

  //3 SECOND TIMER
  if (now - lastThreeSecTimerRead > 900000) {
    lastThreeSecTimerRead = now;
    //client.loop(); //MQTT keep alive
  }
  
  //5 MINUTE TIMER
  if (now - lastFiveMinTimerRead > 300000) {
    lastFiveMinTimerRead = now;
  }

  //15 MINUTE TIMER
  if (now - lastFifteenMinTimerRead > 900000) {
    lastFifteenMinTimerRead = now;
    //client.loop(); //MQTT keep alive
  }

  //HOURLY TIMER
  if (now - lastStatusUpdate > 3600000) {
    lastStatusUpdate = now;
    sprintf(payloadReason, "Hourly");
    sendMQTTStatus();
  }
  
  //5 SECOND TIMER
  if (now - lastRead > 4999 || (sensorForceUpdate == true)) {
    client.loop(); //MQTT keep alive
    lastRead = now;

    readDHTBME280();

    if (sendStatusMQTT == true) {
      sprintf(payloadReason, "First");
      sendMQTTStatus();
      sendStatusMQTT = !sendStatusMQTT;
      sentMQTTMessages++;
    }
    
    if ((fabs(temp - lastSentTemp) > 0.15) || (fabs(hum - lastSentHum) > 0.25 ) || (firstMQTTMessageSent == false) || (sensorForceUpdate == true)) { //30000 for 30 sekunder //kun ved average -> and now > 30001
      sendSensorData();
      sentMQTTMessages++;
    }
      
    //ARRAY FOR LAST 10 DATA POSTS HANDLING HERE, SHIFTING DATA ONE POSITION
    for (int e = 0; e <= 8; e++) {
      if (dataArray[e + 1] != "") {
        dataArray[e] = dataArray[e + 1];
      }
    }
    dataArray[9] = payload;

    //ARRAY FOR LAST 10 DATA POSTS HANDLING HERE, SHIFTING DATA ONE POSITION ONLY IF BME SENSOR IS ONLINE!
    if ( bmeSensorOnline == true ) {
      for (int e = 0; e <= 8; e++) {
        if (dataArray[e + 1] != "") {
          dataArray[e] = dataArray[e + 1];
        }
      }
    }
    dataArray[9] = payloadBME;
    
    //|| (fabs(bmeTemp - lastSentBMETemp) > 0.15) || (fabs(bmeHum - lastSentBMEHum) > 0.25) || (fabs(bmePres - lastSentBMEPres) > 0.5)
    //CODE TO SEND MESSAGES ON EVERY CHANGE ((hum != lastSentHum) || (temp != lastSentTemp)) OR EVERY CHANGE LARGER THAN 0.1
    if (firstMQTTMessageSent == false) {
      firstMQTTMessageSent = !firstMQTTMessageSent;
    }
    //sendDHTBME280Data();
    readSendIRData();
  }


  server.handleClient();
  endTime = micros();
  loopTime = endTime - startTime;

  if ( loopTime < 500000 ) {
    if ( loopTime > longestLoopTime ) {
      longestLoopTime = loopTime;
    } else if ( loopTime < shortestLoopTime ) {
      shortestLoopTime = loopTime;
    }
    lastLoopTime = loopTime;
  }

  if ( loopTime < 500000 ) {
    if ( loopTime > longestLoopTime ) {
      longestLoopTime = loopTime;
    } else if ( loopTime < longestLoopTimeThisPeriod ) {
      longestLoopTimeThisPeriod = loopTime;
    }
    lastLoopTime = loopTime;
  }
}

void wifiReconnect() {
  WiFi.mode(WIFI_STA);
  WiFi.hostname(deviceId);
  //  WiFi.begin(ssid, password);

  display.clearDisplay();
  display.setCursor(0, 0);
  display.display();
  display.println("WIFI DOWN!  ");
  display.println(wifiStatus);
  display.println("Reconnecting in 5...");
  display.display();

  Serial.print("WiFi reconnecting... ");

  while (wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
    delay(500);
    Serial.print(".");
    display.print(".");
    display.display();
  }

  Serial.print(" reconnected to WiFi ");
  Serial.print(WiFi.localIP());
  Serial.println(" on ");
  Serial.print(WiFi.SSID());
  Serial.println(" OK! ");
  display.println("");
  display.println("RECONNECTED!");
  display.display();

  getWIFIInfo();

  wifiReconnects++;
  sprintf(payloadWIFIReconnects, "%ld", wifiReconnects);
  sprintf(payloadReason, "WIFIReconnect");
  addLogEntry("WIFI reconnect");
  sendMQTTStatus();
}

void MQTTReconnect() {
  while (!client.connected()) {
    Serial.print("Reconnecting to MQTT broker... ");
    if (client.connect(deviceId, MQTTUser, MQTTPass)) {
      Serial.println("connected............ OK!"); //debug code can be removed?
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" trying again in next loop");

      display.clearDisplay();
      display.setCursor(0, 0);
      display.display();
      display.println("MQTT DOWN!  ");
      display.println(mqttStatus);
      display.println("Reconnecting in next loop...");
      return;
    }
  }
  mqttReconnects++;
  sprintf(payloadMQTTReconnects, "%ld", mqttReconnects);
  sprintf(payloadReason, "MQTTReconnect");
  addLogEntry("MQTT reconnect");
  sendMQTTStatus();
}

void softReset() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.display();
  display.println("Resetting.  ");
  display.println("OLED...... ");
  display.println("Display...");
  display.display();
  delay(500);
}

void valuesToDisplayOled() {
  display.setTextSize(1);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Temperature :    "); //10 tegn maks ved x64 bredde
  display.println(payloadTemp);
  display.print("Humidity :       ");
  display.println(payloadHum);
  if ( bmeSensorOnline == true ) {
    display.print("Pressure :     ");
    display.println(payloadBMEPres);
  }
  display.print("WiFi :      ");
  display.println(wifiStatus);
  display.print("MQTT :      ");
  display.println(mqttStatus);
  display.print("T:");
  display.println(payloadNTPTime);
  display.print("IP :       ");
  display.println(payloadIP);
  display.display();
}

void valuesToDisplayOledLarge() {
  display.setTextSize(2);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Temp:"); //10 tegn maks ved x64 bredde
  display.println(payloadTemp);
  display.println("Humidity:");
  display.println(payloadHum);
  display.display();
}

void getDateTime() {
  timeClient.update();
  timeHour = hour();
  timeMinute = minute();
  timeSecond = second();
  timeDay = day();
  timeMonth = month();
  timeYear = year();
  sprintf(payloadNTPTime, "%i-%02d-%02d %02d:%02d:%02d", timeYear, timeMonth, timeDay, timeHour, timeMinute, timeSecond); //%i ,range
}

void getWIFIInfo() {
  //Serial.println(WiFi.BSSIDstr());
  String bssid = WiFi.BSSIDstr();
  bssid.toCharArray(payloadAPMAC, 50);
  
  IPAddress localAddr = WiFi.localIP();
  byte oct1 = localAddr[0];
  byte oct2 = localAddr[1];
  byte oct3 = localAddr[2];
  byte oct4 = localAddr[3];
  sprintf(payloadIP, "%d.%d.%d.%d", oct1, oct2, oct3, oct4);

  addressMacString = WiFi.macAddress();
  //sprintf(addressMac, "%s", addressMacString);

  sprintf(payloadRSSI, "%i", abs(WiFi.RSSI()));
  networkSSID = WiFi.SSID();
  networkSSID.toCharArray(payloadSSID, 50);
}

void sendMQTTStatus() { //Status MQTT (things that doesnt change often, like IP adress and counts)
  getWIFIInfo();
  unsigned long deviceUptime = millis();
  deviceUptime = deviceUptime / 3600000; //3600000 for hours!
  deviceUptime = deviceUptime + (rollOverCount * 1193);
  sprintf(payloadUptime, "%ld", deviceUptime);
  sprintf(topicStatus, "home/%s/status", deviceId); //%i ,range
  sprintf(payloadStatus, "{\"IP\":\"%s\",\"Version\":\"%s\",\"GotIP\":%ld,\"SSID\":\"%s\",\"MQTTReconnects\":\"%s\",\"WIFIReconnects\":\"%s\",\"Reason\":\"%s\",\"Timestamp\":\"%s\",\"Uptime\":\"%s\",\"APMac\":\"%s\",\"RSSI\":%s}", payloadIP, fwVersion, wifiGotIP, payloadSSID, payloadMQTTReconnects, payloadWIFIReconnects, payloadReason, payloadNTPTime, payloadUptime, payloadAPMAC, payloadRSSI);
  client.publish(topicStatus, payloadStatus);
  Serial.println(payloadStatus);

  //ARRAY FOR LAST 10 STATUS POSTS HANDLING HERE, SHIFTING DATA ONE POSITION
  for (int e = 0; e <= 8; e++) {
    if (statusArray[e + 1] != "") {
      statusArray[e] = statusArray[e + 1];
    }
  }
  statusArray[9] = payloadStatus;
}

void addLogEntry(String toLogArrayZero) { //, String toLogArrayOne //maybe for more information to the log file
  //ARRAY FOR LAST 10 STATUS POSTS HANDLING HERE, SHIFTING DATA ONE POSITION
  getDateTime();
  for (int e = 0; e <= 23; e++) {
    if (realLogArray[e + 1] != "") {
      realLogArray[e] = realLogArray[e + 1];
    }
  }
  toLogArrayZero.toCharArray(logInfo, 50);
  //payloadNTPTime.toCharArray(logTime, 50);
  sprintf(toLogArray, "[%s] - [%s]", logInfo, payloadNTPTime);

  realLogArray[24] = toLogArray;
}

void ntpSyncCheck() {
  if ((ntpTimeSynced == false) && (timeYear > 2015) && (timeYear > 2015)) {
    ntpTimeSynced = true;
    delay(500);
    syncTime = millis();
    sprintf(payloadReason, "Synced");
    addLogEntry("Synced");
    sendMQTTStatus();
  } else if (timeClient.getEpochTime() < 1000000) {
    ntpTimeSynced = false;
  }
}

void readSendIRData() {
  if (movementSensor.available()) {
    ir1 = movementSensor.getIR1();
    ir2 = movementSensor.getIR2();
    ir3 = movementSensor.getIR3();
    ir4 = movementSensor.getIR4();
    float tempC = movementSensor.getTemperature(); //getTemperatureF for Fahrenheit
    diff13 = ir1 - ir3;
    diff24 = ir2 - ir4;
    diff1324 = diff13 - diff24;
    movementSensor.refresh(); //Read dummy register after new data is read
    int irThreshold = 48;
    int irDiffThreshold = 64;
//(fabs(temp - lastSentTemp) > 0.15) || (fabs(hum - lastSentHum) > 0.25 )
    if ((abs(ir1 - ir1old) > irThreshold) || (abs(ir2 - ir2old) > irThreshold) || (abs(ir3 - ir3old) > irThreshold) || (abs(ir4 - ir4old) > irThreshold) || (abs(diff13 - diff13old) > irDiffThreshold) || (abs(diff24 - diff24old) > irDiffThreshold) || (abs(diff1324 - diff1324old) > irDiffThreshold)){
      sprintf(topicDetector, "home/%s/detector", deviceId); 
      // sprint function: f for float / c for char / s for string / i for integer / ld long / lu unsigned long//http://www.cplusplus.com/reference/cstdio/printf/
      sprintf(payloadDetector, "{\"IR1\":%i,\"IR2\":%i,\"IR3\":%i,\"IR4\":%i,\"DIFF13\":\"%i\",\"DIFF24\":%i,\"DIFF1324\":%i,\"TEMPERATURE\":%f,\"Timestamp\":\"%s\"}", ir1, ir2, ir3, ir4, diff13, diff24, diff1324, tempC, payloadNTPTime ); // payloadTimestampNew
      client.publish(topicDetector, payloadDetector);
      Serial.println(payloadDetector);
      ir1old = ir1;
      ir2old = ir2;
      ir3old = ir3;
      ir4old = ir4;
      diff13old = diff13;
      diff24old = diff24;
      diff1324old = diff1324;
    }
  }
}

void readDHTBME280() {
  temp = dht.getTemperature(); //degree celcius (float...)
  hum = dht.getHumidity(); //
  
  if (isnan(temp) || isnan(hum)) {
    temp = lastTemp;
    hum = lastHum;
  } else {
    lastTemp = temp;
    lastHum = hum;
  }

  dtostrf(temp, 4, 1, payloadTemp);
  dtostrf(hum, 4, 1, payloadHum);

  if ( bmeSensorOnline == true ) {
    bmeTemp = bme.readTemperature(); //degree celcius
    bmeHum = bme.readHumidity(); //humidity
    bmePres = bme.readPressure() / 100.0F; //hpa!
    bmeAlti = bme.readAltitude(SEALEVELPRESSURE_HPA);

    if (isnan(bmeTemp) || isnan(bmeHum) || isnan(bmePres) || isnan(bmeAlti)) {
      //vi tager sidste værdi og lægger ind i variablen..
      bmeHum = lastBMEHum;
      bmeTemp = lastBMETemp;
      bmePres = lastBMEPres;
      bmeAlti = lastBMEAlti;
    } else {
      lastBMETemp = bmeTemp;
      lastBMEHum = bmeHum;
      lastBMEPres = bmePres;
      lastBMEAlti = bmeAlti;
    }
    dtostrf(bmeTemp, 4, 1, payloadBMETemp);
    dtostrf(bmeHum, 4, 1, payloadBMEHum);
    dtostrf(bmePres, 4, 1, payloadBMEPres);
    dtostrf(bmeAlti, 4, 1, payloadBMEAlti);
  }
}

void sendSensorData() {
  //Serial.println(sensorForceUpdate);
  sensorForceUpdate == false;
  longestLoopTimeThisPeriod = 0; //reset af looptime measurement for every readout

  deltaTemp = temp - lastSentTemp;
  deltaHum = hum - lastSentHum;
  dtostrf(deltaTemp, 4, 1, payloadDeltaTemp);
  dtostrf(deltaHum, 4, 1, payloadDeltaHum);

  //Data MQTT (things that changes often like temperature, time and such...)
  sprintf(topic, "home/%s/sensors", deviceId); //%i ,range
  sprintf(payload, "{\"Temperature\":%s,\"Humidity\":%s,\"Deltatemperature\":%s,\"Deltahumidity\":%s,\"Timestamp\":\"%s\"}", payloadTemp, payloadHum, payloadDeltaTemp, payloadDeltaHum, payloadNTPTime ); // payloadTimestampNew
  client.publish(topic, payload);
  Serial.println(payload);
  
  //BME280!
  bmeDeltaTemp = bmeTemp - lastSentBMETemp;
  bmeDeltaHum = bmeHum - lastSentBMEHum;
  bmeDeltaPres = bmePres - lastSentBMEPres;
  dtostrf(bmeDeltaTemp, 4, 1, payloadDeltaBMETemp);
  dtostrf(bmeDeltaHum, 4, 1, payloadDeltaBMEHum);
  dtostrf(bmeDeltaPres, 4, 1, payloadDeltaBMEPres);

  lastSentHum = hum;
  lastSentTemp = temp;

  if ( bmeSensorOnline == true ) {
    sprintf(topicBME, "home/%s/sensors/bme", deviceId); //%i ,range
    sprintf(payloadBME, "{\"BMETemperature\":%s,\"Deltatemperature\":%s,\"BMEHumidity\":%s,\"Deltahumidity\":%s,\"BMEPressure\":%s,\"Deltapressure\":%s,\"BMEAltitude\":%s,\"Timestamp\":\"%s\"}", payloadBMETemp, payloadDeltaBMETemp, payloadBMEHum, payloadDeltaBMEHum, payloadBMEPres, payloadDeltaBMEPres, payloadBMEAlti, payloadNTPTime);
    // sprint function: f for float / c for char / s for string / i for integer / ld long / lu unsigned long//http://www.cplusplus.com/reference/cstdio/printf/
    client.publish(topicBME, payloadBME);
    Serial.println(payloadBME);

    lastSentBMETemp = bmeTemp;
    lastSentBMEHum = bmeHum;
    lastSentBMEPres = bmePres;
    lastSentBMEAlti = bmeAlti;
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
 
  if ((char)payload[0] == '1') {
   //Serial.println((char)payload[0]);
   sprintf(payloadReason, "Requested");
   addLogEntry("Requested");
   sendMQTTStatus();
   sensorForceUpdate == true;
   } else {
  //Serial.println((char)payload[0]);
  } 
}
