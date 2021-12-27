/**
  * Human Interface Device named "a11y cya keys"
  * OLED 128x64 i2c@0x3C - SSD1306_SWITCHCAPVCC
  * Rotary Encoder i2c@0x20 interupt@pin3
  * 6 (2x3) Cherry Switches: @pins[0,1,2,8,9,10]
  * * Setup to pull LOW with activated switch, set pins HIGH with internal PULLUP
  *
  */

/**
  * HID Computer Interface
  * https://github.com/NicoHood/HID/wiki/Consumer-API
*/
#include <HID-Project.h>
#include <HID-Settings.h>

/**
  * OLED Display Library
  * https://github.com/adafruit/Adafruit-GFX-Library
*/
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Wire.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

/**
  * DUPPA Encoder Library
*/
#include <i2cEncoderMiniLib.h>
#define ENCODER_ADDRESS 0x20 /* A0 is soldered */
i2cEncoderMiniLib Encoder(ENCODER_ADDRESS);
#define ROTARTY_INT 3

/**
  * Define the 6 switch pins
*/
#define BUTTON_A1 0
#define BUTTON_A2 1
#define BUTTON_A3 2
#define BUTTON_B1 8
#define BUTTON_B2 9
#define BUTTON_B3 10

/**
  * Button Objects
*/
int rectangle1[] = {0,16,42,23,3};
int rectangle2[] = {43,16,42,23,3};
int rectangle3[] = {86,16,42,23,3};
int rectangle4[] = {0,40,42,23,3};
int rectangle5[] = {43,40,42,23,3};
int rectangle6[] = {86,40,42,23,3};

int buttonNameXcord[] = {
  10,55,95,
  10,52,95
};
int buttonNameYcord[] = {
  24,24,24,
  48,48,48
};
char *buttonNames[] = {
  "MUTE", "VID", "",
  "PREV", "PLAY", "NEXT",
  "MEDIA_VOL_UP", "MEDIA_VOL_DOWN", "MEDIA_VOLUME_MUTE"
};

void button1Actions(){
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.press('M');
}
void button2Actions(){
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.press('V');
}
void button3Actions(){}
void button4Actions(){
  Consumer.write(MEDIA_PREVIOUS);
}
void button5Actions(){
  Consumer.write(MEDIA_PLAY_PAUSE);
}
void button6Actions(){
  Consumer.write(MEDIA_NEXT);
}
void encoderDecrementActions(){
  Consumer.write(MEDIA_VOLUME_DOWN);
  // Mouse.move(-40, 0, 0); // etch-a-sketch
}
void encoderIncrementActions(){
  Consumer.write(MEDIA_VOLUME_UP);
  // Mouse.move(40, 0, 0); // etch-a-sketch
}
void encoderPushActions(){
  Consumer.write(MEDIA_VOLUME_MUTE);
}


/**
  * Initialize Debounce for all buttons
*/
bool a1ButtonState;
bool a2ButtonState;
bool a3ButtonState;
bool b1ButtonState;
bool b2ButtonState;
bool b3ButtonState;

bool last_a1ButtonState = HIGH;
bool last_a2ButtonState = HIGH;
bool last_a3ButtonState = HIGH;
bool last_b1ButtonState = HIGH;
bool last_b2ButtonState = HIGH;
bool last_b3ButtonState = HIGH;

// the following variables are unsigned longs because the time, measured in milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 20;    // the debounce time; increase if the output flickers

