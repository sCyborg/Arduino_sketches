int LedR1 = 02;
int LedR2 = 03;
int LedR3 = 04;
int LedR4 = 05;
int LedM = 06;
int LedG = 07;
int Button = 12;
int val;                        // variable for reading the pin status
int buttonState;                // variable to hold the button state
int lightMode = 0;              // Is the light on or off?


void setup()
{
  Serial.begin(9600);
  pinMode(Button, INPUT);
  pinMode(LedR1, OUTPUT);
  pinMode(LedR2, OUTPUT);
  pinMode(LedR3, OUTPUT);
  pinMode(LedR4, OUTPUT);
  pinMode(LedM, OUTPUT);
  pinMode(LedG, OUTPUT);
}

void loop()
{
  val = digitalRead(Button);      // read input value and store it in val

  if (val != buttonState) {          // the button state has changed!
    if (val == LOW) {                // check if the button is pressed
      if (lightMode == 0) {          // light is off switch to
        lightMode = 1;               // light is on
          Serial.print("Read switch input: ");
          Serial.println(digitalRead(Button));    // Read the pin and display the value
          delay(100);
          digitalWrite(LedR1, HIGH);
          delay(60000);
          digitalWrite(LedR2, HIGH);
          delay(60000);
          digitalWrite(LedR3, HIGH);
          delay(60000);
          digitalWrite(LedR4, HIGH);
          delay(60000);
          digitalWrite(LedR1, LOW);
          digitalWrite(LedR2, LOW);
          digitalWrite(LedR3, LOW);
          digitalWrite(LedR4, LOW);
          digitalWrite(LedM, HIGH);
          delay(60000);
          digitalWrite(LedM,LOW);
          digitalWrite(LedG, HIGH);
        } else {
          lightMode = 0;               // light is off!
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

