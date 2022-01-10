// SEE NOTES TAB

#include "secret_credentials.h"         //Secret credentials file, WiFi passwords etc..

//**************************************************************************************************************
//Setup variables for heartbeat
int interval = 10000;                     //(10'000 = 10 seconds) Set heartbeat interval
int maxMissedHeartbeats = 180;            //(180 * 10'000 = 30mins) Number of allowed missed heartbeats before watchdog 'bites'

//Fixed variables - do not change
int missedHeartbeats = 0;               //Do not change
unsigned long previousMillis = 0;       //Do not change
unsigned long lcdPreviousMillis = 0;    //Last time LCD heartbeat was updated
bool heartbeat_fail = false;            //use this for printing flashing heart symbol at top right of LCD.
bool mqtt_fail = false;                 //use this to stop spamming LCD with "MQTT not connected" messages, only get ONE.
//**************************************************************************************************************


//Debugging
#ifdef DEBUG
#define DEBUG_PRINT(x)    Serial.print (x)
#define DEBUG_PRINTLN(x)  Serial.println (x)
#define DEBUG_PRINTLNHEX(x) Serial.println (x, HEX)
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#define DEBUG_PRINTLNHEX(x)
#endif
//


//Libraries
#include <PubSubClient.h>       // https://github.com/knolleary/pubsubclient
#include <ESP8266WiFi.h>        // connection to WiFi - not needed if ArduinoOTA libray is included
#include <ESP8266mDNS.h>        // for OTA updates
#include <WiFiUdp.h>            // for OTA updates
#include <ArduinoOTA.h>         // for OTA updates
#include <Adafruit_SCD30.h>     // for co2 sensor
#include <Wire.h>
#include <LiquidCrystal_I2C.h>    //not the main LiquidCrystal library, must use I2C version..ignore AVR warning when compiling.

//Setup the LCD
LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

//Setup LCD custom heart character for heartbeat, see notes for website help
byte customChar[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000,
  0b00000
};

//Setup the SCD30 sensor
Adafruit_SCD30  scd30; 


//Wifi credentials
const char ssid[] = mySSID;           //Enter your WIFI Name 
const char pass[] = myPASSWORD;       //Enter your WIFI Password

//ArduinoOTA
const char ota_name[] = MY_VERSION;     //Enter your name for the OTA service
const char ota_pass[] = myOTApassword;  //Enter your OTA password


//MQTT Server Setting variables************************************************************************************************************************************
IPAddress mqtt_server_ip          myMqttIP;         // Change to the IP Address for your MQTT Server...
const int mqtt_port =             myMqttPort;       // Port for the MQTT Server...
const char *mqtt_username =       myMqttUser;       // Change to your MQTT Server username...
const char *mqtt_password =       myMqttPassword;   // Change to your MQTT Server password...
String mqtt_client_id =           MY_VERSION;       // Used for unique MQTT Client ID
String base_mqtt_topic =          BASE_TOPIC;       // Start of the MQTT Topic name used by this device
String sub_mqtt_topic =           SUB_TOPIC;        // The subtopic used for mqtt callback commands
int mqtt_connect_count;                             // Count of how may times we've connected to the MQTT server since booting (should always be 1 or more)
WiFiClient My_WiFi_Client;
PubSubClient MQTTclient(My_WiFi_Client);
//******************************************************************************************************************************************************************


void setup() {

#ifdef DEBUG
  Serial.begin(74880);
#endif

  lcd.init();                     // initialize the lcd
  lcd.backlight();                // turn on the backlight, also can be done via mqtt callback since v003
  lcd.clear();
  lcd.createChar(0, customChar);  // create a new custom character for heartbeat

  setup_wifi(); //connect to wifi and start OTA server

  //MQTT setup and connect
  MQTTclient.setServer(mqtt_server_ip, mqtt_port); //Setup MQTT broker information as defined earlier
  MQTTclient.setCallback(MQTTcallback);            //Set callback details - this function is called automatically whenever a message arrives on a subscribed topic (a bit like BLYNK_WRITE)
  MQTT_Connect();                                  //Connect to MQTT broker, publish Status/MAC/count, and subscribe to keypad topic


  //Optional - print some info to user
  DEBUG_PRINTLN();
  DEBUG_PRINT("User mqtt client id: ");
  DEBUG_PRINTLN(mqtt_client_id);
  DEBUG_PRINT("User base topic: ");
  DEBUG_PRINTLN(base_mqtt_topic);
  DEBUG_PRINT("User sub topic: ");
  DEBUG_PRINTLN(sub_mqtt_topic);
  DEBUG_PRINT("User OTA name: ");
  DEBUG_PRINTLN(ota_name);
  DEBUG_PRINTLN();


  //Set any digital pins outputs/inputs here


  //Any start code here
  DEBUG_PRINTLN("Adafruit SCD30 test!");

  // Try to initialize!
  if (!scd30.begin()) {
    DEBUG_PRINTLN("Failed to find SCD30 chip");
    lcd.clear();
    lcd.setCursor(0, 0); // set the cursor to column 0, line 0
    lcd.print("SDC30 missing!");  // Print a message to the LCD
    while (1) {
      delay(10);
    }
  }
  DEBUG_PRINTLN("SCD30 Found!");
  lcd.clear();
  lcd.setCursor(0, 0); // set the cursor to column 0, line 0
  lcd.print("SDC30 found");  // Print a message to the LCD
  delay(2000); //time to read the message
  lcd.clear();


  if (!scd30.setMeasurementInterval(5)) {
    DEBUG_PRINTLN("Failed to set measurement interval");
    while (1) {
      delay(10);
    }
  }
  DEBUG_PRINT("Measurement Interval: ");
  DEBUG_PRINT(scd30.getMeasurementInterval());
  DEBUG_PRINTLN(" seconds");
}
