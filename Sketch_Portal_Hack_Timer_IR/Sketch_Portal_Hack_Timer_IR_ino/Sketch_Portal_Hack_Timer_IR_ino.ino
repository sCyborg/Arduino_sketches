#include <IRremote.h>


int RECV_PIN = 02;
IRrecv irrecv(RECV_PIN);
int LedR1 = 03;                 //Led Red 1
int LedR2 = 04;                 //Led Red 2
int LedR3 = 05;                 //Led Red 3
int LedR4 = 06;                 //Led Red 4
int LedM = 07;                  //Led Multi
int LedG = 8;                  //Led Green
int Button = 12;                // Button
int val;                        // variable for reading the pin status
int buttonState;                // variable to hold the button state
int lightMode = 0;              // Is the light on or off?
decode_results results;

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
  pinMode(Button, INPUT);
  pinMode(LedR1, OUTPUT);
  pinMode(LedR2, OUTPUT);
  pinMode(LedR3, OUTPUT);
  pinMode(LedR4, OUTPUT);
  pinMode(LedM, OUTPUT);
  pinMode(LedG, OUTPUT);
  pinMode(13, OUTPUT);
}

int on = 0;
unsigned long last = millis();

void loop()
{ 
  if (irrecv.decode(&results)) {
    // If it's been at least 1/4 second since the last
    // IR received, toggle the relay
    if (millis() - last > 250) {
      on = !on;
          digitalWrite(LedR1, HIGH);
          delay(60000);
          digitalWrite(LedR2, HIGH);
          delay(60000);
          digitalWrite(LedR3, HIGH);
          delay(60000);
          digitalWrite(LedR4, HIGH);
          delay(110000);
          digitalWrite(LedM, HIGH);
          delay(10000);
          digitalWrite(LedR1, LOW);
          digitalWrite(LedR2, LOW);
          digitalWrite(LedR3, LOW);
          digitalWrite(LedR4, LOW);
          digitalWrite(LedM, LOW);
          digitalWrite(LedG, HIGH);
        } else {
          lightMode == 0;               // light is off!
          digitalWrite(LedR1, LOW);
          digitalWrite(LedR2, LOW);
          digitalWrite(LedR3, LOW);
          digitalWrite(LedR4, LOW);
          digitalWrite(LedM, LOW);
          digitalWrite(LedG, LOW);
    }
    last = millis();      
    irrecv.resume(); // Receive the next value
    }

      
{
  val = digitalRead(Button);      // read input value and store it in val

  if (val != buttonState) {          // the button state has changed!
    if (val == LOW) {                // check if the button is pressed
    if (lightMode == 0) {          // light is off switch to
        lightMode == 1;               // light is on
          Serial.print("Read switch input: ");
          Serial.println(digitalRead(Button));    // Read the pin and display the value
          delay(100);
          digitalWrite(LedR1, HIGH);
          digitalWrite(LedG, LOW);
          delay(60000);
          digitalWrite(LedR2, HIGH);
          delay(60000);
          digitalWrite(LedR3, HIGH);
          delay(60000);
          digitalWrite(LedR4, HIGH);
          delay(110000);
          digitalWrite(LedM, HIGH);
          delay(10000);
          digitalWrite(LedR1, LOW);
          digitalWrite(LedR2, LOW);
          digitalWrite(LedR3, LOW);
          digitalWrite(LedR4, LOW);
          digitalWrite(LedM, LOW);
          digitalWrite(LedG, HIGH);
        } else {
          lightMode == 0;               // light is off!
          digitalWrite(LedR1, LOW);
          digitalWrite(LedR2, LOW);
          digitalWrite(LedR3, LOW);
          digitalWrite(LedR4, LOW);
          digitalWrite(LedM, LOW);
          digitalWrite(LedG, LOW);
      }
     }
    }
    buttonState = val;                 // save the new state in our v
}
    }




