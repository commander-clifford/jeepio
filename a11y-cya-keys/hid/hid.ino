// MVP it's totally dumb and repetitive but it works for now, TODO:DRY

// https://github.com/NicoHood/HID/wiki/Consumer-API
#include <HID-Project.h>
#include <HID-Settings.h>
// #include "Keyboard.h"

#include <Wire.h>
#include <i2cEncoderMiniLib.h>
#include <SPI.h>

//https://github.com/adafruit/Adafruit-GFX-Library
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define UI_BMPWIDTH  128

const int IntPin = 3; /* Definition of the interrupt pin. You can change according to your board */
//Class initialization with the I2C addresses
i2cEncoderMiniLib Encoder(0x20); /* A0 is soldered */

const int a1_Button = 0;
const int a2_Button = 1;
const int a3_Button = 2;
const int b1_Button = 8;
const int b2_Button = 9;
const int b3_Button = 10;

int myPins[] = {2, 4, 8, 3, 6};
char *buttonNames[] = {
  "HAND", "CAM>", "CTRL", "MUTE", "VIDEO", "VIEW",
  "MEDIA_VOL_UP", "MEDIA_VOL_DOWN", "MEDIA_PLAY/PAUSE"
};

// const char buttons[] =
// {
//   {title: "HAND"},
//   {title: "CAM>"}
// };

int a1ButtonState;
int a2ButtonState;
int a3ButtonState;
int b1ButtonState;
int b2ButtonState;
int b3ButtonState;

int last_a1ButtonState = HIGH;
int last_a2ButtonState = HIGH;
int last_a3ButtonState = HIGH;
int last_b1ButtonState = HIGH;
int last_b2ButtonState = HIGH;
int last_b3ButtonState = HIGH;

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 20;    // the debounce time; increase if the output flickers

//Callback when the encoder is pushed
void encoder_push(i2cEncoderMiniLib* obj) {
  Serial.println("Pressed: Encoder: PLAY/PAUSE");
  Consumer.write(MEDIA_PLAY_PAUSE);
}

void encoder_double_push(i2cEncoderMiniLib* obj) {
  Serial.println("Double Pressed: Encoder: MEDIA_NEXT");
  Consumer.write(MEDIA_NEXT);
}

void encoder_released(i2cEncoderMiniLib* obj) {
  Serial.println("encoder_released");
}

void encoder_long_push(i2cEncoderMiniLib* obj) {
  Serial.println("encoder_long_push");
}

//Callback when the CVAL is incremented
void encoder_increment(i2cEncoderMiniLib* obj) {
  // Serial.println("Encoder: Down: Volume Down");
  press_key("E_DOWN");
}

//Callback when the CVAL is decremented
void encoder_decrement(i2cEncoderMiniLib* obj) {
  // Serial.println("Encoder: Up: Volume Up");
  press_key("E_UP");
}

void draw_display() {

  display.clearDisplay();

  // top-large mode label
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(46,0);
  display.println("HBO");

  display.setTextSize(1);

  // key 1 label
  display.drawRoundRect(0,16,42,23,3,SSD1306_WHITE);
  display.setCursor(10,24);
  display.println(buttonNames[0]);

  // key 2 label
  display.drawRoundRect(43,16,42,23,3,SSD1306_WHITE);
  display.setCursor(53,24);
  display.println(buttonNames[1]);

  // key 3 label
  display.drawRoundRect(86,16,42,23,3,SSD1306_WHITE);
  display.setCursor(95,24);
  display.println(buttonNames[2]);

  // key 4 label
  display.drawRoundRect(0,40,42,23,3,SSD1306_WHITE);
  display.setCursor(10,48);
  display.println(buttonNames[3]);

  // key 5 label
  display.drawRoundRect(43,40,42,23,3,SSD1306_WHITE);
  display.setCursor(49,48);
  display.println(buttonNames[4]);

  // key 6 label
  display.drawRoundRect(86,40,42,23,3,SSD1306_WHITE);
  display.setCursor(95,48);
  display.println(buttonNames[5]);

  display.display();
}

void press_key(char* key) {
  Serial.print("Pressed: ");

  // _-_-_-   1:A1    _-_-_- //
  // _-_-_- [*][ ][ ] _-_-_- //
  // _-_-_- [ ][ ][ ] _-_-_- //
  if(key == "A1"){

    // TESTING
    Serial.print("Key A1: ");
    Serial.println(buttonNames[0]);

    // ACTIONS
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.press('Y');

    // FEEDBACK
    display.fillRoundRect(0,16,42,23,3,SSD1306_WHITE);
    display.setTextColor(SSD1306_BLACK);
    display.setCursor(10,24);
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
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press('N');

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
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press('H');

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
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press('A');

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
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press('V');

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
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press('W');

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
    Consumer.write(MEDIA_VOLUME_UP);

  }

  if(key == "E_DOWN"){
    // TESTING
    Serial.print("Key E_DOWN: ");
    Serial.println(buttonNames[7]);

    // ACTIONS
    Consumer.write(MEDIA_VOLUME_DOWN);
  }

}

void setup() {

  pinMode(a1_Button, INPUT_PULLUP); // pin 0 / key 1 / space(row/col) A1
  pinMode(a2_Button, INPUT_PULLUP); // pin 1 / key 2 / space(row/col) A2
  pinMode(a3_Button, INPUT_PULLUP); // pin 2 / key 3 / space(row/col) A3
  pinMode(b1_Button, INPUT_PULLUP); // pin 8 / key 4 / space(row/col) B1
  pinMode(b2_Button, INPUT_PULLUP); // pin 9 / key 5 / space(row/col) B2
  pinMode(b3_Button, INPUT_PULLUP); // pin 10 / key 6 / space(row/col) B3
  pinMode(IntPin, INPUT_PULLUP); // pin 3 / rotary interupt

  Wire.begin();
  Serial.begin(115200);

  Serial.println("**** a11y-cya-keys ****");
  Encoder.reset();
  Encoder.begin(
    i2cEncoderMiniLib::WRAP_ENABLE
  | i2cEncoderMiniLib::DIRE_RIGHT
  | i2cEncoderMiniLib::IPUP_ENABLE
  | i2cEncoderMiniLib::RMOD_X2
  );

  Encoder.writeCounter((int32_t) 0); /* Reset the counter value */
  Encoder.writeMax((int32_t) 10); /* Set the maximum threshold*/
  Encoder.writeMin((int32_t) - 10); /* Set the minimum threshold */
  Encoder.writeStep((int32_t) 1); /* Set the step to 1*/
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



  Consumer.begin();                         //initialize computer connection
  delay(1000);
  Keyboard.begin();

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  draw_display();

}

void loop() {

  // read the rotary encoder, a.k.a the knob
  if (digitalRead(IntPin) == LOW) {
    /* Check the status of the encoder and call the callback */
    Encoder.updateStatus();
  }

  int readingA1 = digitalRead(a1_Button);

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

  int readingA2 = digitalRead(a2_Button);
  if (readingA2 != last_a2ButtonState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (readingA2 != a2ButtonState) {
      a2ButtonState = readingA2;
      Keyboard.releaseAll();
      if (a2ButtonState == LOW) {
        press_key("A2");
      } else {
        draw_display();
      }
    }
  }
  last_a2ButtonState = readingA2;

  int readingA3 = digitalRead(a3_Button);
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

  int readingB1 = digitalRead(b1_Button);
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

  int readingB2 = digitalRead(b2_Button);
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

  int readingB3 = digitalRead(b3_Button);
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
