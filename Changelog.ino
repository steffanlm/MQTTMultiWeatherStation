/* COMPILE AS:
WEMOS D1 R2 & mini
Rest is default!

Changelog: Code running on ESP8266 Wemos D1 Mini. Added(new), Changed(function), Corrected/Fixed(bugs)
to do: 
 - #4 add websocket for fun?
 - #13 add slider to change deltage change needed or to any other variables https://www.w3schools.com/howto/tryit.asp?filename=tryhow_css_js_rangeslider 
 - #14 add snackbar to show latest updates: https://www.w3schools.com/howto/howto_js_snackbar.asp
 - #15 add something parallax? https://www.w3schools.com/howto/howto_css_parallax.asp
 - #16 add something switch https://www.w3schools.com/howto/howto_css_switch.asp
 - #17 add something scroll indicator https://www.w3schools.com/howto/howto_js_scroll_indicator.asp
 - #18 add something sort by table https://www.w3schools.com/howto/howto_js_sort_table.asp
 - #19 add something filter dropdown https://www.w3schools.com/howto/howto_js_filter_dropdown.asp
 - #20 add something timeline https://www.w3schools.com/howto/tryit.asp?filename=tryhow_css_timeline
 - #21 add something transparent text https://www.w3schools.com/howto/howto_css_image_transparent.asp
 - #24 add google charts to something? https://www.w3schools.com/howto/howto_google_charts.asp
 - #27 add light value somehow
 - #29 add REED sensor to input pin and send status via MQTT and show in WEBPAGE
 - #30 add k-type thermocoupler MAX31855 to I2C og SPI and send temperatures as sensor #1 and #2 on MQTT
 - #31 change code so BME and other sensors only send out information on topics regarding own changes and not for all sensors (just like implementation of MAX31855)
 - #32 change topics to be declared once, and not in every loop

v2.5.8: (in progress - change last digit on every load)
 - Added AK9753 sensor to the setup, which is available only in bathroom
 - Added IR1-4, and DIFF13 and DIFF24 to MQTT message, sent out every 5 second.
 - Added temperature readout from AK sensor to MQTT message
 - Fixed #34 change code so AK sensor is not blocking code execution when not present
 - Fixed #33 change code to read and send sensordata in own void, like the AK sensor (do for BME280 and DHT22)
 - Fixed MQTT count since last reboot, as it stated every loop and not messages sent.
 - Added several timers, 3 sec, 5 min and 15 min timers. 
 - Moved MQTT loop to keep connection from 5 sec to 5 min timer.
 - Added "Loft" as new sensor to support temperature and humidity readings from "Loft", with IP address .59
 - Added diff1324 to show difference of the two differences of the IR sensors
 - Changed send behaviour for IR data, to only send on changes, and not every 5 sec, as it fills up the databases
 - Removed OTA password (commented out)
 .6 - Fixed MQTT disconnects due to wrong timing. Moved from 5 minute timer to 5 second timer. Should fix the random reboots on the sensors.
 .7 - Moved RSSI from "sensors" to "status" MQTT topic
 .8 - not loaded yet...
    - Changed small letters to capital letters for reconnect state on webpage.
 
v2.4:
 - Added APMac to show which AP sensor is connected
 - Added MQTT Callback to send status by request from HASS

v2.3:
 - Added pseudo code to the MAX31855 sensor, so not active for the time beging
 - Added reed switch as a "door alert" to D4 #29 
 - Added uptime on MQTT with "payloadUptime" to status MQTT message
 - Added "toilet" and "bryggers" as sensors for bathroom
 - mDNS up and running for all sensors #10
 
v2.2:  
 - Added HTTP post to MQTT stream, to add data directly to database via datacatch.asp!
 - Fixed mDNS to work properly with host! (failed due to OTA naming the mDNS on begin)
 - Removed bottom menu
 - Added direct links in left menu
 - Removed array with sensordata
 - Added calculations for looptime, longest and shortest (no average is calculated)
 - Changed looptime calculations from millis to micros
 
v2.1:
 - Added BME280 sensor to most of the code (MQTT and setup code - not put into webinterface) #1 & #2
 - Added BME280 sensor online bool to skip code execution when sensor is not found
 - Added function to remove gauges for pressure when sensor is not online
 - Added pressure to OLED display, when active
 - Changed barometric scale to fit most common values from DMI
 - Added BME280 payloads to data log 
 - Added logfiles to separate textboxes (#25)
 - Added interrupt for button pres to D5 / pin 14
 - Added log, screensize change etc. for the newly added button
 - Added BME280 sensordata to trigger sending data on MQTT 
 - Changed from math to fabs() to trigger sending dataon MQTT from both DHT22 and BME280 sensors (simplifying code)
 - Added delta measure from all BME sensors
 - Added array with the last 100 sent values from the pressure sensor
 - Added CHARTS page, with array printing for the time being
 - Changed deltahum from 0.15 to 0.25 to reduce packages sent!
 - Changed sensor readout from 3 to 5 seconds
 - Changed RSSI in JSON string from string to integer, to be able to show charts in HASS
 - Fixed sending out MQTT messages from both sensors on same topic, which caused HASS to neglect the data
 - Fixed sending out MQTT message when BME sensor is offline - code jibberish and a few if sentences were wrong - corrected now :D
 - Added button to turn on OLED on/off added to case and to code
 - Removed change of large/small text on button pres (reused button to turn off OLED screen)
 - Changed realLogArray size from 50 to 25 due to seemingly poor performance
 - Changed sensorPressureArray from 100 to 50 due to seemingly poor performance
v2.0: 
 - Added DNS address to setup of WiFI, which should make NTP calls ok (#6)
 - Added syncTime variable, to show the time before NTP synchronization has taken place
 - Fixed void getDateTime() to return what expected, as timeClient.update(); is now called within the function, to ensure correct timestamp
 - Fixed gateway address on webpage, to show actual gateway
 - Fixed getVCC to show correct voltage
 - Changed ntpSync to give correct indication of sync and sending out new MQTT status message
 - Changed all the timebased loops, to work with >999 instead of >1000 to hit 1sec mark
 - Added "reason" to every status MQTT message
 - Added hard links to all sensors, hitting mDNS names (link and mDNS needs to work) (#9)
-2.0.1
 - Changed temperature maximum value from 30 to 35 degrees
 - Fixed "reason" to always be right, as it now calls function instead of setting variable (wasnt fixed correctly before)
 - Added real data to log file
 - Fixed sending real IP address to status when IP changed
 - Changed uptime calculation to reflect the real uptime, based on a single calculation
 - Changed http-equiv='refresh' content='300' from 10 seconds to 120 seconds
 - Fixed links to all the sensors to be correct
 - Fixed reason message for MQTT status to be correct
 - Fixed internal log array to print out correct messages with correct timestamp (merged from 2 columns to just 1 column with sprintf) (#11)
 - Changed Toggle OLED on/off and textsize small/large buttons to reflect the real change in internal log
-2.0.2
 - Changed internal log, so datetime are added consequently in the void and not "from last request.."
 - Corrected "Update clock" button to reflect "Update clock" and not "Button toggle OLED pressed"
 - Added CSS slider to make some fun
 - Added menu at bottom to change between different stations
 - Corrected some reasons for sending out new status messages
 - Added Log as separate page
 - Removed Log from frontpage
v1.9: 
 - Added control to change OLED text size
 - Moved deviceID to header
 - Added Pressure gauge to webpage (ready for BME280 to come)
 - Added Network Power gauge to webpage
 - Cleaned up information and restructured webpage a bit
 - Changed all gauges to have different colors
 - Fixed overflow in log textarea
 - Changed array size of status log from 5 to 10 entities
 - Changed array print direction to webpage, so newest message is always at top
 - Changed force new status message to call function instead of setting bool value
 - Added ntpSyncCheck void to send out fresh MQTT messages when time has been set
 - Fixed temperaturegauge scle from 0-30 and not 15-30 as printed on page
v1.8: 
 - Changed MQTT from just "sensors" to "data" and "status"
 - Added sendStatusMQTT = true; bool value to define if a new status message should be sent out. Triggers on wifi eventhandlers and on MQTT and WIFI reconnect modules
 - Added OTA first try
 - Added MAC address to webpage
 - Reverted from singleWIFI to multiWIFI
 - Fixed sendStatusMQTT code, so should work now with sending out new STATUS message
 - Changed OLED update from every 3 second cycle to own 1 second cycle along with fetching time and updating WIFI info
 - Changed MQTT status update to every hour, instead of being condition based, which will update the actual IP address
 - Added Timestamp to status MQTT message
 - Added buttons to send new MQTT status and data messages
 - Added toggle OLED button and OLED status to web interface
 - Added array to print out last 10 mqtt data messages on webpage in ascending order
 - Changed MQTT reconnect function to enter "return" when connection is not established, to continue rest of the code
 - Changed webinterface to be more usefull and hot :)
 - Added array to print out last 5 mqtt status message on webpage in asending order
 - Added gauge to temperature
 - Added gauge to humidity
 - Added MOBILE into define
 - Added "data only"  to OLED with larger text
v1.7:
 - Added MQTT and WiFi reconnect counter to webpage
 - Added "real" IP and not only first setup IP to the webpage
 - Added IP address to OLED and to MQTT topic
 - Changed ntptime to show yyyy-mm-dd hh:mm:ss on OLED, webpage and in MQTT messages
 - Added RSSI to MQTT as absolute value
 - Changed delay from WiFi connection is made till MQTT connections is established by to 5 seconds from 1 (might be possible to have less delay!)
 - Cleaned up code a bit, especially setup void setup()
 - Changed MultiWifi to SingleWiFi as debug purpose
 - Removed delays during setup till WiFI have been loaded to reduce problems with WiFi getting IP by DHCP
 - Added version to MQTT payload
 - Added SSID to MQTT payload
 - Added "got IP events" to MQTT, GotIP 
 - Fixed "mqttReconnects" counter to actually count on reconnects
 - Added WIFI and MQTT reconnect counter to MQTT
 - Added "Restart CPU"
 - Added "Reset CPU"
v1.6: 
 - Added button for NTP time reset clock, kind debugging tool to fix ntp time when not connected
 - Added hostname with ESP8266mDNS library, trying to show host name on network (might not be working)
 - Fixed NTP time button on webserver, so its possible to do a forced update of the time - but forced update aint updating!!! SO KNOWN BUG!
 - Added "Epoch time" to webserver, to define wether NTP time has updated or not
 - Fixed redirect ti ../? after clicking buttons on homepage https://tttapa.github.io/ESP8266/Chap10%20-%20Simple%20Web%20Server.html
 - Added "handleNotFound" to webserver handling of links/commands
 - Changed boot order to try and fix NTP time not updating correctly on first boot
 - Added count of MQTT messages sent since last reboot
 - Added multiple wifi points, which the station can connect to https://tttapa.github.io/ESP8266/Chap07%20-%20Wi-Fi%20Connections.html
 - Cleaned up code
 - Fixed SSIDs printed "as is" and not "as ment to be" in serial print and web server
 - Changed sentMessage variable to sentMQTTMessages to make code more clear
 - Added colorcode to green for "normal" operation and red for any errors
 - Added button to reset count of messages since last reboot
 - Added more defines, so right IP adress will be chosen correctly whn defining the ID
v1.5:
 - Added NTP time to code
 - Added NTP time to web interface as "device local time"
 - Added NTP time to OLED to show local time
 - Changed delaytime after wifi has been setup till ntp time calls for "update"
 - Changed readout function from 2000 milliseconds back to 3000 milliseconds to recieve higher %/msg change, which will be used for turning on fans
 - Changed the order of all the includes and define to conform with newly added modules
v1.4:
 - Changed function from sending readouts from regular intervals (15 seconds) to whenever Temperature or Humidity readouts has a significant ammount (>0.1 at this time) changed from last readout
 - Changed time from readouts from 3000 milliseconds to 2000 milliseconds
 - Added "firstMQTTMessageSent" as variable, to force sending first readout
 - Added client loop so MQTT wont loose connection when there have been no change for a while
 - Added Deltatemperature and Deltahumidity to MQTT JSON
v1.3.1:
 - Corrected writings on OLED to stay in one line
 - Added boot text to OLED
v1.3:
 - Added correct handling of WiFi disconnects to just reconnect WiFi
 - Added function to show WiFi disconnects to OLED
 - Cleaned up alot of debug code
 - Added firmware version to webpage, just like the uptime etc.
v1.2:
 - Added 128x64 OLED monitor
 - Changed boot sequence printed on serial interface
 - Fixed endless loop when MQTT connection was lost
 - Added update of OLED when MQTT is down
 - Known bug: Webserver is out of the reconnect loop, hence it wont show the MQTT status change
v1.1:
 - Added OLED monitor, which easily can be disabled, but code works without disabling anything
 - Added Webserver, to show status and technical information about sensors and broadcasting
 - Added Restart MQTT interface via button/link, hopefully this can soft fix connection problems after WiFi AP restarts
v1.0: 
 - First running code with 100% MQTT success and no disconnects. Still got WiFi problems when connections is lost due to restarted broker/WiFi AP.
*/
