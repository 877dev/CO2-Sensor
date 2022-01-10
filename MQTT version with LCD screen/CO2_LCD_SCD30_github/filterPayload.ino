void filter(char *message, uint8_t arraylength)
{

  //Testing only - just for information in serial for debugging
  DEBUG_PRINT("You typed in: ");
  DEBUG_PRINT(message);
  DEBUG_PRINT(" which is this long: ");
  DEBUG_PRINTLN(arraylength);

  //lcd.backlight();    // turn on backlight
  //lcd.noBacklight();  // turn off backlight


  //EXAMPLES:

  //CHECK FOR THE STRING "lcd_on"
  if (strcmp(message, "lcd_on") == 0)   //check for word red
  {
    // Do something
    lcd.backlight();    // turn on backlight.
    return;   //return exits from the subroutine at this point, any other below code is skipped.
  }


  //CHECK FOR THE STRING "lcd_off"
  if (strcmp(message, "lcd_off") == 0)   //check for word red
  {
    // Do something
    lcd.noBacklight();  // turn off backlight
    return;   //return exits from the subroutine at this point, any other below code is skipped.
  }


  else
  {
    //BAD STRING
    DEBUG_PRINTLN("Bad command!");
    DEBUG_PRINTLN();
  }


}
