 //Handle 
void handleRoot(){ //leder efter arguments, fx parameter1: /path?parameter1=value1&parameter2=value2 (kunne det blot være "COMMAND"?
  if ( server.hasArg("ORDER") || server.hasArg("MQTTRESET") || server.hasArg("COMMAND") || server.hasArg("CPURESTART") || server.hasArg("CPURESET") || server.hasArg("SENDMQTTSTATUS") || server.hasArg("SENDMQTTDATA") || server.hasArg("TOGGLEOLED") || server.hasArg("TOGGLEOLEDTEXTSIZE")) { 
    handleCommand();
  } else if (server.hasArg("PAGE")) { 
    handlePage();
  } else {
    server.send ( 200, "text/html", webPage() );
  }  
}

void handlePage() {
  String page;
  page = server.arg("PAGE");  //spørger ind til specifikt argument, her command: /?PAGE=LOG etc
  
  if ( page == "LOG" ) {
    //Serial.println("Page");
    //server.sendHeader("Location", String("/"), true);
    //server.send(303);   
    server.send ( 200, "text/html", webPageLog() );
  } else if ( page == "CHARTS" ) {
    server.send ( 200, "text/html", webPageCharts() );
  } else if ( page == "FRONTEND" ) {
    server.send ( 200, "text/html", webPage() );
  } else {
    Serial.println("A page not yet defined...");
    addLogEntry("Page not yet defined"); 
  } 
}

void handleCommand() {
  String command;
  command = server.arg("COMMAND");  //spørger ind til specifikt argument, her command: /?COMMAND=RESET
  
  if ( command == "RESET" ) {
    Serial.println("Button Reset pressed");
    softReset();
    server.sendHeader("Location", String("/"), true);
    server.send(303);   
    
  } else if ( command == "MQTTRESET" ) {
    Serial.println("Button MQTT reconnect pressed");
    addLogEntry("Button MQTT reconnect pressed"); 
    MQTTReconnect();
    server.sendHeader("Location", String("/"), true);   
    server.send(303);   
    
  } else if ( command == "RESETMQTTCOUNTER" ) {
    Serial.println("Button Reset MQTT counter pressed");
    sentMQTTMessages = 0;
    addLogEntry("Button Reset MQTT counter pressed"); 
    //server.send ( 200, "text/html", webPage() );
    server.sendHeader("Location", String("/"), true);   
    server.send(303);   
    
  } else if ( command == "NTPTIMEUPDATE" ) {
    Serial.println("Button NTP time update pressed");
    //setup_ntp();
    timeClient.forceUpdate();
    addLogEntry("Button Update clock pressed"); 
    //server.send ( 200, "text/html", webPage() );
    server.sendHeader("Location", String("/"), true);
    server.send(303);   
    
  } else if ( command == "CPURESTART" ) {
    Serial.println("Button Restart CPU pressed");
    server.sendHeader("Location", String("/"), true);
    server.send(303);  
    delay(2000);
    ESP.restart();  
    
  } else if ( command == "CPURESET" ) {
    Serial.println("Button Reset CPU pressed");
    server.sendHeader("Location", String("/"), true);
    server.send(303);  
    delay(2000);
    ESP.reset();
      
  } else if ( command == "SENDMQTTSTATUS" ) {
    Serial.println("Button Send MQTT status pressed");
    addLogEntry("Button Send MQTT status pressed"); 
    server.sendHeader("Location", String("/"), true);
    sprintf(payloadReason, "Button");
    sendMQTTStatus();
    server.send(303);  
     
  } else if ( command == "SENDMQTTDATA" ) {
    Serial.println("Button Send MQTT data pressed");
    firstMQTTMessageSent = false;
    addLogEntry("Button Send MQTT data pressed"); 
    server.sendHeader("Location", String("/"), true);
    server.send(303);  

  } else if ( command == "TOGGLEOLED" ) { 
    Serial.println("Button Toggle OLED pressed");
    enableOLED = !enableOLED;
    if (enableOLED == true) {
      addLogEntry("Button Toggle OLED on pressed"); 
    } else {
      addLogEntry("Button Toggle OLED off pressed"); 
    }
    server.sendHeader("Location", String("/"), true);
    server.send(303);    

  } else if ( command == "TOGGLEOLEDTEXTSIZE" ) { 
    Serial.println("Button Toggle OLED text size pressed");
    OLEDLarge = !OLEDLarge;
    if (OLEDLarge == true) {
      addLogEntry("Button Toggle OLED text size large pressed"); 
    } else {
      addLogEntry("Button Toggle OLED text size small pressed");  
    }  
    server.sendHeader("Location", String("/"), true);
    server.send(303);  
      
  } else {
    Serial.println("A function not yet defined...");
  } 
}

