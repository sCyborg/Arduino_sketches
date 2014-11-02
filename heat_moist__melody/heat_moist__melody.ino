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
    Serial.println(" Soil Moist check and Thermometer with Relay. ");

sensors.begin();

  Serial.print("..Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");

  if (!sensors.getAddress(insideThermometer, 0)) Serial.println("Unable to find address for Device 0"); 
  
  Serial.println(" ");
  Serial.print("Device insideThermometer ");
  Serial.println("Old alarm temps...");
  printAlarmInfo(insideThermometer);
  Serial.println(" ");
  
  sensors.setResolution(insideThermometer, 10);
 
  Serial.print("Device insideThermometer Resolution ");
  Serial.print(sensors.getResolution(insideThermometer), DEC); 
  Serial.println();

  Serial.println(" ");
  Serial.println("Setting new alarm temps...");
  sensors.setHighAlarmTemp(insideThermometer, 32);
  sensors.setLowAlarmTemp(insideThermometer, 30);
  Serial.print("New insideThermometer ");
  printAlarmInfo(insideThermometer);
  Serial.println();
  Serial.println(" ");

  sensors.setAlarmHandler(&newAlarmHandler);

  Serial.print("Moisture 1 (Paper Towels): ");
  Serial.print(analogRead(0));
  Serial.println();
  Serial.print("Moisture 2 (Santaka): ");
  Serial.print(analogRead(1));
  Serial.println();
  Serial.print("Moisture 3 (Bhut Jolokia): ");
  Serial.print(analogRead(2));
  Serial.println();
  Serial.print("Moisture 4 (Carolina Reaper): ");
  Serial.print(analogRead(3));
  Serial.println();
  Serial.println(" ");
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
  printAddress(deviceAddress);
  printTemp(deviceAddress);
  Serial.println();
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
    Serial.print("Current Temp C: ");
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
  Serial.print("High Alarm: ");
  Serial.print(temp, DEC);
  Serial.print("C");
  Serial.print(" Low Alarm: ");
  temp = sensors.getLowAlarmTemp(deviceAddress);
  Serial.print(temp, DEC);
  Serial.print("C");
  Serial.print(" ");
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
  Serial.print("Warning below treshold moisture Average = ");
  Serial.print(average);
  Serial.println();
  printCurrentTemp(insideThermometer);
  Serial.println();
  digitalWrite(ledR, HIGH);
  digitalWrite(ledG, LOW);
  tone(3, 600 , 500);
  if (m1 < 120){
  Serial.print("Moisture 1 below average check (paper Towels): ");
  Serial.print(analogRead(0));
  Serial.println();
  }
  if (m2 < 250){
  Serial.print("Moisture 2 below average check (Santaka): ");
  Serial.print(analogRead(1));
  Serial.println();
  }
  if (m3 < 250){
  Serial.print("Moisture 3 below average check (Bhut Jolokia): ");
  Serial.print(analogRead(2));
  Serial.println();
  }
  if (m4 < 250){
  Serial.print("Moisture 4 below average check (Carolina Reaper: ");
  Serial.print(analogRead(3));
  Serial.println();
  }
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
  Serial.print("Moisture Average: ");
  Serial.print(average);
  Serial.println();
  printCurrentTemp(insideThermometer);
  Serial.println();
  digitalWrite(ledG, HIGH);
  digitalWrite(ledR, LOW); 
  if (m1 < 175){
  Serial.print("Moisture 1 below average check (paper Towels): ");
  Serial.print(analogRead(0));
  Serial.println();
  }
  if (m2 < 325){
  Serial.print("Moisture 2 below average check (Santaka): ");
  Serial.print(analogRead(1));
  Serial.println();
  }
  if (m3 < 325){
  Serial.print("Moisture 3 below average check (Bhut Jolokia): ");
  Serial.print(analogRead(2));
  Serial.println();
  }
  if (m4 < 325){
  Serial.print("Moisture 4 below average check (Carolina Reaper: ");
  Serial.print(analogRead(3));
  Serial.println();
  }
}

void Alarm()  { 
   sensors.requestTemperatures();
     if (sensors.hasAlarm())
     if (sensors.hasAlarm(insideThermometer)) {
   float tempC = sensors.getTempC(insideThermometer);
     if (tempC != DEVICE_DISCONNECTED) {
     if (tempC >= sensors.getHighAlarmTemp(insideThermometer)) {
      digitalWrite(Relay, HIGH); 
      Serial.println("Temperature is to High (switching) heating element (is) off.");
     }   
     if (tempC <= sensors.getLowAlarmTemp(insideThermometer)) {
       digitalWrite(Relay, LOW);
       Serial.println("Temperature is to Low (switching) heating element (is) on.");          
     }
    }
  }
}
void loop(){
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
delay(60000);
}

  
