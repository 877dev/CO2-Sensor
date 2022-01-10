void loop ()
{

  // Add support for OTA***************************************
  ArduinoOTA.handle();
  // **********************************************************


  //Call a function regularly to manage heartbeat timer, used in main loop and when connecting to WiFi
  heartbeatTimer();

  //Check if SCD30 sensor has data and print to LCD regularly
  readSCD30sensor();


  //MQTT processing
  if (!MQTTclient.connected())     // Note that MQTTclient.connected() will still return 'true' until the MQTT keepalive timeout has expired (around 35 seconds for my setup)
  {
    //reconnect if not connected
    MQTT_Connect();
  }
  else
  {
    //process mqtt
    MQTTclient.loop();
  }



}