void setup() {

  // Set the 6 switches pulled HIGH/UP - activated switch is pulled LOW
  pinMode(BUTTON_A1, INPUT_PULLUP); // pin 0 / key 1 / space(row/col) A1
  pinMode(BUTTON_A2, INPUT_PULLUP); // pin 1 / key 2 / space(row/col) A2
  pinMode(BUTTON_A3, INPUT_PULLUP); // pin 2 / key 3 / space(row/col) A3
  pinMode(BUTTON_B1, INPUT_PULLUP); // pin 8 / key 4 / space(row/col) B1
  pinMode(BUTTON_B2, INPUT_PULLUP); // pin 9 / key 5 / space(row/col) B2
  pinMode(BUTTON_B3, INPUT_PULLUP); // pin 10 / key 6 / space(row/col) B3

  // Set the Rotarty Interupt Pin
  pinMode(ROTARTY_INT, INPUT_PULLUP); // pin 3 / rotary i2c interupt

  Wire.begin();
  Serial.begin(115200);
  Consumer.begin(); //initialize computer connection
    delay(1000);
  Keyboard.begin();

  Serial.println("**** a11y-cya-keys ****");

  Encoder.reset();
  Encoder.begin(
    i2cEncoderMiniLib::WRAP_ENABLE
  | i2cEncoderMiniLib::DIRE_RIGHT
  | i2cEncoderMiniLib::IPUP_ENABLE
  | i2cEncoderMiniLib::RMOD_X2
  );

  // Encoder.writeCounter((int32_t) 0); /* Reset the counter value */
  // Encoder.writeMax((int32_t) 10); /* Set the maximum threshold*/
  // Encoder.writeMin((int32_t) - 10); /* Set the minimum threshold */
  // Encoder.writeStep((int32_t) 1); /* Set the step to 1*/
  Encoder.writeDoublePushPeriod(50); /*Set a period for the double push of 500ms */

  // Definition of the events
  Encoder.onIncrement = encoder_increment;
  Encoder.onDecrement = encoder_decrement;
  // Encoder.onMax = encoder_max;
  // Encoder.onMin = encoder_min;
  Encoder.onButtonPush = encoder_push;
  Encoder.onButtonRelease = encoder_released;
  Encoder.onButtonDoublePush = encoder_double_push;
  Encoder.onButtonLongPush = encoder_long_push;

  /* Enable the I2C Encoder V2 interrupts according to the previus attached callback */
  Encoder.autoconfigInterrupt();

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  draw_display();

}

void encoder_push(i2cEncoderMiniLib* obj) {
  Serial.println("Pressed Encoder");
  press_key("E_PUSH");
}

void encoder_double_push(i2cEncoderMiniLib* obj) {
  Serial.println("Double Pressed Encoder");
}

void encoder_released(i2cEncoderMiniLib* obj) {
  Serial.println("Encoder Released");
}

void encoder_long_push(i2cEncoderMiniLib* obj) {
  Serial.println("Long Pressed Encoder");
}

void encoder_increment(i2cEncoderMiniLib* obj) {
  Serial.println("Rotated Encoder: LEFT");
  press_key("E_DOWN");
}

void encoder_decrement(i2cEncoderMiniLib* obj) {
  Serial.println("Rotated Encoder: RIGHT");
  press_key("E_UP");
}

void draw_display() {

  display.clearDisplay();

  // top-large mode label
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(24,0);
  display.println("JEEP.io");

  display.setTextSize(1);

  // key 1 label
  display.drawRoundRect(rectangle1[0],rectangle1[1],rectangle1[2],rectangle1[3],rectangle1[4],SSD1306_WHITE);
  display.setCursor(buttonNameXcord[0],buttonNameYcord[0]);
  display.println(buttonNames[0]);

  // key 2 label
  display.drawRoundRect(rectangle2[0],rectangle2[1],rectangle2[2],rectangle2[3],rectangle2[4],SSD1306_WHITE);
  display.setCursor(buttonNameXcord[1],buttonNameYcord[1]);
  display.println(buttonNames[1]);

  // key 3 label
  display.drawRoundRect(rectangle3[0],rectangle3[1],rectangle3[2],rectangle3[3],rectangle3[4],SSD1306_WHITE);
  display.setCursor(buttonNameXcord[2],buttonNameYcord[2]);
  display.println(buttonNames[2]);

  // key 4 label
  display.drawRoundRect(rectangle4[0],rectangle4[1],rectangle4[2],rectangle4[3],rectangle4[4],SSD1306_WHITE);
  display.setCursor(buttonNameXcord[3],buttonNameYcord[3]);
  display.println(buttonNames[3]);

  // key 5 label
  display.drawRoundRect(rectangle5[0],rectangle5[1],rectangle5[2],rectangle5[3],rectangle5[4],SSD1306_WHITE);
  display.setCursor(buttonNameXcord[4],buttonNameYcord[4]);
  display.println(buttonNames[4]);

  // key 6 label
  display.drawRoundRect(rectangle6[0],rectangle6[1],rectangle6[2],rectangle6[3],rectangle6[4],SSD1306_WHITE);
  display.setCursor(buttonNameXcord[5],buttonNameYcord[5]);
  display.println(buttonNames[5]);

  display.display();
}

