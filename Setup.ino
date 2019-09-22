/* SETUP SPLIT FROM MAIN FILE! CONTAINS ALL SETUP MODULES!

 */
void setup_wifi() {
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Preparing: "); //10 tegn maks ved x64 bredde, 20 tegn maks ved x128
  display.println("WIFI"); //10 tegn maks ved x64 bredde, 20 tegn maks ved x128
  display.display(); 
  
  Serial.println();
  Serial.print("Connecting to WiFi using ID: ");
  WiFi.hostname(deviceId);
  Serial.println(deviceId);
  
  WiFi.mode(WIFI_STA);
  //WiFi.begin(ssid, password);
  
  wifiMulti.addAP(ssidOne, passwordOne);   // add Wi-Fi networks you want to connect to
  wifiMulti.addAP(ssidTwo, passwordTwo);
  
  #ifdef BAD
    IPAddress ip(10, 0, 1, 54) ;
  #elif defined BRYGGERS
    IPAddress ip(10, 0, 1, 59) ;
  #elif defined GARAGE
    IPAddress ip(10, 0, 1, 55) ;
  #elif defined STUE
    IPAddress ip(10, 0, 1, 56) ;
  #elif defined MOBILE
    IPAddress ip(10, 0, 1, 57) ;
  #elif defined TOILET
    IPAddress ip(10, 0, 1, 58) ;
  #elif defined LOFT
    IPAddress ip(10, 0, 1, 59) ;
  #elif defined WEBTEST
    IPAddress ip(10, 0, 1, 234) ; //
  #endif

  //IPAddress gateway(192, 168, 1, 1);
  IPAddress gateway(10, 0, 1, 1);
  IPAddress subnet(255, 255, 255, 0);
  IPAddress dns1(8, 8, 8, 8);
  IPAddress dns2(8, 8, 4, 4);
  
  WiFi.config(ip, gateway, subnet, dns1, dns2);

  Serial.print("WiFi connecting... ");
////Multi WiFi connections
  while (wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
    delay(500);
    Serial.print('.');
  }
  //WiFi.setDNS(dns);
  Serial.print(" connected on IP adress ");
  Serial.print(WiFi.localIP());
  Serial.print(" on ");
  Serial.print(WiFi.SSID());
  Serial.println(" OK! ");

  getWIFIInfo();

  sprintf(payloadWIFIReconnects, "%ld", wifiReconnects);

}

void setup_ntp () {
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Preparing: "); //10 tegn maks ved x64 bredde, 20 tegn maks ved x128
  display.println("NTP time"); //10 tegn maks ved x64 bredde, 20 tegn maks ved x128
  display.display(); 
  timeClient.begin();
  setSyncProvider(&ntpSyncProvider);
  setSyncInterval(5);
  timeClient.update();
}

void setup_oled() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 64x48)
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Preparing: "); //10 tegn maks ved x64 bredde, 20 tegn maks ved x128
  display.println("Setup..."); //10 tegn maks ved x64 bredde, 20 tegn maks ved x128
  display.display(); 
}

void setup_mqtt() {
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Preparing: "); //10 tegn maks ved x64 bredde, 20 tegn maks ved x128
  display.println("MQTT"); //10 tegn maks ved x64 bredde, 20 tegn maks ved x128
  display.display(); 
    
  client.setServer(mqtt_server, 1883);
  
  //change to if....
  while (!client.connected()) {
    Serial.print("Setting up MQTT connection... ");
    if (client.connect(deviceId, MQTTUser, MQTTPass)) {
      Serial.println("connected............... OK!"); //debug code can be removed?
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");

      display.clearDisplay();
      display.setCursor(0,0);
      display.println("MQTT DOWN!  ");
      display.println(mqttStatus);
      display.println("Reconnecting in 5...");
      display.display(); 
      delay(5000);
      mqttReconnects++;
    }
  }
  sprintf(payloadMQTTReconnects, "%ld", mqttReconnects);
  client.setCallback(callback);
  client.subscribe("home/update");
}

