//Secret credentials example

//Wifi Credentials
#define mySSID "your_ssid"
#define myPASSWORD "your_password"

//ArduinoOTA password
#define myOTApassword "your_ota_password"

//MQTT Credentials
#define myMqttIP (192, 1, 1, 123)
#define myMqttPort 1883
#define myMqttUser "your_mqtt_user"
#define myMqttPassword "your_mqtt_password"

//Define project specific variables
#define MY_VERSION  "YOUR_NAME_HERE"                //The name that will show in MQTT browser and OTAdevice
#define BASE_TOPIC  "YOUR_BASE_TOPIC_HERE"          //The base topic
#define SUB_TOPIC   "/command"                      //The sub topic for MQTT callback functions
#define DEBUG                                       //Debug serial prints - comment out to disable





//Optionally save this file to your PC and add this line to your sketch, then no need to include in every sketch folder..
//#include <C:\arduino\secret_credentials.h>  //or whatever filename/directory you want
//*********************************************************************************************