void handleNotFound(){
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}

void redirect() {
  server.sendHeader("Location", String("/"), true);
  server.send ( 302, "text/plain", "");
}

String leftMenu(String page) {

  page += "<div id=\"mySidenav\" class=\"sidenav\">";
  page += "  <a href=\"javascript:void(0)\" class=\"closebtn\" onclick=\"closeNav()\">&times;</a>";
  page += "  <a href=\"?PAGE=FRONTEND\">Frontend</a>";
  page += "  <a href=\"?PAGE=LOG\">Logs</a>";
  page += "  <a href=\"?PAGE=CHARTS\">Charts</a>";
  page += "  <a href=\"?PAGE=SETUP\">Setup</a>";
  page += "<br>";
  page +=   "<a class=\"active\" href=\"http://webtest.local\">Webtest</a>";
  page +=   "<a href=\"http://garage.local\">Garage</a>";
  page +=   "<a href=\"http://stue.local\">Stue</a>";
  page +=   "<a href=\"http://bad.local\">Bad</a>";
  page +=   "<a href=\"http://mobile.local\">Mobile</a>";
  page +=   "<a href=\"http://bryggers.local\">Bryggers</a>";
  page +=   "<a href=\"http://toilet.local\">Toilet</a>";
  page +=   "<a href=\"http://loft.local\">Loft</a>";
  page +=   "<a href=\"\"><-Sensors</a>";
  page += "</div>";
    
  page += "<span style=\"font-size:30px;cursor:pointer\" onclick=\"openNav()\">&#9776;</span>";
  return page;
}

String bottomMenu(String page) {

  page += "<ul>";
  page +=   "<li><a class=\"active\" href=\"http://webtest.local\">Webtest</a></li>";
  page +=   "<li><a href=\"http://garage.local\">Garage</a></li>";
  page +=   "<li><a href=\"http://stue.local\">Stue</a></li>";
  page +=   "<li><a href=\"http://bad.local\">Bad</a></li>";
  page +=   "<li><a href=\"http://mobile.local\">Mobile</a></li>";
  page +=   "<li><a href=\"http://bryggers.local\">Bryggers</a></li>";
  page +=   "<li><a href=\"http://toilet.local\">Toilet</a></li>";
  page +=   "<li><a href=\"http://loft.local\">Loft</a></li>";
  page +=   "<li><a href=\"\"><-Sensors</a></li>";
  page += "</ul>";
  return page;
}