void setup_webserver() {
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Preparing: "); //10 tegn maks ved x64 bredde, 20 tegn maks ved x128
  display.println("Web server"); //10 tegn maks ved x64 bredde, 20 tegn maks ved x128
  display.display(); 
  
  server.on ( "/", handleRoot );            // .../?parameter1=value1&parameter2=value2 køres i handleRoot (kan ændres hvis nødvendigt..
  server.on ( "/COMMAND", handleRoot );     // .../COMMAND?parameter1=value1&parameter2=value2 køres i handleRoot (kan ændres hvis nødvendigt..
  server.on ( "/RESET", handleRoot );       // .../RESET?parameter1=value1&parameter2=value2 køres i handleRoot (kan ændres hvis nødvendigt..
  server.on ( "/MQTTRESET", handleRoot );   // .../MQTTRESET?parameter1=value1&parameter2=value2 køres i handleRoot (kan ændres hvis nødvendigt..
  server.onNotFound(handleNotFound);        // if handle is not found!
  server.begin();
  Serial.println("Webserver started..................................... OK!"); 
}

void setup_sensors() {
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Preparing: "); //10 tegn maks ved x64 bredde, 20 tegn maks ved x128
  display.println("Sensors"); //10 tegn maks ved x64 bredde, 20 tegn maks ved x128
  display.display(); 
  
  //DHT22 sensor
  dht.setup(DHT22_PIN); // data pin 13

  //First readout!
  temp = dht.getTemperature();
  hum = dht.getHumidity();

  if(isnan(temp) || isnan(hum)) {
  //Setting up hum and tem as 
    lastSentTemp = 24;
    lastSentHum = 45;
    lastTemp = 24;
    lastHum = 45 ;
  } else {
    lastSentTemp = temp;
    lastSentHum = hum;
    lastTemp = temp;
    lastHum = hum ;    
  }
  
  //BME280 sensor
  //BME280 sensor
  bool status = bme.begin(0x76);
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    addLogEntry("BME not initialized"); 
    bmeSensorOnline = false;
    //while (1);
  } else {
    bmeSensorOnline = true;  
  }
  
  //First readout!
  if (bmeSensorOnline == true) {
    bmeTemp = bme.readTemperature(); //degree celcius
    bmeHum = bme.readHumidity(); //humidity
    bmePres = bme.readPressure() / 100.0F; //hpa!
    bmeAlti = bme.readAltitude(SEALEVELPRESSURE_HPA);
  }
  
  if(isnan(bmeTemp) || isnan(bmeHum) || isnan(bmePres) || isnan(bmeAlti)) {
  //Setting up hum and tem as 
    lastSentBMETemp = 24;
    lastSentBMEHum = 45;
    lastSentBMEPres = 1000;
    lastSentBMEAlti = 45 ;
  } else {
    lastSentBMETemp = bmeTemp;
    lastSentBMEHum = bmeHum;
    lastSentBMEPres = bmePres;
    lastSentBMEAlti = bmeAlti ;    
  }
}

//CHANGE FOR mDNS NAME RESOLUTION!?
void setup_ota() {
  ArduinoOTA.setHostname(deviceId);
  //ArduinoOTA.setPassword("sensor");
  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Preparing: "); //10 tegn maks ved x64 bredde, 20 tegn maks ved x128
  display.println("OTA"); //10 tegn maks ved x64 bredde, 20 tegn maks ved x128
  display.display(); 
}

void setup_arrays() {
  for (int e = 0; e <= 23; e++) { // 25 punkter i array
    realLogArray[e]= ""; 
  }

  for (int e = 0; e <= 8; e++) { // 10 punkter i array
    dataArray[e] = ""; 
  }

  for (int e = 0; e <= 8; e++) { // 10 punkter i array
    statusArray[e] = "";
  }
  
}
void setup_human_detection() {
  if (movementSensor.begin() == false)
  {
    Serial.println("Device not found. Check wiring.");
    //while (1);
  }
  display.println("Preparing: "); //10 tegn maks ved x64 bredde, 20 tegn maks ved x128
  display.println("HumanDetec"); //10 tegn maks ved x64 bredde, 20 tegn maks ved x128
  display.display(); 
}
