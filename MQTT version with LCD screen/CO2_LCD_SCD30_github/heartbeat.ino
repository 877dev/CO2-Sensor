void heartbeatTimer()   //Timer to manage heartbeat calls
{
  unsigned long currentMillis = millis();         //read millis to a variable to save time and cpu

  //Check if time for heartbeat
  if (currentMillis - previousMillis > interval)
  {
    previousMillis = currentMillis;
    heartbeat();
  }


  if (heartbeat_fail == false)   // i.e. heartbeat was ok..
  {
    unsigned long currentMillis2 = millis();         //read millis to a variable to save time and cpu

    if (currentMillis2 - lcdPreviousMillis > 250)   //how long the heartbeat symbol can stay on screen i.e. 250ms
    {
      //Clear the heartbeat char from LCD every 500ms
      lcd.setCursor(15, 0);     // move cursor to (15, 0) TOP RIGHT
      lcd.print(" ");           // Clear the space
      heartbeat_fail = true;     //reset flag
    }
  }

  
}





void heartbeat()  // function for heartbeat check and watchdog pat, called regularly by millis timer
{
  //publish freeram value first of all
  MQTTclient.publish((base_mqtt_topic + "/freeram").c_str(), String(ESP.getFreeHeap()).c_str(), true);

  //Heartbeat check
  if (!MQTTclient.publish((base_mqtt_topic + "/heartbeat").c_str(), String(WiFi.RSSI()).c_str(), true))
  {
    //if MQTT message NOT published successfully, i.e. heartbeat missed
    ++missedHeartbeats; //increment heartbeats missed

    //Print some information
    DEBUG_PRINTLN();
    DEBUG_PRINTLN((String)"Heartbeat missed " + missedHeartbeats + " times");
    DEBUG_PRINTLN((String)"Max heartbeats missed is " + maxMissedHeartbeats);

    lcd.setCursor(15, 0); // move cursor to (15, 0) TOP RIGHT
    lcd.print(" ");  // print a space to clear
    heartbeat_fail = true;  //use this for printing flashing hear symbol top right of LCD.

    if (missedHeartbeats == maxMissedHeartbeats)
    {
      DEBUG_PRINTLN("WATCHDOG TIMEOUT-  Rebooting...");
      lcd.clear();
      lcd.setCursor(0, 0);                // set the cursor to column 0, line 0
      lcd.print("WATCHDOG         ");     // Print a message to the LCD
      lcd.setCursor(0, 1);                // set the cursor to column 0, line 0
      lcd.print("REBOOTING        ");     // Print a message to the LCD
      delay(5000);                        // Delay to read the message

      ESP.restart();
    }
  }
  else //MQTT published successfully
  {
    DEBUG_PRINTLN("Heartbeat sent");
    DEBUG_PRINTLN();
    missedHeartbeats = 0;                 //reset the counter
    
    //Optional - Pat the hardware watchdog.....
    //digitalWrite(LED_BUILTIN, LOW);
    //delay(100);
    //digitalWrite(LED_BUILTIN, HIGH);

    //Print heart symbol to top right of LCD...
    lcd.setCursor(15, 0); // move cursor to (15, 0) TOP RIGHT
    lcd.write((byte)0);  // print the custom char there.
    heartbeat_fail = false;  //use this for printing flashing hear symbol top right of LCD.
    lcdPreviousMillis = millis();

  }
}
