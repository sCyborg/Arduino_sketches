#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 5

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// arrays to hold device addresses
DeviceAddress insideThermometer;
int Relay = 2;

// function that will be called when an alarm condition exists during DallasTemperatures::processAlarms();
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

void setup(void)
{
  pinMode(Relay, OUTPUT);
  
  // start serial port
  Serial.begin(9600);
  Serial.println("Dallas Temperature IC Control Library Demo");

  // Start up the library
  sensors.begin();
  
  // locate devices on the bus
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");

  // search for devices on the bus and assign based on an index
  if (!sensors.getAddress(insideThermometer, 0)) Serial.println("Unable to find address for Device 0"); 

  Serial.print("Device insideThermometer ");
  printAlarmInfo(insideThermometer);
  Serial.println();

  // set the resolution to 9 bit (Each Dallas/Maxim device is capable of several different resolutions)
  sensors.setResolution(insideThermometer, 12);
 
  Serial.print("Device insideThermometer Resolution ");
  Serial.print(sensors.getResolution(insideThermometer), DEC); 
  Serial.println();

  // set alarm ranges 30c (27c) to 32c (29c)
  // Growing fase use 27c (24c) to 31c (28c)
  Serial.println("Setting alarm temps...");
  sensors.setHighAlarmTemp(insideThermometer, 32);
  sensors.setLowAlarmTemp(insideThermometer, 30);

  Serial.print("New insideThermometer ");
  printAlarmInfo(insideThermometer);
  Serial.println();

  // attach alarm handler
  sensors.setAlarmHandler(&newAlarmHandler);
   
  // Explain Temperature reading differences. 
  Serial.println("~28c on control device is ~25c | 33c on control is ~30c");
}

void loop(void)
{ 
  // ask the devices to measure the temperature
  sensors.requestTemperatures();
  printCurrentTemp(insideThermometer);
  Serial.print(" substract ~3.06c to get the realtemp.");
  Serial.println();
  delay(30000);
  // if an alarm condition exists as a result of the most recent 
  // requestTemperatures() request, it exists until the next time 
  // requestTemperatures() is called AND there isn't an alarm condition
  // on the device
  if (sensors.hasAlarm())
    // Sensor 1 Relay
    if (sensors.hasAlarm(insideThermometer)) {
    float tempC = sensors.getTempC(insideThermometer);
    if (tempC != DEVICE_DISCONNECTED) {
      // HIGH alarm  
      if (tempC >= sensors.getHighAlarmTemp(insideThermometer)) {
          digitalWrite(Relay, HIGH); 
          Serial.println("Temperature to HIGH heat off.");
          printCurrentTemp(insideThermometer);
          delay(60000);
          // HIGH alarm
       }   
       // LOW alarm
        if (tempC <= sensors.getLowAlarmTemp(insideThermometer)) {
          digitalWrite(Relay, LOW);
          Serial.println("Temperature to LOW heat on.");
          printCurrentTemp(insideThermometer);
          delay(60000);
        // LOW alarm
        }
    }
}
}

