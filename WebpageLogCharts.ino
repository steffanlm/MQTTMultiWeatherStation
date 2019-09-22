String webPageLog(){
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


//SIDE MENU
  page = leftMenu(page);

  page += "<script>";
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
  
//BOTTOM MENU
//  page = bottomMenu(page);

  page += "<table align=\"center\" border=>";
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
//  page += "  </td>";
//  page += "</tr>";
  page += "<tr>";

//THE WIDE FIVE COLUMN AREA  
  page += "  <td colspan=\"5\">";
  page += "<h3>Log :</h3> ";
  page += "Data log @ topic: <b>";
  page += topic;    
  page += "</b>";
  page += " ";
  page += "<a href=\"?COMMAND=SENDMQTTDATA\"><button class=\"button\">Force new data message</button></a>";  
  page += " ";
  page += "Messages sent since last reboot: <b>";
  page += sentMQTTMessages;  
  page += "</b> ";
  page += "<a href=\"?COMMAND=RESETMQTTCOUNTER\"><button class=\"button\">Reset counter</button></a>";    
  page += "<BR>";
  
  page += "<textarea readonly name=\"Text1\" cols=\"180\" rows=\"10\">";  //style=\"overflow:hidden\"
  for (int e = 0; e <= 9; e++) {
      page += e+1;
      page += ": "; 
      page += dataArray[9-e];   
      if (e != 9) {
        page += "&#13;&#10;";
      }
  }
  page += "</textarea>";
  page += "<BR>";

  page += "<BR>";

  page += "Status log @ topic: <b>";
  page += topicStatus;    
  page += "</b>";
  page += " ";
  page += "<a href=\"?COMMAND=SENDMQTTSTATUS\"><button class=\"button\">Force new status message</button></a>";  
  page += "</b><BR>";     
  page += "<textarea readonly name=\"Text1\" cols=\"180\" rows=\"10\">"; //style=\"overflow:hidden\"
  for (int e = 0; e <= 9; e++) {
      page += e+1;
      page += ": "; 
      page += statusArray[9-e];   
      if (e != 9) {
        page += "&#13;&#10;";
      }
  }
  page += "</textarea>";
  page += "<BR>";  
  page += "<BR>";  
//TEST FOR REALLOG
  page += "Internal log @ array: <b><BR>";
  page += "<textarea readonly name=\"Text1\" cols=\"180\" rows=\"7\">";
  for (int e = 0; e <= 24; e++) {
      page += e+1;
      page += ": "; 
      page += realLogArray[24-e];   
      
      if (e != 24) {
        page += "&#13;&#10;";
      }
  }
  page += "</textarea>";

  page += "<BR>";  
  page += "  </td>";

  page += "</tr>";
  page += "</table>";


  page += "</div></body></html>";
  return page;
}

String webPageCharts(){
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

  page += "<script>";
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
  
  page += "  <td>";//colspan=\"5\"
  page += "<h3>Charts :</h3> ";
  page += " ";
  
//  page += "<textarea readonly name=\"DHTTemperature\" cols=\"10\" rows=\"40\">";  //style=\"overflow:hidden\"
//  for (int e = 0; e <= 49; e++) {
//      page += e+1;
//      page += ": "; 
//      page += sensorPressureArray[49-e];   
//      if (e != 49) {
//        page += "&#13;&#10;";
//      }
//  }
//
//  page += "</textarea>";
//  page += "  </td>";
//  page += "  <td>";
//
//  page += "</textarea>";
//  page += "<textarea readonly name=\"BMEPressure\" cols=\"10\" rows=\"40\">";  //style=\"overflow:hidden\"
//  for (int e = 0; e <= 49; e++) {
//      page += e+1;
//      page += ": "; 
//      page += sensorPressureArray[49-e];   
//      if (e != 49) {
//        page += "&#13;&#10;";
//      }
//  }
//  page += "</textarea>";
  page += "  </td>";
    
  page += "</tr>";
  page += "</table>";


  page += "</div></body></html>";
  return page;
}
