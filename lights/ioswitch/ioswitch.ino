#define NPIN 10
const int in_pins[] =  {PA0,  PA1,  PA2,  PA3,  PA4, PA5, PA6,  PA7, PB0, PB1};
const int out_pins[] = {PB12, PB13, PB14, PB15, PA8, PA9, PA10, PB6, PB7, PB8};
const uint16_t top = 60000;

uint8_t button_delay[NPIN];
int last_in[NPIN];
int out_status[NPIN];
int i, s, in_byte;

void reset() {
  for (i=0;i<NPIN;i++) {
    pinMode(in_pins[i], INPUT_PULLUP);
    pinMode(out_pins[i], OUTPUT);
    last_in[i] = HIGH;
    out_status[i] = HIGH;
    button_delay[i] = 0;
    Serial3.write(i+'a');
    digitalWrite(out_pins[i], out_status[i]);
    Serial3.flush();
  }
}


void setup() {
  Serial3.begin(115200);
  Serial3.write(">>>\n");
  reset();
}

void toggle_output(int pin) {
  if (out_status[pin] == LOW) {
    out_status[pin] = HIGH;
    Serial3.write(pin+'a');
  } else {
    out_status[pin] = LOW;
    Serial3.write(pin+'A');
  }
  digitalWrite(out_pins[pin], out_status[pin]);
  Serial3.flush();
}

void loop()
{
  if (Serial3.available()) {
    in_byte = Serial3.read();
    if (in_byte>='a' & in_byte<=('a'+NPIN)) {
      i = in_byte-'a';
      if (out_status[i] == LOW) {
        toggle_output(i);
      }
    } else if (in_byte>='A' & in_byte<=('A'+NPIN)) {
      i = in_byte-'A';
      if (out_status[i] == HIGH) {
        toggle_output(i);
      }
    } else if (in_byte == '?') {
      for (i=0;i<NPIN;i++) {
        if (out_status[i] == LOW) {
          Serial3.write(i+'A');
        } else {
          Serial3.write(i+'a');
        }
        Serial3.flush();
      }
    } else if (in_byte == '!') {
      reset();
    }
  }
  for (i=0;i<8;i++) {
    s = digitalRead(in_pins[i]);
    if (s != last_in[i]) {
      last_in[i] = s;
      button_delay[i] = top;
    } else {
      if (s == LOW) {
        if (button_delay[i] > 0) {
          button_delay[i] -= 1;
        }
        if (button_delay[i] == 1) {
          toggle_output(i);
        }
      }
    }
  }
}
