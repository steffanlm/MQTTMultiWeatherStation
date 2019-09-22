/* WEBPAGE SPLIT FROM MAIN FILE! CONTAINS WEBPAGE AND HANDLES!

 */

String webPage(){
  long uptime = millis();
  String page = "<html lang=da-DA><head><meta http-equiv='refresh' content='120'/>"; 

  page += "<style>";
  page = styles(page);
  page += "</style>";
  
  page += "<title> ";
  page += "Home sensors";
  page += "</title>";
  page += "</head><body><div id=\"main\"><h1>Weather station v";
  page += fwVersion;
  page += " located @ ";
  page += deviceId;
  page += "</h1>";

//BOTTOM MENU
//  page = bottomMenu(page);

//SCRIPTS!!!!
  page += "<script>";
//SCRIPT FOR SLIDER  
//  page += "var slider = document.getElementById(\"myRange\");";
//  page += "var output = document.getElementById(\"demo\");";
//  page += "output.innerHTML = slider.value;";
//  
//  page += "slider.oninput = function() {";
//  page += "  output.innerHTML = this.value;";
//  page += "}";

//SCRIPT FOR LEFT MENU
/* Set the width of the side navigation to 250px */
  page += "function openNav() {";
  page += "    document.getElementById(\"mySidenav\").style.width = \"250px\";";
  page += "    document.getElementById(\"main\").style.marginLeft = \"250px\";";
  page += "    document.body.style.backgroundColor = \"rgba(0,0,0,0.2)\";";
  page += "}";

/* Set the width of the side navigation to 0 */
  page += "function closeNav() {";
  page += "    document.getElementById(\"mySidenav\").style.width = \"0\";";
  page += "    document.getElementById(\"main\").style.marginLeft = \"0\";";
  page += "    document.body.style.backgroundColor = \"white\";";
  page += "}";
  
  page += "</script>";

//SIDE MENU
  page = leftMenu(page);
  
  page += "<table align=\"center\" border=>";
  
  page += "<tr>";
  page += "  <td>";
  page += "<h3>DHT22</h3> ";
  page += "  </td>";
//UPPER FIRST COLUMN  DHT22
  page += "  <td>";
  page += "<h3>Temperature :</h3> ";
  page += "  <div class=\"sc-gauge\">";
  page += "      <div class=\"sc-background\">";
  page += "        <div class=\"sc-percentage\"></div>";
  page += "        <div class=\"sc-mask\"></div>";
  page += "        <span class=\"sc-value\">";
  page +=           payloadTemp;  
  page += "        </span>";
  page += "      </div>";
  page += "      <span class=\"sc-min\">0 &#8451;</span>";
  page += "      <span class=\"sc-max\">35 &#8451;</span>";
  page += "  </div>";
  page += "  </td>";
//UPPER SECOND COLUMN  DHT22
  page += "  <td>";
  page += "<h3>Humidity :</h3> ";
  page += "  <div class=\"sc-gauge\">";
  page += "      <div class=\"sc-background\">";
  page += "        <div class=\"sc-percentageTwo\"></div>";
  page += "        <div class=\"sc-mask\"></div>";
  page += "        <span class=\"sc-value\">";
  page +=           payloadHum;  
  page += "        </span>";
  page += "      </div>";
  page += "      <span class=\"sc-min\">0 &#37</span>";
  page += "      <span class=\"sc-max\">100 &#37</span>";
  page += "  </div>";  
  page += "  </td>";   
//UPPER THIRD COLUMN  DHT22
  page += "  <td>";

  page += "<h3>Network power :</h3> ";
  page += "  <div class=\"sc-gauge\">";
  page += "      <div class=\"sc-background\">";
  page += "        <div class=\"sc-percentageThree\"></div>";
  page += "        <div class=\"sc-mask\"></div>";
  page += "        <span class=\"sc-value\">";
  page +=           payloadRSSI;  
  page += "        </span>";
  page += "      </div>";
  page += "      <span class=\"sc-min\">0 dB</span>";
  page += "      <span class=\"sc-max\">100 dB</span>";
  page += "  </div>";  
  page += "  </td>";
  
//UPPER FOURTH COLUMN
  page += "  <td>";

  page += "<h3>Load indication:</h3> ";

  page += "Last loop time: <b>";
  page += lastLoopTime;  
  page += " microseconds"; 
  page += "</b><BR>";

  page += "Shortest loop time : <b>";
  page += shortestLoopTime;  
  page += " microseconds";
  page += "</b><BR>";

  page += "Longest loop time this period : <b>";
  page += longestLoopTimeThisPeriod;   
  page += " microseconds";
  page += "</b><BR>";
  page += "Longest loop time since boot : <b>";
  page += longestLoopTime;   
  page += " microseconds";
  page += "</b><BR>";
  page += "<a href=\"?\"><button class=\"button\">Refresh</button></a>";
  
  page += "  </td>";
    
//UPPER FIFTH COLUMN
  page += "  <td>";

//  page += "  <h3>Custom Range Slider</h3>";
//  page += "<p>Drag the slider to display the current value.</p>";
//  
//  page += "<div class=\"slidecontainer\">";
//  page += "  <input type=\"range\" min=\"1\" max=\"100\" value=\"50\" class=\"slider\" id=\"myRange\">";
//  page += "  <p>Value: <span id=\"demo\"></span></p>";
//  page += "</div>";
//  
//  page += "";

//  page += "<h3>Network power :</h3> ";
//  page += "  <div class=\"sc-gauge\">";
//  page += "      <div class=\"sc-background\">";
//  page += "        <div class=\"sc-percentageFive\"></div>";
//  page += "        <div class=\"sc-mask\"></div>";
//  page += "        <span class=\"sc-value\">";
//  page +=           payloadRSSI;  
//  page += "        </span>";
//  page += "      </div>";
//  page += "      <span class=\"sc-min\">0 dB</span>";
//  page += "      <span class=\"sc-max\">100 dB</span>";
//  page += "  </div>";  
  page += "  </td>";
  
  page += "</tr>";

///////////////////////////////////////////////////////////////BME280\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

  page += "<tr>";
  page += "  <td>";
  page += "<h3>BME280</h3> ";
  page += "  </td>";
//UPPER FIRST COLUMN BME280  
  page += "  <td>";
  if ( bmeSensorOnline == true ) {
  page += "<h3>Temperature :</h3> ";
  page += "  <div class=\"sc-gauge\">";
  page += "      <div class=\"sc-background\">";
  page += "        <div class=\"sc-percentageFive\"></div>";
  page += "        <div class=\"sc-mask\"></div>";
  page += "        <span class=\"sc-value\">";
  page +=           payloadBMETemp;  
  page += "        </span>";
  page += "      </div>";
  page += "      <span class=\"sc-min\">0 &#8451;</span>";
  page += "      <span class=\"sc-max\">35 &#8451;</span>";
  page += "  </div>";
  } else {  
  page += "<h3>Temperature :</h3> ";
  page += "<b><font color=\"red\">Sensor not connected</font></b>"; 
  }
  page += "  </td>";
//UPPER SECOND COLUMN  BME280
  page += "  <td>";
  if ( bmeSensorOnline == true ) {
  page += "<h3>Humidity :</h3> ";
  page += "  <div class=\"sc-gauge\">";
  page += "      <div class=\"sc-background\">";
  page += "        <div class=\"sc-percentageSix\"></div>";
  page += "        <div class=\"sc-mask\"></div>";
  page += "        <span class=\"sc-value\">";
  page +=           payloadBMEHum;  
  page += "        </span>";
  page += "      </div>";
  page += "      <span class=\"sc-min\">0 &#37</span>";
  page += "      <span class=\"sc-max\">100 &#37</span>";
  page += "  </div>";  
  } else {  
  page += "<h3>Humidity :</h3> ";
  page += "<b><font color=\"red\">Sensor not connected</font></b>"; 
  }
  page += "  </td>";   
//UPPER THIRD COLUMN  BME280
  page += "  <td>";
  if ( bmeSensorOnline == true ) {
    page += "<h3>Pressure :</h3> ";
    page += "  <div class=\"sc-gauge\">";
    page += "      <div class=\"sc-background\">";
    page += "        <div class=\"sc-percentageSeven\"></div>";
    page += "        <div class=\"sc-mask\"></div>";
    page += "        <span class=\"sc-value\">";
    page +=           payloadBMEPres;  
    page += "        </span>";
    page += "      </div>";
    page += "      <span class=\"sc-min\">960 mbar</span>";
    page += "      <span class=\"sc-max\">1045 mbar</span>";
    page += "  </div>";  
  } else {  
    page += "<h3>Pressure :</h3> ";
    page += "<b><font color=\"red\">Sensor not connected</font></b>"; 
  }
  page += "  </td>";   
//UPPER FOURTH COLUMN  BME280
  page += "  <td>";
//  page += "<h3>Network power :</h3> ";
//  page += "  <div class=\"sc-gauge\">";
//  page += "      <div class=\"sc-background\">";
//  page += "        <div class=\"sc-percentageThree\"></div>";
//  page += "        <div class=\"sc-mask\"></div>";
//  page += "        <span class=\"sc-value\">";
//  page +=           payloadRSSI;  
//  page += "        </span>";
//  page += "      </div>";
//  page += "      <span class=\"sc-min\">0 dB</span>";
//  page += "      <span class=\"sc-max\">100 dB</span>";
//  page += "  </div>";  
  page += "  </td>";
//UPPER FIFTH COLUMN  BME280
  page += "  <td>";

//  page += "  <h3>Custom Range Slider</h3>";
//  page += "<p>Drag the slider to display the current value.</p>";
//  
//  page += "<div class=\"slidecontainer\">";
//  page += "  <input type=\"range\" min=\"1\" max=\"100\" value=\"50\" class=\"slider\" id=\"myRange\">";
//  page += "  <p>Value: <span id=\"demo\"></span></p>";
//  page += "</div>";
  
  page += "";

//  page += "<h3>Network power :</h3> ";
//  page += "  <div class=\"sc-gauge\">";
//  page += "      <div class=\"sc-background\">";
//  page += "        <div class=\"sc-percentageFive\"></div>";
//  page += "        <div class=\"sc-mask\"></div>";
//  page += "        <span class=\"sc-value\">";
//  page +=           payloadRSSI;  
//  page += "        </span>";
//  page += "      </div>";
//  page += "      <span class=\"sc-min\">0 dB</span>";
//  page += "      <span class=\"sc-max\">100 dB</span>";
//  page += "  </div>";  
  page += "  </td>";
  
  page += "</tr>";
  
///////////////////////////////////////////////////////////////BME280\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

  page += "<tr>";
  page += "  <td>";
  page += "<h3>Generic</h3> ";
  page += "  </td>";
//FIRST COLUMN THIRD ROW
  page += "  <td>";
  page += "<h3>Communication :</h3> ";
  page += "MQTT Server : <b>";
  page += mqtt_server;  
  page += "</b> connection <b>";  
  if (client.connected()){
    page += "<font color=\"green\">up</font></b>"; 
  } else {
    page += "<font color=\"red\">down</font></b>"; 
  }
  page += "<BR>";
  page += "</b> Client state is <b>"; 
  if (client.state() == 0){
     page += "<font color=\"green\">";
  } else {
    page += "<font color=\"red\">";
  }
  page += client.state();
  page += " ("; 
  page += mqttStatus;
  page += "</font> ) </b>";  
  page += "<BR>";
  page += " Reconnects: <b>";
  page += mqttReconnects; 
  page += " ";
  page += "</b><a href=\"?COMMAND=MQTTRESET\"><button class=\"button\">MQTT reconnect</button></a>";
  page += "<BR>";
  page += "Data messages sent since last reboot: <b>";
  page += sentMQTTMessages;  
//  page += " (";
//  if (sentMQTTMessage == 0) {
//    page+= "0";
//  } else  {
//    page += sentMQTTMessages / (uptime / 3600000);
//  }
//  page += ") pr. hour ";
  page += "</b> ";
  page += "<a href=\"?COMMAND=RESETMQTTCOUNTER\"><button class=\"button\">Reset counter</button></a>";    
  page += "<BR>";
  page += "  </td>";

//SECOND COLUMN THIRD ROW
  page += "  <td>";
  page += "<h3>Synchronization :</h3> ";
  page += "NTP epoch time: <b>";
  page += timeClient.getEpochTime();  
  page += "</b><BR>";
  page += " NTP status: <b>";
  if (timeClient.getEpochTime() > 1000000){
    page += "<font color=\"green\"> synced </font></b>"; 
  } else {
    page += "<font color=\"red\"> NOT synced </font></b>"; 
  }
  page += "<a href=\"?COMMAND=NTPTIMEUPDATE\"><button class=\"button\">Update clock</button></a><BR>";  
  page += "Time before sync: <b>";
  page += syncTime;
  page += " ms</b>";
  page += "  </td>";

//THIRD COLUMN THIRD ROW 
  page += "  <td>";
  page += "<h3>Network :</h3> ";
  page += "SSID : <b>";
  page += WiFi.SSID();  
  page += "</b><BR>";

  page += "Signal strength : <b>";
  page += WiFi.RSSI();  
  page += "</b><BR>";

  page += "WiFi status : <b>";
  page += wifiStatus; 
  page += "</b>";   
  page += "<BR> Reconnects: <b>";
  page += wifiReconnects; 
  page += "</b><BR>";
  
  page += "IP adress: <b>";
  page += payloadIP; 
  page += "</b><BR>";

  page += "GW adress: <b>";
  page += WiFi.gatewayIP().toString();
  page += "</b><BR>";  

//  page += "GW adress: <b>";
//  page += WiFi.gatewayIP();
//  page += "</b><BR>";  

  page += "MAC adress: <b>";
  page += addressMacString;  
  page += "</b><BR>";
  page += "  </td>";

//FOURTH COLUMN THIRD ROW  
  page += "  <td>";
  page += "<h3>Controls :</h3> ";
  page += " OLED status: <b>";
  if (enableOLED == true) {
    page += "<font color=\"green\">on </font></b>"; 
  } else {
    page += "<font color=\"red\">off </font></b>"; 
  }
  page += "</b>"; 
  page += "<a href=\"?COMMAND=TOGGLEOLED\"><button class=\"button\">Toggle OLED</button></a>";
  page += "<BR> ";
  page += "<BR> ";
  page += " OLED text size is <b>";
  if (OLEDLarge == true) {
    page += "<font color=\"purple\">large </font></b>"; 
  } else {
    page += "<font color=\"orange\">small </font></b>"; 
  }
  page += "</b>"; 
  page += "<a href=\"?COMMAND=TOGGLEOLEDTEXTSIZE\"><button class=\"button\">Toggle OLED text size</button></a>";
  page += "<BR> ";
  page += "<BR> ";
  page += "<a href=\"?COMMAND=CPURESTART\"><button class=\"button\">Restart CPU (soft reset!)</button></a>";
  page += "  ";
  page += "<a href=\"?COMMAND=CPURESET\"><button class=\"button\">Reset CPU (hard reset!)</button></a>";
  page += "  </td>";

//FIFTH COLUMN THIRD ROW
  page += "  <td>";

  page += "<h3>General information :</h3> ";
  
  page += "ESP chip ID : <b>";
  page += ESP.getChipId();  
  page += "</b><BR>";

  page += "ESP voltage : <b>";
  page += ESP.getVcc()/(float)1000;  
  page += "</b><BR>";

  page += "DeviceID : <b>";
  page += deviceId;  
  page += "</b><BR>";

  page += "Firmware version : <b>";
  page += fwVersion;  
  page += "</b><BR>";

  page += "Device uptime : <b>";
  page += uptime / 60000;  
  page += " minutes </b>";
  page += " <b>(";
  page += uptime / 3600000;  
  page += " hours)</b><BR>";

  page += "Device local time : <b>";
  page += payloadNTPTime;  
  page += "</b><BR>";
  page += "<a href=\"?\"><button class=\"button\">Refresh</button></a>";
  
  page += "  </td>";
  
  page += "</tr>";

//  page += "<tr>";
//
//  page += "  <td>";
//  page += "<a href=\"http://webtest.local\">Webtest</a>";
//  page += "  </td>";
//
//  page += "  <td>";
//  page += "<a href=\"http://garage.local\">Garage</a>";
//  page += "  </td>";
// 
//  page += "  <td>";
//  page += "<a href=\"http://stue.local\">Stue</a>";
//  page += "  </td>";
//  
//  page += "  <td>";
//  page += "<a href=\"http://bad.local\">Bad</a>";
//  page += "  </td>";
//
//  page += "  <td>";
//  page += "<a href=\"http://mobile.local\">Mobile</a>";
//  page += "  </td>";
//  
//  page += "</tr>";

  page += "</table>";

//  page += "<a href=\"?COMMAND=RESET\">Reset unit</a><br>";
//  page += "<a href=\"?COMMAND=RESET\"><button>Reset</button></a>"; 

  page += "</div></body></html>";
  return page;
}
