
#include <SeeedOLED.h>

#include "ssd1306.h"
#include "nano_gfx.h"
#include "sova.h"


#include <Wire.h>
#include <i2cEncoderMiniLib.h>
SAppMenu menu;

const char *menuItems[] =
{
    "draw bitmap",
    "sprites",
    "fonts",
    "canvas gfx",
    "draw lines",
};

/*This is a basic example for using the I2C Encoder mini
  The counter is set to work between +10 to -10, at every encoder click the counter value is printed on the terminal.
  It's also printet when the push button is pressed, or released or double pushed.
  The callback are used instead of the switch case

  Connections with Arduino UNO:
  - -> GND
  + -> 5V
  SDA -> A4
  SCL -> A5
  INT -> A3
*/

const int IntPin = A6; /* Definition of the interrupt pin. You can change according to your board */
//Class initialization with the I2C addresses
i2cEncoderMiniLib Encoder(0x20); /* A0 is soldered */

//Callback when the CVAL is incremented
void encoder_increment(i2cEncoderMiniLib* obj) {
  Serial.print("Increment: ");
  Serial.println(Encoder.readCounterByte());

  // TODO: make display write function
  SeeedOled.clearDisplay();          //clear the screen and set start position to top left corner
//  SeeedOled.setNormalDisplay();      //Set display to normal mode (i.e non-inverse mode)
//  SeeedOled.setPageMode();           //Set addressing mode to Page Mode
  SeeedOled.setTextXY(0, 0);         //Set the cursor to Xth Page, Yth Column
  SeeedOled.putNumber(Encoder.readCounterByte());
//SeeedOled.putString("Cole Rylee"); //Print the String
}

//Callback when the CVAL is decremented
void encoder_decrement(i2cEncoderMiniLib* obj) {
  Serial.print("Decrement: ");
  Serial.println(Encoder.readCounterByte());

  SeeedOled.clearDisplay();          //clear the screen and set start position to top left corner
//  SeeedOled.setNormalDisplay();      //Set display to normal mode (i.e non-inverse mode)
//  SeeedOled.setPageMode();           //Set addressing mode to Page Mode
  SeeedOled.setTextXY(0, 0);         //Set the cursor to Xth Page, Yth Column
  SeeedOled.putNumber(Encoder.readCounterByte());
//SeeedOled.putString("Mom & Dad"); //Print the String

}

//Callback when CVAL reach MAX
void encoder_max(i2cEncoderMiniLib* obj) {
  Serial.print("Maximum threshold: ");
  Serial.println(Encoder.readCounterByte());
}

//Callback when CVAL reach MIN
void encoder_min(i2cEncoderMiniLib* obj) {
  Serial.print("Minimum threshold: ");
  Serial.println(Encoder.readCounterByte());
}

//Callback when the encoder is pushed
void encoder_push(i2cEncoderMiniLib* obj) {
  Serial.println("Encoder is pushed!");
}

//Callback when the encoder is released
void encoder_released(i2cEncoderMiniLib* obj) {
  Serial.println("Encoder is released");
}

//Callback when the encoder is double pushed
void encoder_double_push(i2cEncoderMiniLib* obj) {
  Serial.println("Encoder is double pushed!");
}

//Callback when the encoder is long pushed
void encoder_long_push(i2cEncoderMiniLib* obj) {
  Serial.println("Encoder is long pushed!");
}

void setup(void) {

    Wire.begin();
  //  SeeedOled.init();  //initialze SEEED OLED display
  //
  //  SeeedOled.clearDisplay();          //clear the screen and set start position to top left corner
  //  SeeedOled.setNormalDisplay();      //Set display to normal mode (i.e non-inverse mode)
  //  SeeedOled.setPageMode();           //Set addressing mode to Page Mode
  //  SeeedOled.setTextXY(0, 0);         //Set the cursor to Xth Page, Yth Column
  //  SeeedOled.putString("Hello World"); //Print the String
  //  SeeedOled.setBrightness(1);
  //  delay(1000);
  //  SeeedOled.setBrightness(100);
  //  delay(1000);
  //  SeeedOled.setBrightness(1);
  //  delay(1000);
  //  SeeedOled.setBrightness(100);

  // TODO: use 1.5in SPI color monitor
  /* Select the font to use with menu and all font functions */
    ssd1306_setFixedFont(ssd1306xled_font6x8);

    ssd1306_128x64_i2c_init();

    ssd1306_clearScreen();
    ssd1306_createMenu( &menu, menuItems, sizeof(menuItems) / sizeof(char *) );
    ssd1306_showMenu( &menu );


  
  pinMode(IntPin, INPUT);

  Serial.begin(115200);
  Serial.println("**** I2C Encoder Mini basic example ****");
  Encoder.reset();
  Encoder.begin(i2cEncoderMiniLib::WRAP_DISABLE
                | i2cEncoderMiniLib::DIRE_LEFT | i2cEncoderMiniLib::IPUP_ENABLE
                | i2cEncoderMiniLib::RMOD_X1 );

  Encoder.writeCounter((int32_t) 0); /* Reset the counter value */
  Encoder.writeMax((int32_t) 10); /* Set the maximum threshold*/
  Encoder.writeMin((int32_t) - 10); /* Set the minimum threshold */
  Encoder.writeStep((int32_t) 1); /* Set the step to 1*/
  Encoder.writeDoublePushPeriod(50); /*Set a period for the double push of 500ms */

  // Definition of the events
  Encoder.onIncrement = encoder_increment;
  Encoder.onDecrement = encoder_decrement;
  Encoder.onMax = encoder_max;
  Encoder.onMin = encoder_min;
  Encoder.onButtonPush = encoder_push;
  Encoder.onButtonRelease = encoder_released;
  Encoder.onButtonDoublePush = encoder_double_push;
  Encoder.onButtonLongPush = encoder_long_push;

  /* Enable the I2C Encoder V2 interrupts according to the previus attached callback */
  Encoder.autoconfigInterrupt();

}

void loop() {
  if (digitalRead(IntPin) == LOW) {
    /* Check the status of the encoder and call the callback */
    Encoder.updateStatus();
  }
}
