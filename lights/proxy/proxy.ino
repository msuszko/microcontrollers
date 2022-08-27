
/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial1

#define NPIN 10

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

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

int i, v, l;

BLYNK_CONNECTED() {
  Serial1.println("Blynk connected");
  Serial.write('?');
}

BLYNK_WRITE_DEFAULT() {
  i = request.pin;
  v = param.asInt();
  if (i >= 0 & i < NPIN) {
    if (v == 0) {
      Serial.write('a'+i);
    } else {
      Serial.write('A'+i);
    }
  }
  
  Serial1.print("Blynk write ");
  Serial1.print(i, DEC);
  Serial1.print(": ");
  Serial1.print(v, DEC);
  Serial1.println("");
}

BLYNK_READ_DEFAULT()
{
  i = request.pin;      // Which exactly pin is handled?
  Blynk.virtualWrite(i, 0);
  Serial1.print("Blynk read ");
  Serial1.print(i, DEC);
  Serial1.println("");
  Serial.write('?');
}

BLYNK_APP_CONNECTED() {
  Serial1.println("Blynk app connected");
  Serial.write('?');
}

void ser_cmd(int cmd) {
  Serial1.print("Rcv: ");
  Serial1.write(cmd);
  Serial1.println("");
  if (cmd>='a' & cmd<=('a'+NPIN)) {
    i = cmd-'a';
    Blynk.virtualWrite(i, 0);
  } else if (cmd>='A' & cmd<=('A'+NPIN)) {
    i = cmd-'A';
    Blynk.virtualWrite(i, 1);
  }
}

void setup()
{
  // Debug console (TX only)
  Serial1.begin(9600);
  Serial1.setDebugOutput(true);
  Serial.begin(115200);
  // use alternative pins (D7 D8)
  Serial.swap();

  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  while (Serial.available()>0) {
    ser_cmd(Serial.read());
  }
  Blynk.run();
}
