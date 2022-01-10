void MQTT_Connect()
{
  
  //DEBUG_PRINT("BOOL STATUS: ");   //for troubleshooting only
  //DEBUG_PRINTLN(mqtt_fail);       //for troubleshooting only
  DEBUG_PRINTLN("Connecting to MQTT...  ");


  //show info on LCD if this is the first attempt.....
  if (mqtt_fail == false)
  {
    lcd.setCursor(0, 0); // set the cursor to column 0, line 0
    lcd.print("Connecting to   ");  // Print a message to the LCD
    lcd.setCursor(0, 1); // set the cursor to column 0, line 0
    lcd.print("MQTT...             ");  // Print a message to the LCD
  }

  // We'll connect with a Retained Last Will that updates the '.../Status' topic with "Dead" when the device goes offline...
  // Attempt to connect...
  /*
    MQTT Connection syntax:
    boolean connect (client_id, username, password, willTopic, willQoS, willRetain, willMessage)
    Connects the client with a Will message, username and password specified.
    Parameters
    client_id : the client ID to use when connecting to the server.
    username : the username to use. If NULL, no username or password is used (const char[])
    password : the password to use. If NULL, no password is used (const char[])
    willTopic : the topic to be used by the will message (const char[])
    willQoS : the quality of service to be used by the will message (int : 0,1 or 2)
    willRetain : whether the will should be published with the retain flag (int : 0 or 1)
    willMessage : the payload of the will message (const char[])
    Returns
    false - connection failed.
    true - connection succeeded
  */

  if (MQTTclient.connect(mqtt_client_id.c_str(), mqtt_username, mqtt_password, (base_mqtt_topic + "/Status").c_str(), 0, 1, "Dead"))

  {
    // We get here if the connection was successful...
    mqtt_connect_count++;
    DEBUG_PRINTLN("CONNECTED!");

    lcd.clear();
    lcd.setCursor(0, 0); // set the cursor to column 0, line 0
    lcd.print("MQTT");  // Print a message to the LCD
    lcd.setCursor(0, 1); // set the cursor to column 0, line 0
    lcd.print("connected!             ");  // Print a message to the LCD
    delay(2000); //time to read the message
    mqtt_fail = false;  //update to show that mqtt did connect, used to stop spamming LCD screen
    lcd.clear();


    // Once connected, publish some announcements...
    // These all have the Retained flag set to true, so that the value is stored on the server and can be retrieved at any point
    // Check the .../Status topic to see that the device is still online before relying on the data from these retained topics
    MQTTclient.publish((base_mqtt_topic + "/Status").c_str(), "Alive", true);
    MQTTclient.publish((base_mqtt_topic + "/MAC_Address").c_str(), WiFi.macAddress().c_str(), true);                 // Device MAC Address
    MQTTclient.publish((base_mqtt_topic + "/MQTT_Connect_Count").c_str(), String(mqtt_connect_count).c_str(), true); // MQTT Connect Count
    MQTTclient.loop();


    // ... and then re/subscribe to the watched topics
    MQTTclient.subscribe((base_mqtt_topic + sub_mqtt_topic).c_str());   // Watch the subtopic for incoming MQTT messages
    MQTTclient.loop();
    // Add other watched topics in here...
  }
  else
  {
    // We get here if the connection failed...
    DEBUG_PRINT("MQTT Connection FAILED, Return Code = ");
    DEBUG_PRINTLN(MQTTclient.state());
    DEBUG_PRINTLN();


    //If first time mqtt failed, update the LCD, otherwise skip....
    if (mqtt_fail == false)
    {
      //Only print this to LCD once!
      lcd.clear();
      lcd.setCursor(0, 0); // set the cursor to column 0, line 0
      lcd.print("MQTT connection  ");  // Print a message to the LCD
      lcd.setCursor(0, 1); // set the cursor to column 0, line 0
      lcd.print("error ");  // Print a message to the LCD
      lcd.print(MQTTclient.state());
      lcd.print("            ");  // Print a message to the LCD
      delay(5000); //time to read the message
      mqtt_fail = true;
    }


    /*
      MQTTclient.state return code meanings...
      -4 : MQTT_CONNECTION_TIMEOUT - the server didn't respond within the keepalive time
      -3 : MQTT_CONNECTION_LOST - the network connection was broken
      -2 : MQTT_CONNECT_FAILED - the network connection failed
      -1 : MQTT_DISCONNECTED - the client is disconnected cleanly
      0 : MQTT_CONNECTED - the client is connected
      1 : MQTT_CONNECT_BAD_PROTOCOL - the server doesn't support the requested version of MQTT
      2 : MQTT_CONNECT_BAD_CLIENT_ID - the server rejected the client identifier
      3 : MQTT_CONNECT_UNAVAILABLE - the server was unable to accept the connection
      4 : MQTT_CONNECT_BAD_CREDENTIALS - the username/password were rejected
      5 : MQTT_CONNECT_UNAUTHORIZED - the client was not authorized to connect *
    */
  }
} // End of void MQTT_Connect



void MQTTcallback(char* topic, byte* payload, unsigned int length)
{
  // This gets called when a message arrives on one of the subscribed topics.
  // If you're sending messages and nothing happens then you probably forgot to subscribe to the topic in the MQTT_Connect function
  DEBUG_PRINT("MQTT arrived on topic [");   //F means "leave this particular array in FLASH" i.e. save RAM by not preloading array into RAM memory
  DEBUG_PRINT(topic);
  DEBUG_PRINT("]");

  //The below shows the received string in serial monitor
  DEBUG_PRINT(" - payload [");
  for (int i = 0; i < length; i++)
  {
    DEBUG_PRINT((char)payload[i]);
  }
  DEBUG_PRINTLN("]");

  // Convert the byte* payload array into a char array called trimmed_payload...
  char trimmed_payload[length + 1];                           // Extra character needed for the terminating nul character
  memset(trimmed_payload, 0, sizeof(trimmed_payload));        // Clear the contents
  memcpy(trimmed_payload, payload, length);                   // Copy the MQTT payload to trimmed_payload

  // This is my standard way of handling incoming M<QTT messages as strings
  // Still needed to allow thye if statement below to work...
  String msg_topic = String((char *)topic);
  String msg_payload = String((char *)payload);
  msg_payload.remove(length); // Trim any unwanted characters off the end of the string

  // We now have two string variables, 'msg_topic' and 'msg_payload' that we can use in 'if' statements below...

  // This processes a message that comes in on the .../sub topic
  if (msg_topic == base_mqtt_topic + sub_mqtt_topic)
  {

    //send to some function
    filter(trimmed_payload, length); // Pass the raw payload to the filter function
  }

  // Handle messages from other topics in here,
  // just like we did with the CMD topic above.
  // DON'T FORGET to subscribe to the topic in void MQTT_Connect()

} // End of void MQTTcallback
