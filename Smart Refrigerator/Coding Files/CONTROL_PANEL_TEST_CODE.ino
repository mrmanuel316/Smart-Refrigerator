#include <TM1637Display.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include "RTClib.h"

#define ONE_WIRE_BUS 16//temp sens
#define CLK 22//seven segment pins
#define DIO 23
#define CLOCKSET 27//buttons
#define HOUR_B 13
#define MINUTE_B 14
#define START_P 19
#define END_P 4
#define Pass_Pin 32 //PASSIVE LED
#define Act_Pin 33 //ACTIVE LED
#define RelayFan 15 // FAN
#define RelayCom 2 //Compressor

RTC_DS3231 rtc;

TM1637Display display(CLK,DIO);
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

void setup() {
  // put your setup code here, to run once:
  pinMode(HOUR_B, INPUT_PULLUP);
  pinMode(MINUTE_B, INPUT_PULLUP);
  pinMode(CLOCKSET, INPUT_PULLUP);
  pinMode(START_P, INPUT_PULLUP);
  pinMode(END_P, INPUT_PULLUP);
  pinMode(Pass_Pin, OUTPUT);
  pinMode(Act_Pin, OUTPUT);
  pinMode(RelayFan, OUTPUT);
  pinMode(RelayCom, OUTPUT);

  Serial.begin(9600); //FOR DEBUG SERIAL
  sensors.begin();
  display.setBrightness(0x0f);  
  updateDisplay();

  Wire.begin(18, 21);  // SDA=18, SCL=21
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
}

void updateDisplay() {
  int displayTime = hours * 100 + minutes; //For time display
  display.showNumberDecEx(displayTime, 0b01000000, true);

}

void countDown() { 
  //makes timer start counting down
  if (minutes > 0) {
    minutes--;
  } else if (hours > 0) {
    hours--;
    minutes = 59;//decreases hours and turns minutes into 59
  } else {
    running = false;
    digitalWrite(Pass_Pin, LOW);
    digitalWrite(Act_Pin, HIGH);
    digitalWrite(RelayFan, LOW); //TURN OFF RELAY
    digitalWrite(RelayCom, HIGH); // TURN ON COMPRESSOR
  }
  
}

void toggleFan() {
  sensors.requestTemperatures(); 
  temp = sensors.getTempFByIndex(0);
  if (digitalRead(RelayCom) == HIGH) return; // skip fan control if compressor is on
  if (temp >= 41.0) { //DEFAULT 41
    //Serial.println(temp); //REMOVE ON FINAL DRAFT
    delay(200);
    digitalWrite(RelayFan, HIGH); //TURN ON RELAY
  } else if (temp <= 38.0) { //DEFAULT 38
    //Serial.println(temp); //REMOVE ON FINAL DRAFT
    delay(200);
    digitalWrite(RelayFan, LOW); //TURN OFF
  }
}

void flashTemperature() {
  sensors.requestTemperatures();
  float t = sensors.getTempFByIndex(0);
  int tempInt = (int)t;

  unsigned long startTime = millis();

  // Flash temperature for 3 seconds
  while (millis() - startTime < 3000) {
    display.showNumberDec(tempInt, true);   // Show temp
    delay(500);
    display.clear();                        // Blank display
    delay(500);
  }

  updateDisplay(); // Return to timer display
}

void loop() {   
  DateTime now = rtc.now();
  int currentHour = now.hour();
  int currentMinute = now.minute(); 

  if (now.minute() != lastMinute) {
    autoOverride = false;
    lastMinute = now.minute();
  } 

  //Serial.print("    Time: ");//REMOVE FOR FINAL
  //Serial.print(now.hour(), DEC);
  //erial.print(':');
  //Serial.print(now.minute(), DEC);
  //Serial.print(':');
  //Serial.print(now.second(), DEC);
  //Serial.println();
  //delay(1000); // Update every second

  if (digitalRead(CLOCKSET) == LOW) {
    delay(200); 
    flashTemperature();  // show temp for 3 seconds
  }

  if (!autoOverride && !countdownTrigger && !running && currentHour == 17 && currentMinute == 0) { //DEFAULT HOUR 17 MINUTE 0
    hours = 1; //change to 3
    minutes = 0;
    running = true; 
    countdownTrigger = true;
    digitalWrite(Pass_Pin, HIGH);
    digitalWrite(Act_Pin, LOW);
    digitalWrite(RelayCom, LOW); //TURN OFF COMPRESSOR
  } else if (!running) {
    // Outside auto window
    digitalWrite(Pass_Pin, LOW);
    digitalWrite(Act_Pin, HIGH);
    digitalWrite(RelayFan, LOW);  // FAN OFF
    digitalWrite(RelayCom, HIGH);   // COMPRESSOR ON
  }
  
  if(!running) countdownTrigger = false;
  
  if (digitalRead(HOUR_B) == LOW) {
    delay(200);
    hours = (hours + 1) % 24; //if hours reach 24 it rolls over back to 0
    updateDisplay();
  }

  if (digitalRead(MINUTE_B) == LOW) {
    delay(200);
    minutes = (minutes +1) % 60; //if minutes reach 60, rolls over back to 0
    if (minutes == 0) hours = (hours + 1) % 24; //and hours increase by 1
    updateDisplay();
  }

  if (digitalRead(START_P) == LOW) {
    delay(200);
    running = true; 
    digitalWrite(Pass_Pin, HIGH);
    digitalWrite(Act_Pin, LOW);
    digitalWrite(RelayCom, LOW); //TURN OFF COMPRESSOR
    
  }
  if (digitalRead(END_P) == LOW) {
    delay(200);
    //shut down the countdown
    autoOverride = true;
    running = false;
    hours = 0;
    minutes = 0;
    digitalWrite(Pass_Pin, LOW);
    digitalWrite(Act_Pin, HIGH);
    digitalWrite(RelayFan, LOW); //TURN OFF RELAY
    digitalWrite(RelayCom, HIGH); //TURN ON COMPRESSOR
    updateDisplay();
  }

  if (running) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= 1000) { ///switch to 1000 for testing(1second) DEFAULT 60000(1minute)
      previousMillis = currentMillis;
      countDown();
      updateDisplay();
    }
    toggleFan();
  } else {
    digitalWrite(RelayFan, LOW);
    digitalWrite(RelayCom, HIGH); //TURN ON COMPRESSOR
  }

}
