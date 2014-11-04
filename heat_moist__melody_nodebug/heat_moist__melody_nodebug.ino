#include <OneWire.h>
#include <DallasTemperature.h>
#include <pitches.h>

float m1 = 0;    // Papertowels
float m2 = 0;    //Santaka
float m3 = 0;   //Bhut Jolokia
float m4 = 0;   // C. Reaper
int ledR = 4;
int ledG = 6;
const int Buzz = 3;
#define ONE_WIRE_BUS 5
int sound = 250;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress insideThermometer;
int Relay = 2;
unsigned long time;
unsigned long time2;
int melody[]= {
  NOTE_C4, NOTE_G3,NOTE_G3, NOTE_A3, NOTE_G3,0, NOTE_B3, NOTE_C4};
int noteDurations[] = {
  4, 8, 8, 4,4,4,4,4 };

void setup(){
    pinMode(ledR,OUTPUT);
    pinMode(ledG,OUTPUT);
    pinMode(Buzz,OUTPUT);
    pinMode(Relay, OUTPUT);
  Serial.begin(57600);
  sensors.begin();
  if (!sensors.getAddress(insideThermometer, 0)) Serial.println("Unable to find address for Device 0"); 
  sensors.setResolution(insideThermometer, 10);
  sensors.setHighAlarmTemp(insideThermometer, 32);
  sensors.setLowAlarmTemp(insideThermometer, 30);
  sensors.setAlarmHandler(&newAlarmHandler);
  melodyPlay();
}
void melodyPlay(){
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second 
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000/noteDurations[thisNote];
    tone(3, melody[thisNote],noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(3);
  }
}

void newAlarmHandler(uint8_t* deviceAddress)
{
  Serial.println("Alarm Handler Start"); 
  printAlarmInfo(deviceAddress);
  printTemp(deviceAddress);
  Serial.println();
  Serial.println("Alarm Handler Finish");
}

void printCurrentTemp(DeviceAddress deviceAddress)
{
  printTemp(deviceAddress);
}

void printAddress(DeviceAddress deviceAddress)
{
  Serial.print("Address: ");
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
  Serial.print(" ");
}

void printTemp(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);
  if (tempC != DEVICE_DISCONNECTED)
  {
        Serial.print(tempC);
  }
  else Serial.print("DEVICE DISCONNECTED");
  Serial.print(" ");
}

void printAlarmInfo(DeviceAddress deviceAddress)
{
  char temp;
  printAddress(deviceAddress);
  temp = sensors.getHighAlarmTemp(deviceAddress);
  temp = sensors.getLowAlarmTemp(deviceAddress);
}
 
void Red(){  
  pinMode(ledR,OUTPUT);
  pinMode(ledG,OUTPUT);
  m1= analogRead(0);
  m2= analogRead(1);
  m3= analogRead(2);
  m4= analogRead(3);
  //float average= (m1 + m2 + m3 + m4) /4;
  float average= (m2 + m3 + m4) /3;
  digitalWrite(ledR, HIGH);
  digitalWrite(ledG, LOW);
  tone(3, 600 , 500);
}
 
void Green(){
  pinMode(ledG,OUTPUT);
  pinMode(ledR,OUTPUT);
  m1= analogRead(0);
  m2= analogRead(1);
  m3= analogRead(2);
  m4= analogRead(3);
  //float average= (m1 + m2 + m3 + m4) /4;
  float average= (m2 + m3 + m4) /3;
  digitalWrite(ledG, HIGH);
  digitalWrite(ledR, LOW); 
}

void Alarm()  { 
   sensors.requestTemperatures();
     if (sensors.hasAlarm())
     if (sensors.hasAlarm(insideThermometer)) {
   float tempC = sensors.getTempC(insideThermometer);
     if (tempC != DEVICE_DISCONNECTED) {
     if (tempC >= sensors.getHighAlarmTemp(insideThermometer)) {
      digitalWrite(Relay, HIGH); 
      }   
     if (tempC <= sensors.getLowAlarmTemp(insideThermometer)) {
       digitalWrite(Relay, LOW);      
     }
    }
  }
}

void loop(){
time = millis();
time2 = time /3600000;
m1= analogRead(0);  // Paper Towels
m2= analogRead(1);  // Santaka
m3= analogRead(2);  // BhutJolokia
m4= analogRead(3);  // C.Reaper
sensors.requestTemperatures();
  Alarm();
  float average= (m2 + m3 + m4) /3;
  //float average = (m1 + m2 + m3 + m4) /4;
  if (average < 375) 
  {
  Red();
  }
  else
  {
  Green();
  }
  Serial.print(time2);
  Serial.print(",");   
  printCurrentTemp(insideThermometer);
  Serial.print(",");
  Serial.print(analogRead(3));
  Serial.print(",");
  Serial.print(analogRead(1));
  Serial.print(",");
  Serial.print(analogRead(2));
  Serial.print(",");
  Serial.println(analogRead(0));
 delay(10000);
}

  
