void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  DEBUG_PRINTLN();
  DEBUG_PRINT("Connecting to ");
  DEBUG_PRINTLN(ssid);
  //show info on LCD
  lcd.setCursor(0, 0); // set the cursor to column 3, line 0
  lcd.print("Connecting to   :");  // Print a message to the LCD
  lcd.setCursor(0, 1); // set the cursor to column 3, line 0
  lcd.print(ssid);  // Print a message to the LCD
  

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    DEBUG_PRINT(".");
    heartbeatTimer();  //keep checking heartbeat timer in case we get stuck here (no WiFi connecting)
  }

  DEBUG_PRINTLN("");
  DEBUG_PRINTLN("WiFi connected");
  DEBUG_PRINT("IP address: ");
  DEBUG_PRINTLN(WiFi.localIP());

  lcd.setCursor(0, 0); // set the cursor to column 0, line 0
  lcd.print("WiFi connnected    :");  // Print a message to the LCD
  lcd.setCursor(0, 1); // set the cursor to column 0, line 0
  lcd.print(WiFi.localIP());  // Print a message to the LCD
  delay(2000); //time to read the message
  lcd.clear();


   // Add support for OTA***************************************
  ArduinoOTA.onError([](ota_error_t error) {
    ESP.restart();
  });
  ArduinoOTA.setHostname(ota_name);

  // No authentication by default
  ArduinoOTA.setPassword(ota_pass);  //password defined in secret credentials file (also see 1Password)

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");
  // **********************************************************

  ArduinoOTA.begin();  /* setup the OTA server */

}
