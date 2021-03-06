/*

  Brainstorm:

  - decouple the ON/OFF of the switch from the ON/OFF of the light
  - in order to enable driver the ability to turn off the light while in operation

  - will incorporate original switch as a pseudo-momentary switch (but its a toggle switch)
  - will have power from interior lights on - to be used as a toggle trigger on/off

  - local push button shall take precedence in function
    - but external trigger MUST be able to turn OFF this light

  - future: switch between red and white modes
  - future: add PWM to be able to dim red and white independently and from other control units

  Actions:
    - switch on/off
      - via local buttons
      - via external trigger
    - switch red/white modes

  Available Triggers:
    - 12v high / low from interior lights on/off (reduced to 3.3v logic )
    - 12v from local button toggle (reduced to 3.3v logic )
      - can local button handle double click with software?
        - can double click toggle red/white
      - local button is toggle button to be used as logic toggle ( if ( localButtonState != localButtonState ){ do stuff } )
        - will it need debounce?

  Use case:
    1. Turn it off from the drivers seat
        - light is toggled ON with local button
        - driver toggles ON interior light via steering column - which in turn sets interior lights system to ON (rear is already ON and stays ON )
        - driver then toggles OFF interior lights via steering column - which in turn sets interior system lights to OFF ( including rear light - -it follows the system state )

  States / Variables:
    System_State
      - is the interior light system on?
      - this is triggered by the Jeep's 'system' interior light on/off status
      - this trigger is OEM wire run to each factory light location - each factory light will detect this independantly ( additionally the main contol unit will detect this )
    Rear_State - has the rear interior light been toggled via the local fixture button
    **Front_State - has the front interior light been toggled via a (there are 2) local fixture button**

    RedLedState
    WhiteLedState
    ButtonState

  Definitions:

  * external trigger is the steering column interior lights trigger (same as door open, etc)
  * local button is the push button / cover of the factory light shell

*/

/* Define pin usage - with non-changing constants (const)*/
const int localButtonPin = 2;     // the number of the local push button pin
const int externalTriggerPin = 3; // the number of the external trigger input
const int redLightsPin =  7;      // the number of the red led lights pin
const int whiteLightsPin =  8;      // the number of the white led lights pin

/* Define variable states - these will change */
/* button and trigger state rep the two 12v inputs */
int localButtonState = LOW; // the current reading from the input pin
int lastButtonState = LOW; // the previous reading from the input pin
int externalTriggerState = LOW;
int lastTriggerState = LOW;
int whiteLedState = HIGH;
int redLedState = LOW;
int globalSystemState = LOW;
int localSystemState = LOW;

/* Lights ON */
void turn_lights_on() {

  Serial.println(" **** Lights ON **** ");

  if(whiteLedState == HIGH){
    digitalWrite(whiteLightsPin, HIGH);
  } else
  if(redLedState == HIGH){
    digitalWrite(redLightsPin, HIGH);
  }

  localSystemState = HIGH;
}

/*
  Lights OFF
  Just turn ALL lights off regardless of mode/state?
  - it might be simpler and safer
*/
void turn_lights_off() {
  Serial.println(" **** Lights OFF **** ");

  // if(whiteLedState == HIGH){
    digitalWrite(whiteLightsPin, LOW);
  // } else
  // if(redLedState == HIGH){
    digitalWrite(redLightsPin, LOW);
  // }

  localSystemState = LOW;
}
/* Toggle local system power */
void toggle_local_state() {
  Serial.println(" **** Toggle Power **** ");
  if(localSystemState == LOW){
    turn_lights_on();
  } else {
    turn_lights_off();
  }
}

void setup() {

  Serial.begin(9600);
  // while (!Serial);
  Serial.println(" **** Rear Passenger Lamp Starting **** ");

  // initialize the LED pins as outputs
  pinMode(redLightsPin, OUTPUT);
  pinMode(whiteLightsPin, OUTPUT);

  // initialize the pushbutton / trigger pins as inputs
  pinMode(localButtonPin, INPUT);
  pinMode(externalTriggerPin, INPUT);

  // set init states for triggers
  localButtonState = digitalRead(localButtonPin);
  lastButtonState = localButtonState;
  externalTriggerState = digitalRead(externalTriggerPin);
  lastTriggerState = externalTriggerState;

}

void loop() {

    // read the state of the pushbutton/trigger value:
    localButtonState = digitalRead(localButtonPin);
    externalTriggerState = digitalRead(externalTriggerPin);

    /*
      First:
      compare the localButtonState to its previous state
      if different then button has been toggled
      (regardless of state, we only care if button state changes - the actual state is irrelevant  )
    */
    if (localButtonState != lastButtonState) {

      // Use the buttons on/off to trigger a state toggle of the lights on/off status
      toggle_local_state();

      // save the current state as the last state, for next time through the loop
      lastButtonState = localButtonState;

    } else {
      // If no changes to local button check trigger
      // compare the trigger State to its previous state
      if(externalTriggerState != lastTriggerState){

        if(externalTriggerState == HIGH){
          turn_lights_on();
        } else {
          turn_lights_off();
        }

        lastTriggerState = externalTriggerState;

      }

    }

    // Poor debounce method might be good enough
    delay(500);

}
