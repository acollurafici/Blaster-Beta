// Solenoid Fire Control
// By AnthonyC

/////////////////////////////////////////////////////////////////

#include "Button2.h"

/////////////////////////////////////////////////////////////////

#define selectorPin  2 // Selector Switch Pin
#define triggerPin  4 // Trigger Pin 

const int solenoidPin = 5; // Solenoid Pin

// Configurable Settings ////////////////////////////////////////

int singleShotPulse = 90; // How long the Solenoid is powered On
int autoPause = 60; // Delay bettween auto/burst shots
int burstAmount = 2; // Burst Mode Shot Count


/////////////////////////////////////////////////////////////////

#define SEMI 1
#define AUTO 2
#define BURST 3

bool shot = false;
bool burst = false;
bool fullAuto = false;
int burstCount = 0;

int fireMode = SEMI;
/////////////////////////////////////////////////////////////////

Button2 trigger, selector;

/////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(9600);
  delay(50);
  Serial.println("\n\nFire Control");
  
  trigger.begin(triggerPin);
  trigger.setPressedHandler(pressed);
  trigger.setReleasedHandler(released);

  pinMode(solenoidPin, OUTPUT);

  selector.begin(selectorPin);
  selector.setReleasedHandler(released);
}

/////////////////////////////////////////////////////////////////

void loop() {
  trigger.loop();
  selector.loop();

  if (fullAuto) { 
    fireOnce();
    delay(autoPause);      
  }

}

/////////////////////////////////////////////////////////////////

void pressed(Button2& btn) {
     if (btn == trigger) {
      Serial.println("Fire!");

      switch (fireMode) {
        case SEMI:
          if (!shot) {
            fireOnce();
            shot = true;
          }
          break;
        case AUTO:
          fullAuto = true;
          break;
        case BURST:
          if (!shot) {
            burstOnce();
          }
          break;
      }
    } 
}


void released(Button2& btn) {
     if (btn == trigger) { // When the trigger is release reset variables.
      shot = false;
      fullAuto = false;
      burstCount = 0;
      
    } else if (btn == selector) { // When Selector switch is release toggle mode 
      switch (fireMode) {
        case SEMI:
          fireMode = BURST;
          Serial.println("Burst");
          break;
        case BURST:
          fireMode = AUTO;
          Serial.println("Auto");
          break;
        case AUTO:
          fireMode = SEMI;
          Serial.println("Semi");
          break;
      }
    }
}

/////////////////////////////////////////////////////////////////

void fireOnce() {
  digitalWrite(solenoidPin, HIGH);
  delay(singleShotPulse);          
  digitalWrite(solenoidPin, LOW);  
}

void burstOnce() {
    shot = true;  
    for (int i = 0; i < burstAmount; i++) {
      fireOnce();
      delay(autoPause); 
      Serial.println(i);
    }    

    shot = false;  
  }
