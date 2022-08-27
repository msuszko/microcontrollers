#define TASKER_MAX_TASKS 8
#include <Tasker.h>

#define STOP_DOWN 1
#define STOP_UP 2
#define DOWN 3
#define UP 4
#define TRAVEL_TIME 15000

const int in_pins[] = {10, 11, 12, A0, A4, A5};
const int out_pins[] = {9, 8, 7, 6, 5, 4, 3, 2};
const uint16_t top = 600;

Tasker tasker;
uint16_t button_delay[8];
int last_in[8];
int out_status[8];
int motor_dir[2];
int i, s;

void setup() {
  Serial.begin(19200);
  Serial.println("Input init");
  for (i=0;i<6;i++) {
    pinMode(in_pins[i], INPUT_PULLUP);
    last_in[i] = HIGH;
    button_delay[i] = 0;
  }
  Serial.println("Output init");
  for (i=0;i<8;i++) {
    pinMode(out_pins[i], OUTPUT);
    out_status[i] = HIGH;
    digitalWrite(out_pins[i], out_status[i]);
  }
  Serial.println("Motor init");
  set_motor(0, STOP_UP);
  set_motor(1, STOP_UP);
  Serial.println("Start");
}

void button_pressed(int pin) {
  Serial.print("Pressed ");
  Serial.println(pin+1);
  if (pin < 4) {
    toggle_output(pin);
  } else {
    toggle_motor(pin-4);
  }
}

void set_motor(int mid, int dir) {
  int up, dn;
  
  switch (dir) {
    case UP:
      Serial.print("Start motor up");
      up = LOW;
      dn = HIGH;
      break;
   case DOWN:
      Serial.print("Start motor down");
      up = HIGH;
      dn = LOW;
      break;
    default:
      Serial.print("Stop motor ");
      up = HIGH;
      dn = HIGH;
      break;
  }
  Serial.println(mid);
  motor_dir[mid] = dir;
  digitalWrite(out_pins[4+mid*2], up);
  digitalWrite(out_pins[5+mid*2], dn);
}

void stop_motor(int mid) {
  if (motor_dir[mid] == UP) {
    motor_dir[mid] = STOP_UP;
  } else {
    motor_dir[mid] = STOP_DOWN;
  }
  set_motor(mid, motor_dir[mid]);
}

void toggle_motor(int mid) {
  switch (motor_dir[mid]) {
    case STOP_DOWN:
      motor_dir[mid] = UP;
      tasker.setTimeout(stop_motor, TRAVEL_TIME, mid);
      break;
    case STOP_UP:
      motor_dir[mid] = DOWN;
      tasker.setTimeout(stop_motor, TRAVEL_TIME, mid);
      break;
    case DOWN:
      motor_dir[mid] = STOP_DOWN;
      tasker.cancel(stop_motor, mid);
      break;
    case UP:
      motor_dir[mid] = STOP_UP;
      tasker.cancel(stop_motor, mid);
      break;
  }
  set_motor(mid, motor_dir[mid]);
}

void toggle_output(int pin) {
  if (out_status[pin] == LOW) {
    out_status[pin] = HIGH;
  } else {
    out_status[pin] = LOW;
  }
  digitalWrite(out_pins[pin], out_status[pin]);
}

void loop()
{
  for (i=0;i<6;i++) {
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
          button_pressed(i);
        }
      }
    }
  }
  tasker.loop();
}

