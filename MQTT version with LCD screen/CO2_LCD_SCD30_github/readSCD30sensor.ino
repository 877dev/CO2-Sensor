void readSCD30sensor()
{

  //read from the sensor
  if (scd30.dataReady()) {
    DEBUG_PRINTLN("Data available!");

    if (!scd30.read()) {
      DEBUG_PRINTLN("Error reading sensor data");
      lcd.setCursor(0, 1);              // Set the cursor to column 0, line 1
      lcd.print("Read error       ");   // Print error message to LCD bottom line
      return;
    }

    float temperature = (scd30.temperature);
    float rel_hum = (scd30.relative_humidity);
    float co2 = (scd30.CO2);


    DEBUG_PRINT("Temperature: ");
    DEBUG_PRINT(temperature);
    DEBUG_PRINTLN(" degrees C");
    MQTTclient.publish((base_mqtt_topic + "/temp").c_str(), String(temperature).c_str(), true);

    DEBUG_PRINT("Relative Humidity: ");
    DEBUG_PRINT(rel_hum);
    DEBUG_PRINTLN(" %");
    MQTTclient.publish((base_mqtt_topic + "/humidity").c_str(), String(rel_hum).c_str(), true);

    DEBUG_PRINT("CO2: ");
    DEBUG_PRINT(co2);
    DEBUG_PRINTLN(" ppm");
    DEBUG_PRINTLN("");
    //Check CO2 reading is valid, i.e. ignore startup values
    if (co2 > 400)
    {
      MQTTclient.publish((base_mqtt_topic + "/co2").c_str(), String(co2).c_str(), true);
    }



    //Update LCD....
    lcd.setCursor(0, 0); // set the cursor to column 0, line 0
    lcd.print("Temp  Hum  CO2  ");  // Print a message to the LCD - MUST SET TO 15 CHARACTERS TO ALLOW FOR HEARTBEAT SYMBOL TOP RIGHT
    lcd.setCursor(0, 1);            // set the cursor to column 0, line 1
    lcd.print(String(temperature, 1).c_str());
    lcd.print("c ");
    lcd.print(String(rel_hum, 0).c_str());
    lcd.print("%  ");
    //Check CO2 reading is valid, i.e. ignore startup values
    if (co2 > 400)
    {
      lcd.print(String(co2, 0).c_str());
      lcd.print("       ");
    }


  } else {
    //Serial.println("No data");
  }


}
