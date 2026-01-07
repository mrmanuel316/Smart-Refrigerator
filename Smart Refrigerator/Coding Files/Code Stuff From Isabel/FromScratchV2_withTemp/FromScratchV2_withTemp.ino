#include <TM1637Display.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// set pin numbers
#define CLOCKSET 36
#define HOUR_B 35
#define MINUTE_B 32
#define START_P 39
#define END_P 34
#define Active_ledPin 25
#define Passive_ledPin 33
#define CLK 19 
#define DIO 18
#define ONE_WIRE_BUS 21

// variable for storing the pushbutton status
TM1637Display display(CLK,DIO);
int CLOCKSET_State = 0;
int HOUR_B_State = 0;
int MINUTE_B_State = 0;
int START_P_State = 0;
int END_P_State = 0;
int hours = 0;
int minutes = 0;
bool setUP = false;
float temp = 0.0;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
Serial.begin(115200);
  sensors.begin();
  pinMode(HOUR_B, INPUT);
  pinMode(MINUTE_B, INPUT);
  pinMode(CLOCKSET, INPUT);
  pinMode(START_P, INPUT);
  pinMode(END_P, INPUT);
  pinMode(Active_ledPin, OUTPUT);
  pinMode(Passive_ledPin, OUTPUT);
  digitalWrite(Active_ledPin, LOW);
  digitalWrite(Passive_ledPin, LOW);

  display.setBrightness(0x0f);  
  updateDisplay();
}
void updateDisplay() {
  int displayTime = hours*100 + minutes;
  display.showNumberDecEx(displayTime, 0b01000000, true);

}
void loop() {
// read the state of the pushbutton value
CLOCKSET_State = digitalRead(CLOCKSET);
START_P_State = digitalRead(START_P);

if (CLOCKSET_State == HIGH) {
delay(300);
digitalWrite(Active_ledPin, HIGH);
delay(500);
digitalWrite(Active_ledPin, LOW);
setUP = !setUP;
updateDisplay();
}
if (START_P_State == HIGH) {
delay(300);
digitalWrite(Active_ledPin, HIGH);
delay(500);
digitalWrite(Active_ledPin, LOW);
sensors.requestTemperatures(); 
temp = sensors.getTempFByIndex(0);
int displayTemp = temp*100;
display.showNumberDecEx(displayTemp, 0b01000000, true);
}

if(!setUP){
  digitalWrite(Passive_ledPin, HIGH);
  HOUR_B_State = digitalRead(HOUR_B);
  MINUTE_B_State = digitalRead(MINUTE_B);

  if (HOUR_B_State == HIGH) {
      delay(200);
      hours = (hours + 1);
      updateDisplay();}
  if (MINUTE_B_State == HIGH) {
      delay(200);
      minutes = (minutes + 1);
      updateDisplay();
  if (CLOCKSET_State == HIGH) {
    setUP = false;
  }
  }
}


else {
// turn LED off
digitalWrite(Active_ledPin, LOW);
digitalWrite(Passive_ledPin, LOW);}
}