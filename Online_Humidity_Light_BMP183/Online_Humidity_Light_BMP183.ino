// This is a demo of the RBBB running as webserver with the Ether Card
// 2010-05-28 <jc@wippler.nl> http://opensource.org/licenses/mit-license.php

//LIBARYS

#include <EtherCard.h>
#include <DHT.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP183.h>
//PINS
#define DHTPIN 2
#define BMP183_CLK  44  // clk
#define BMP183_SDO  45  // AKA MISO
#define BMP183_SDI  46  // AKA MOSI
#define BMP183_CS   47  // cslave

//DEFINITIONS
#define STATIC 0  // set to 1 to disable DHCP (adjust myip/gwip values below)
#define DHTTYPE DHT11   // DHT 11 

DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP183 bmp = Adafruit_BMP183(BMP183_CLK, BMP183_SDO, BMP183_SDI, BMP183_CS);

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
int lightPin = A0;

int t=0;
int h=0;
int Timer= 0;

void setup () {
  Serial.begin(57600);
  bmp.begin();
  dht.begin();
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
  h = dht.readHumidity();
  t = dht.readTemperature();
}

//  Here we build a web page and pass the t and h values into it
static word homePage() {
  //ReadBMP183();
  word barTemp = bmp.getTemperature();
  word barPressure = bmp.getPressure();
  //ReadDHT11();
  word sensorValue = analogRead(lightPin); 
  bfill = ether.tcpOffset();
  bfill.emit_p(PSTR(
    "HTTP/1.0 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "Pragma: no-cache\r\n"
    "\r\n"
    "<meta http-equiv='refresh' content='30'/>"
    "<title>Real-time Weather</title>" 
    "<h1>Temp: $D C <br>Humidity: $D % <br>Light: $D <br>Pressure: &D <br>Temp(baro): &D C </h1>"),
  t, h, sensorValue , digitalRead(lightPin), barPressure, digitalRead(barPressure), barTemp, digitalRead(barTemp));
  return bfill.position();
}

void loop () {
  word len = ether.packetReceive();
  word pos = ether.packetLoop(len);
  
  if (pos)  // check if valid tcp data is received
  {
    ReadDHT11();
    ether.httpServerReply(homePage()); // send web page data
  }
}