void press_key(char* key) {

  // Serial.print(" button1Actions ");
  // Serial.println(sizeof(button1Actions));


  Serial.print("Pressed: ");

  // _-_-_-   1:A1    _-_-_- //
  // _-_-_- [*][ ][ ] _-_-_- //
  // _-_-_- [ ][ ][ ] _-_-_- //
  if(key == "A1"){

    // TESTING
    Serial.print("Key A1: ");
    Serial.println(buttonNames[0]);

    // ACTIONS
    button1Actions();

    // FEEDBACK
    display.fillRoundRect(rectangle1[0],rectangle1[1],rectangle1[2],rectangle1[3],rectangle1[4],SSD1306_WHITE);
    display.setTextColor(SSD1306_BLACK);
    display.setCursor(buttonNameXcord[0],buttonNameYcord[0]);
    display.println(buttonNames[0]);
    display.display();

  }

  // _-_-_-   2:A2    _-_-_- //
  // _-_-_- [ ][*][ ] _-_-_- //
  // _-_-_- [ ][ ][ ] _-_-_- //
  if(key == "A2"){

    // TESTING
    Serial.print("Key A2: ");
    Serial.println(buttonNames[1]);

    // ACTIONS
    button2Actions();

    // Mouse.press(MOUSE_MIDDLE);

    // FEEDBACK
    display.fillRoundRect(43,16,42,23,3,SSD1306_WHITE);
    display.setTextColor(SSD1306_BLACK);
    display.setCursor(53,24);
    display.println(buttonNames[1]);
    display.display();

  }

  // _-_-_-   3:A3    _-_-_- //
  // _-_-_- [ ][ ][*] _-_-_- //
  // _-_-_- [ ][ ][ ] _-_-_- //
  if(key == "A3"){

    // TESTING
    Serial.print("Key A3: ");
    Serial.println(buttonNames[2]);

    // ACTIONS
    button3Actions();
    // Keyboard.press(KEY_LEFT_ALT);
    // Keyboard.press(KEY_LEFT_CTRL);
    // Keyboard.press(KEY_LEFT_GUI);
    // Keyboard.press('n');
    // Keyboard.press('u');
    // Keyboard.press('l');
    // Keyboard.release('l');
    // Keyboard.press('l');

    // Mouse.press(MOUSE_RIGHT);

    // FEEDBACK
    display.fillRoundRect(86,16,42,23,3,SSD1306_WHITE);
    display.setTextColor(SSD1306_BLACK);
    display.setCursor(95,24);
    display.println(buttonNames[2]);
    display.display();

  }

  // _-_-_-   4:B1    _-_-_- //
  // _-_-_- [ ][ ][ ] _-_-_- //
  // _-_-_- [*][ ][ ] _-_-_- //
  if(key == "B1"){

    // TESTING
    Serial.print("Key B1: ");
    Serial.println(buttonNames[3]);

    // ACTIONS
    button4Actions();

    // FEEDBACK
    display.fillRoundRect(0,40,42,23,3,SSD1306_WHITE);
    display.setTextColor(SSD1306_BLACK);
    display.setCursor(10,48);
    display.println(buttonNames[3]);
    display.display();

  }

  // _-_-_-   5:B2    _-_-_- //
  // _-_-_- [ ][ ][ ] _-_-_- //
  // _-_-_- [ ][*][ ] _-_-_- //
  if(key == "B2"){

    // TESTING
    Serial.print("Key B2: ");
    Serial.println(buttonNames[4]);

    // ACTIONS
    button5Actions();

    // FEEDBACK
    display.fillRoundRect(43,40,42,23,3,SSD1306_WHITE);
    display.setTextColor(SSD1306_BLACK);
    display.setCursor(49,48);
    display.println(buttonNames[4]);
    display.display();

  }

  // _-_-_-   6:B3    _-_-_- //
  // _-_-_- [ ][ ][ ] _-_-_- //
  // _-_-_- [ ][ ][*] _-_-_- //
  if(key == "B3"){

    // TESTING
    Serial.print("Key B3: ");
    Serial.println(buttonNames[5]);

    // ACTIONS
    button6Actions();

    // FEEDBACK
    display.fillRoundRect(86,40,42,23,3,SSD1306_WHITE);
    display.setTextColor(SSD1306_BLACK);
    display.setCursor(95,48);
    display.println(buttonNames[5]);
    display.display();

  }

  if(key == "E_UP"){
    // TESTING
    Serial.print("Key E_UP: ");
    Serial.println(buttonNames[6]);

    // ACTIONS
    encoderIncrementActions();

  }

  if(key == "E_DOWN"){
    // TESTING
    Serial.print("Key E_DOWN: ");
    Serial.println(buttonNames[7]);

    // ACTIONS
    encoderDecrementActions();
  }

  if(key == "E_PUSH"){
    // TESTING
    Serial.print("Key E_PUSH: ");
    Serial.println(buttonNames[8]);

    // ACTIONS
    encoderPushActions();
  }

}