String styles(String page) {
  page += "body { margin:0; background-color: #fffff; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }";
  page += "table {";
  page += "    width:100%;";
  page += "}";
  page += "table, th, td {";
  page += "    border: 1px solid black;";
  page += "    border-collapse: collapse;";
  page += "}";
  page += "th, td {";
  page += "    padding: 5px;";
  page += "    text-align: left;";
  page += "    vertical-align: top;";
  page += "}";
  page += "td:hover {background-color: #eee5de;}";
  page += ".button {";
  page += "    -webkit-transition-duration: 0.4s; /* Safari */";
  page += "    transition-duration: 0.4s;";
  page += "}";
  page += "";
  page += ".button:hover {";
  page += "    background-color: #555555; /* GREY */";
  page += "    color: white;";
  page += "}";
  page += "textarea{";
  page += "width: 100%;";
//  page += "height:100px;";
  page += "border:1px solid #000000;";
  page += "background-color:#fffff;";
  page += "border-radius: 8px;";
  page += "padding: 4px 8px;";
  page += "box-sizing: border-box;";
  page += "resize: none;";
  page += "}";

//CSS der tilhører gauges  
  page += "code { display:inline-block; max-width:600px;  padding:10px 0 0; line-height:1.5; font-family:monospace; color:#ccc }"; //padding = 80 normalt
  page += ".sc-gauge  { width:200px; height:10px; margin:10px auto; }"; //height = 200 normaly
  page += ".sc-background { position:relative; height:100px; margin-bottom:10px; background-color:#fff; border-radius:150px 150px 0 0; overflow:hidden; text-align:center; }";
  page += ".sc-mask { position:absolute; top:20px; right:20px; left:20px; height:80px; background-color:#eee5de; border-radius:150px 150px 0 0 }"; //color for hiding colorangle

  page += ".sc-percentage { position:absolute; top:100px; left:-200%; width:400%; height:400%; margin-left:100px; background-color:#ff0000 ; }";
  float degree = 0;
  float upper = 35;
  float lower = 0;
  degree = temp / (upper - lower) * 180;
  page += ".sc-percentage { transform:rotate(";
  page += degree;
  page += "deg); transform-origin:top center; }";

  page += ".sc-percentageTwo { position:absolute; top:100px; left:-200%; width:400%; height:400%; margin-left:100px; background-color:#00aeef; }";
  upper = 100;
  lower = 0;
  degree = hum / (upper - lower) * 180;
  page += ".sc-percentageTwo { transform:rotate(";
  page += degree;
  page += "deg); transform-origin:top center; }";

  page += ".sc-percentageThree { position:absolute; top:100px; left:-200%; width:400%; height:400%; margin-left:100px; background-color:#008000; }";
  upper = 100;
  lower = 0;
  degree = abs((float) WiFi.RSSI()) / (upper - lower) * 180; //https://www.arduino.cc/en/Reference/Cast
  page += ".sc-percentageThree { transform:rotate(";
  page += degree;
  page += "deg); transform-origin:top center; }";

//FOUR mangler!!!

  page += ".sc-percentageFive { position:absolute; top:100px; left:-200%; width:400%; height:400%; margin-left:100px; background-color:#ff0000; }";
  upper = 35;
  lower = 0;
  degree = bmeTemp / (upper - lower) * 180; //https://www.arduino.cc/en/Reference/Cast
  page += ".sc-percentageFive { transform:rotate(";
  page += degree;
  page += "deg); transform-origin:top center; }";
  page += ".sc-min { float:left; }";
  page += ".sc-max { float:right; }";
  page += ".sc-value { position:absolute; top:50%; left:0; width:100%;  font-size:48px; font-weight:70 }";

  page += ".sc-percentageSix { position:absolute; top:100px; left:-200%; width:400%; height:400%; margin-left:100px; background-color:#00aeef; }";
  upper = 100;
  lower = 0;
  degree = bmeHum / (upper - lower) * 180; //https://www.arduino.cc/en/Reference/Cast
  page += ".sc-percentageSix { transform:rotate(";
  page += degree;
  page += "deg); transform-origin:top center; }";
  page += ".sc-min { float:left; }";
  page += ".sc-max { float:right; }";
  page += ".sc-value { position:absolute; top:50%; left:0; width:100%;  font-size:48px; font-weight:70 }";

  page += ".sc-percentageSeven { position:absolute; top:100px; left:-200%; width:400%; height:400%; margin-left:100px; background-color:#800080; }";
  upper = 1045;
  lower = 960;
  degree = (bmePres - lower) / (upper - lower) * 180; //https://www.arduino.cc/en/Reference/Cast //(upper - bmePres) could be new code!
  page += ".sc-percentageSeven { transform:rotate(";
  page += degree;
  page += "deg); transform-origin:top center; }";
  page += ".sc-min { float:left; }";
  page += ".sc-max { float:right; }";
  page += ".sc-value { position:absolute; top:50%; left:0; width:100%;  font-size:48px; font-weight:70 }";

//CSS der tilhører slideren
  page += ".slidecontainer {";
  page += "    width: 100%;";
  page += "}";

  page += ".slider {";
  page += "    -webkit-appearance: none;";
  page += "    width: 100%;";
  page += "    height: 25px;";
  page += "    background: #d3d3d3;";
  page += "    outline: none;";
  page += "    opacity: 0.7;";
  page += "    -webkit-transition: .2s;";
  page += "    transition: opacity .2s;";
  page += "}";

  page += ".slider:hover {";
  page += "    opacity: 1;";
  page += "}";

  page += ".slider::-webkit-slider-thumb {";
  page += "    -webkit-appearance: none;";
  page += "    appearance: none;";
  page += "    width: 25px;";
  page += "    height: 25px;";
  page += "    background: #4CAF50;";
  page += "    cursor: pointer;";
  page += "}";

  page += ".slider::-moz-range-thumb {";
  page += "    width: 25px;";
  page += "    height: 25px;";
  page += "    background: #4CAF50;";
  page += "    cursor: pointer;";
  page += "}";

//CSS der tilhører menuen i bunden
  page += "ul {";
  page += "    list-style-type: none;";
  page += "    border: 1px solid #e7e7e7;";
  page += "    background-color: #f3f3f3;";
  page += "    margin: 0;";
  page += "    margin: 0;";
  page += "    padding: 0;";
  page += "    overflow: hidden;";
  page += "    background-color: #333;";
  page += "    position: fixed;";
  page += "    bottom: 0;";
  page += "    width: 100%;";
  page += "}";

  page += "li {";
  page += "    float: left;";
  page += "}";

  page += "li a {";
  page += "    display: block;";
  page += "    color: white;";
  page += "    text-align: center;";
  page += "    padding: 10px 12px;";
  page += "    text-decoration: none;";
  page += "    border-right: 1px solid #bbb;";
  page += "}";

  page += "li a:hover:not(.active) {";
  page += "    background-color: #111;";
  page += "}";

  page += "li:last-child {";
  page += "    border-right: none;";
  page += "}";

  page += ".active {";
  page += "    background-color: #4CAF50;";
  page += "}";

//CSS der tilhører menuen i siden
  page += ".sidenav {";
  page += "    height: 100%; ";/* 100% Full-height */
  page += "    width: 0;"; /* 0 width - change this with JavaScript */
  page += "    position: fixed;"; /* Stay in place */
  page += "    z-index: 1;";/* Stay on top */
  page += "    top: 0; ";/* Stay at the top */
  page += "    left: 0;";
  page += "    background-color: #111;"; /* Black*/
  page += "    overflow-x: hidden; ";/* Disable horizontal scroll */
  page += "    padding-top: 60px;"; /* Place content 60px from the top */
  page += "    transition: 0.5s;"; /* 0.5 second transition effect to slide in the sidenav */
  page += "}";

  page += ".sidenav a {";
  page += "    padding: 8px 8px 8px 32px;";
  page += "    text-decoration: none;";
  page += "    font-size: 25px;";
  page += "    color: #818181;";
  page += "    display: block;";
  page += "    transition: 0.3s;";
  page += "}";

  page += ".sidenav a:hover {";
  page += "    color: #f1f1f1;";
  page += "}";

  page += ".sidenav .closebtn {";
  page += "    position: absolute;";
  page += "    top: 0;";
  page += "    right: 25px;";
  page += "    font-size: 36px;";
  page += "    margin-left: 50px;";
  page += "}";

// //Style page content - use this if you want to push the page content to the right when you open the side navigation */
//  page += "#main {";
//  page += "    transition: margin-left .5s;";
//  page += "    padding: 20px;";
//  page += "}";

  page += "@media screen and (max-height: 450px) {";
  page += "    .sidenav {padding-top: 15px;}";
  page += "    .sidenav a {font-size: 18px;}";
  page += "}";
  
  return page;
}
