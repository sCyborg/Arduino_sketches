// This is based on a demo of the RBBB running as webserver with the Ether Card
// 2010-05-28 <jc@wippler.nl> http://opensource.org/licenses/mit-license.php

//LIBARYS
#include <Wire.h>
#include <RTClib.h>
#include <SD.h>
#include <EtherCard.h>
#include <DHT.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP183.h>
#include <OneWire.h>
#include <DallasTemperature.h>

//PINS
#define DHTPIN 2
#define BMP183_CLK  44  // clk
#define BMP183_SDO  45  // AKA MISO
#define BMP183_SDI  46  // AKA MOSI
#define BMP183_CS   47  // cslave
#define ONE_WIRE_BUS 3 

//DEFINITIONS
#define STATIC 0  // set to 1 to disable DHCP (adjust myip/gwip values below)
#define DHTTYPE DHT11   // DHT 11 
DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP183 bmp = Adafruit_BMP183(BMP183_CLK, BMP183_SDO, BMP183_SDI, BMP183_CS);
const int chipSelect = 41;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

#if STATIC
// ethernet interface ip address
static byte myip[] = { 192,168,178,200 };
// gateway ip address
static byte gwip[] = { 192,168,178,1 };
#endif

// ethernet interface mac address, must be unique on the LAN
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };

byte Ethernet::buffer[500];
BufferFiller bfill;

// Global Variables Defined
// This is so we can see them in all functions
// t is the temperature, h is the humidity
// Timer is used to reduce the frequency of reading the DHT11 - I found reading it every loop
// caused timing issues with the Ethernet.
int lightPin = A0;   // Lightsensor at analog 1
//Rainsensor
int sensorRReading = A1;    // Rain sensor at analog 2

int td=0;                   // Temp DHT11
int hd=0;                   // Humidity DHT11
int Timer= 0;
int p=0;                    // Pressure BMP183
int bt=0;                   // Temperature BMP183  
int t2=0;
int t3=0;

DeviceAddress insideThermometer = { 0x28, 0xFA, 0xFE, 0xDB, 0x04, 0x00, 0x00, 0xAC  };

void setup () {
  Serial.begin(57600);
  bmp.begin();
  dht.begin();
  sensors.begin();
  //set sensor resolution
 sensors.setResolution(insideThermometer, 10);
  // see if the card is present and can be initialized:
  if (!SD.begin(38, 39, 40, 41)) 
  Serial.println("card initialized.");
    if (ether.begin(sizeof Ethernet::buffer, mymac,53) == 0)
    Serial.println(F("Failed to access Ethernet controller"));
    Serial.println("BMP183 Pressure Sensor Test"); Serial.println("");
#if STATIC
  ether.staticSetup(myip, gwip);
#else
  if (!ether.dhcpSetup())
    Serial.println("DHCP failed");
#endif

  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip);  
  ether.printIp("DNS: ", ether.dnsip); 
}
//  Here we get values from the DHT11
static void ReadDHT11()
{
  hd = dht.readHumidity();
  td = dht.readTemperature();
}
float printTemperature(DeviceAddress deviceAddress)
{
 float tempC = sensors.getTempC(deviceAddress);
 if (tempC == -127.00) {
   Serial.print("Error, sensor does not reply");
 } else {
   Serial.print("C: ");
   Serial.print(tempC);
 return(tempC);
  }
}
//  Here we build a web page and pass the t and h values into it
static word homePage() {
  //ReadBMP183();
  //p = bmp.getPressure();
  //bt = bmp.getTemperature();
  //ReadDHT11();
 long t = millis() / 1000;
 word h = t / 3600;
 byte m = (t / 60) % 60;
 byte s = t % 60;  
  sensors.requestTemperatures();
  sensors.getTempCByIndex(0);
  word sensorValue = analogRead(lightPin);
  word sensorRValue = analogRead(sensorRReading);
  bfill = ether.tcpOffset();
  bfill.emit_p(PSTR(
    "HTTP/1.0 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "Pragma: no-cache\r\n"
    "\r\n"
    "<meta http-equiv='refresh' content='10'/>"
    "<html><head><title>Real-time Weather-Station</title></head>" 
    "<body align= 'center' bgcolor='#FFF8DC'>"
    "<h1>Temp: $D C <br>Temp2: $T C <br>Humidity: $D % <br>Light: $D <br> Rain: $D</h1><h2>Uptime: $D$D:$D$D:$D$D (hh:mm:ss)</h2><h3>For rain sensor the folowing counts: <br> High (680) = Dry<br> Low = Wet.</h3>"
    "</body></html>"
    ), td ,printTemperature(insideThermometer), hd, sensorValue ,  sensorRValue, h/10, h%10, m/10, m%10, s/10, s%10);
  return bfill.position();
}
void loop () {
  word len = ether.packetReceive();
  word pos = ether.packetLoop(len); 
  if (pos)  // check if valid tcp data is received
  {
    ReadDHT11();
    ether.httpServerReply(homePage()); // send web page data
    printTemperature(insideThermometer);
    sensors.requestTemperatures();
    sensors.getTempCByIndex(0);
  } 
}