void loop() {

  // read the rotary encoder, a.k.a the knob
  if (digitalRead(ROTARTY_INT) == LOW) {
    /* Check the status of the encoder and call the callback */
    Encoder.updateStatus();
  }

  int readingA1 = digitalRead(BUTTON_A1);

  if (readingA1 != last_a1ButtonState) {
   lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
   if (readingA1 != a1ButtonState) {
     a1ButtonState = readingA1;
     Keyboard.releaseAll();
     if (a1ButtonState == LOW) {
       press_key("A1");
     } else {
       draw_display();
     }
   }
  }
  last_a1ButtonState = readingA1;

  int readingA2 = digitalRead(BUTTON_A2);
  if (readingA2 != last_a2ButtonState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (readingA2 != a2ButtonState) {
      a2ButtonState = readingA2;
      Keyboard.releaseAll();
      // Mouse.release(MOUSE_MIDDLE);
      if (a2ButtonState == LOW) {
        press_key("A2");
      } else {
        draw_display();
      }
    }
  }
  last_a2ButtonState = readingA2;

  int readingA3 = digitalRead(BUTTON_A3);
  if (readingA3 != last_a3ButtonState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (readingA3 != a3ButtonState) {
      a3ButtonState = readingA3;
      Keyboard.releaseAll();
      if (a3ButtonState == LOW) {
        press_key("A3");
      } else {
        draw_display();
      }
    }
  }
  last_a3ButtonState = readingA3;

  int readingB1 = digitalRead(BUTTON_B1);
  if (readingB1 != last_b1ButtonState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (readingB1 != b1ButtonState) {
      b1ButtonState = readingB1;
      Keyboard.releaseAll();
      if (b1ButtonState == LOW) {
        press_key("B1");
      } else {
        draw_display();
      }
    }
  }
  last_b1ButtonState = readingB1;

  int readingB2 = digitalRead(BUTTON_B2);
  if (readingB2 != last_b2ButtonState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (readingB2 != b2ButtonState) {
      b2ButtonState = readingB2;
      Keyboard.releaseAll();
      if (b2ButtonState == LOW) {
        press_key("B2");
      } else {
        draw_display();
      }
    }
  }
  last_b2ButtonState = readingB2;

  int readingB3 = digitalRead(BUTTON_B3);
  if (readingB3 != last_b3ButtonState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (readingB3 != b3ButtonState) {
      b3ButtonState = readingB3;
      Keyboard.releaseAll();
      if (b3ButtonState == LOW) {
        press_key("B3");
      } else {
        draw_display();
      }
    }
  }
  last_b3ButtonState = readingB3;

}
