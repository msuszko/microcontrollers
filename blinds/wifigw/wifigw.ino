
/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial1
//#define DEBUG 1

#define NBLIND 8

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


char out_buff[5] = {0, 0, 0, 0, 0};
char in_buff[5] = {0, 0, 0, 0, 0};
int cmd_ptr;


BLYNK_CONNECTED() {
  Serial1.println("Blynk connected");
  for (int i=0;i<NBLIND;i++) {CC
    Serial.write('A'+i);
  }
}

BLYNK_WRITE_DEFAULT() {
  int i = request.pin;
  int v = param.asInt();
  if (v < 0) {
    v = 0;
  } else if (v > 999) {
    v = 999;
  }
  if ((i >= 0) & (i < NBLIND)) {
    sprintf(out_buff, "%c%03d", i+'a', v);
    Serial.write(out_buff);
    Serial.flush();
  }
  
  Serial1.print("Blynk write ");
  Serial1.print(i, DEC);
  Serial1.print(": ");
  Serial1.println(v, DEC);
  Serial1.println(out_buff);
}

BLYNK_READ_DEFAULT()
{
  int i = request.pin;      // Which exactly pin is handled?
  Serial1.print("Blynk read ");
  Serial1.println(i, DEC);
  Serial.write('A'+i);
}

BLYNK_APP_CONNECTED() {
  Serial1.println("Blynk app connected");
  for (int i=0;i<NBLIND;i++) {
    Serial.write('A'+i);
  }
}

void ser_cmd(int in_byte) {
  int b;
  if ((in_byte>='a') & (in_byte<=('a'+NBLIND))) {
    Serial1.print('>');
    Serial1.write(in_byte);
    
    in_buff[0] = in_byte;
    cmd_ptr = 1;
/*  } else if (in_byte>='A' & in_byte<=('A'+NBLIND)) {
    report(&blinds[in_byte-'A']);
    cmd_ptr = 0; */
  } else if ((in_byte>='0') & (in_byte<='9') & (cmd_ptr > 0) & (cmd_ptr < 4)) {
    in_buff[cmd_ptr] = in_byte;
    cmd_ptr++;
  }
#if DEBUG
  Serial1.print("buff: ");
  Serial1.println(in_buff);
#endif
  if (cmd_ptr > 3) {
    if ((in_buff[0]>='a') & (in_buff[0]<=('a'+NBLIND))) {
      in_buff[4] = 0;
#if DEBUG
      Serial1.print("full buff: ");
      Serial1.println(in_buff);
#endif
      int new_pos = atoi(in_buff + 1);
#if DEBUG
      Serial1.print("new pos:");
      Serial1.println(new_pos, DEC);
#endif
      b = in_buff[0] - 'a';
      Blynk.virtualWrite(b, new_pos);
      Serial1.print("Blynk send ");
      Serial1.print(new_pos, DEC);
      Serial1.print(" to ");
      Serial1.println(b, DEC);
    }
    cmd_ptr = 0;
    for (uint i=0; i<sizeof(in_buff);i++) {
      in_buff[i] = 0;
    }
  }
}

void setup()
{
  // Debug console (TX only)
  Serial1.begin(115200);
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
