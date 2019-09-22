# MQTTMultiWeatherStation
Homemade sensor working with lots of different accessories to a Wemos D1 mini.
Connects via WIFI to a MQTT broker, where it delivers all the measurements in JSON format

Compiles with Arduino Genuino dev tool

# Lists of sensors:
BME280 for temperature, humidity and barometric pressure
AM2302/DHT22 for temperature and humidty
AK9753 for IR reply from the surroundings, can be used for presence sensing


# Features:
Oled screen: Small screen can be connected via I2C, SSD1306 with 48x64 resolution
Everythings synced through ntp
OTA available to update the sensors
mDNS to be used with Bonjour for accessing sensors without knowing the IP address
MQTT used to transfer data
MultiWIFI to connect to prioritized WIFI networks in range
Small webserver embedded to show data, control some part of the sensor and show a log of whats happening
Prepared for MAX31855 thermocoupler unit, not fully implemented or tested though

# Setup:
Change the following to match your own setup in MQTTMultiWeatherStation.ino file:
const char* ssidOne = "xxxx";
const char* passwordOne = "yyyy";

const char* ssidTwo = "xxxx2";
const char* passwordTwo = "yyyy2";

const char* mqtt_server = "cccc3";
const char* MQTTUser = "ccc";
const char* MQTTPass = "hhhh";
