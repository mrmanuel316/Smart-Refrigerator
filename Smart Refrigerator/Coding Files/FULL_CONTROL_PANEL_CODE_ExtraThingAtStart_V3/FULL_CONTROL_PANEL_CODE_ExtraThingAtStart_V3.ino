#include <TM1637Display.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include "RTClib.h"

#define ONE_WIRE_BUS 16
#define CLK 22
#define DIO 23
#define CLOCKSET 27
#define HOUR_B 13
#define MINUTE_B 14
#define START_P 19
#define END_P 4
#define Pass_Pin 32
#define Act_Pin 33
#define RelayFan 15
#define RelayCom 2

RTC_DS3231 rtc;

TM1637Display display(CLK, DIO);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

int hours = 0;
int minutes = 0;
bool running = false;
bool countdownTrigger = false;
bool autoOverride = false;
int lastMinute = -1;
float temp = 0.0;
unsigned long previousMillis = 0;
bool fanArmed = true;

// ★ ADDED — helper that enforces 1-second delay between relay swaps
void safeRelaySwitch(int relayPin, bool newState) {
  bool oldState = digitalRead(relayPin);

  // No change needed
  if (oldState == newState) return;

  // If relay is turning OFF and another is ON, delay before switching
  if (oldState == HIGH && newState == LOW) {
    digitalWrite(relayPin, LOW);  
    delay(1000);  // 1-second interlock delay
  } else {
    // If turning ON, ensure safe timing
    delay(1000);  
    digitalWrite(relayPin, HIGH);
  }
}

void setup() {
  pinMode(HOUR_B, INPUT_PULLUP);
  pinMode(MINUTE_B, INPUT_PULLUP);
  pinMode(CLOCKSET, INPUT_PULLUP);
  pinMode(START_P, INPUT_PULLUP);
  pinMode(END_P, INPUT_PULLUP);
  pinMode(Pass_Pin, OUTPUT);
  pinMode(Act_Pin, OUTPUT);
  pinMode(RelayFan, OUTPUT);
  pinMode(RelayCom, OUTPUT);

  sensors.begin();
  display.setBrightness(0x0f);
  updateDisplay();

  Wire.begin(18, 21);
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
}

void updateDisplay() {
  int displayTime = hours * 100 + minutes;
  display.showNumberDecEx(displayTime, 0b01000000, true);
}

void countDown() { 
  if (minutes > 0) {
    minutes--;
  } else if (hours > 0) {
    hours--;
    minutes = 59;
  } else {
    running = false;
    digitalWrite(Pass_Pin, LOW);
    digitalWrite(Act_Pin, HIGH);

    // ★ Use safe switching for relays
    safeRelaySwitch(RelayFan, LOW);
    safeRelaySwitch(RelayCom, HIGH);
  }
}

void toggleFan() {
  sensors.requestTemperatures(); 
  temp = sensors.getTempFByIndex(0);

  if (digitalRead(RelayCom) == HIGH) {
    safeRelaySwitch(RelayFan, LOW);
    fanArmed = true;
    return;
  }

  if (fanArmed && temp >= 41.0) {
    safeRelaySwitch(RelayFan, HIGH);
    fanArmed = false;
  }

  if (!fanArmed && temp <= 37.0) {
    safeRelaySwitch(RelayFan, LOW);
    fanArmed = true;
  }
}

void flashTemperature() {
  sensors.requestTemperatures();
  float t = sensors.getTempFByIndex(0);
  int tempInt = (int)t;

  unsigned long startTime = millis();

  while (millis() - startTime < 3000) {
    display.showNumberDec(tempInt, true);
    delay(500);
    display.clear();
    delay(500);
  }

  updateDisplay();
}

void loop() {   
  DateTime now = rtc.now();
  int currentHour = now.hour();
  int currentMinute = now.minute();

  if (now.minute() != lastMinute) {
    autoOverride = false;
    lastMinute = now.minute();
  }

  if (digitalRead(CLOCKSET) == LOW) {
    delay(200);
    flashTemperature();
  }

  // AUTO START AT 12:00
  if (!autoOverride && !countdownTrigger && !running &&
      currentHour == 12 && currentMinute == 0) {

    hours = 3;
    minutes = 0;
    running = true;
    countdownTrigger = true;

    digitalWrite(Pass_Pin, HIGH);
    digitalWrite(Act_Pin, LOW);

    // ★ Use safe switching
    safeRelaySwitch(RelayCom, LOW);
  }

  // SYSTEM IDLE
  else if (!running) {
    digitalWrite(Pass_Pin, LOW);
    digitalWrite(Act_Pin, HIGH);

    safeRelaySwitch(RelayFan, LOW);
    safeRelaySwitch(RelayCom, HIGH);
  }

  if (!running) countdownTrigger = false;

  if (digitalRead(HOUR_B) == LOW) {
    delay(200);
    hours = (hours + 1) % 24;
    updateDisplay();
  }

  if (digitalRead(MINUTE_B) == LOW) {
    delay(200);
    minutes = (minutes + 1) % 60;
    if (minutes == 0) hours = (hours + 1) % 24;
    updateDisplay();
  }

  if (digitalRead(START_P) == LOW) {
    delay(200);
    running = true;
    digitalWrite(Pass_Pin, HIGH);
    digitalWrite(Act_Pin, LOW);
    safeRelaySwitch(RelayCom, LOW);
  }

  if (digitalRead(END_P) == LOW) {
    delay(200);
    autoOverride = true;
    running = false;
    hours = 0;
    minutes = 0;

    digitalWrite(Pass_Pin, LOW);
    digitalWrite(Act_Pin, HIGH);

    safeRelaySwitch(RelayFan, LOW);
    safeRelaySwitch(RelayCom, HIGH);

    fanArmed = true;
    updateDisplay();
  }

  if (running) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= 60000) {
      previousMillis = currentMillis;
      countDown();
      updateDisplay();
    }
    toggleFan();
  } else {
    safeRelaySwitch(RelayFan, LOW);
    safeRelaySwitch(RelayCom, HIGH);
    fanArmed = true;
  }
}
