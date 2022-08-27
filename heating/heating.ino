
/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <SimpleTimer.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#if 1
#include "secrets.h"
#else
/*
// Blynk API key
char auth[] = "01234567890abcdef01234567890abcd";

// Your WiFi credentials.
// Set password to "" for open networks.
const char ssid[] = "yournetwork";
const char pass[] = "";
#endif

#define ONE_WIRE_BUS 3        // This is the ESP8266 pin
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

DeviceAddress devaddr_mixed = { 0x28, 0x3C, 0x06, 0xE0, 0x00, 0x00, 0x00, 0x5B };
DeviceAddress devaddr_hot = { 0x28, 0x6C, 0x4D, 0xDC, 0x00, 0x00, 0x00, 0xB6 };
DeviceAddress devaddr_cool = { 0x28, 0x4B, 0x69, 0xE0, 0x00, 0x00, 0x00, 0x38 };
float temp_mixed, temp_hot, temp_cool;

SimpleTimer timer;

// This function will be called every time Slider Widget
// in Blynk app writes values to the Virtual Pin 1
BLYNK_WRITE(V1)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  // You can also use:
  // String i = param.asStr();
  // double d = param.asDouble();
  Serial.print("V1 Slider value is: ");
  Serial.println(pinValue);
}


void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);
  while (Blynk.connect() == false) {
    // Wait until connected
  }
  sensors.begin();
  sensors.setResolution(devaddr_mixed, 10);
  timer.setInterval(2000L, send_temp);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
}

void loop()
{
  Blynk.run();
  timer.run();
}

void send_temp() {
  sensors.requestTemperatures();
  temp_mixed = sensors.getTempC(devaddr_mixed);
  Blynk.virtualWrite(V2, temp_mixed);
}
