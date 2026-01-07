#include <TM1637Display.h>

#define CLK 19 //can be changed
#define DIO 18
#define CLOCKSET 36
#define HOUR_B 35
#define MINUTE_B 32
#define START_P 39
#define END_P 34
#define ledPin 25

TM1637Display display(CLK,DIO);

int hours = 0;
int minutes = 0;
bool setUP = false;
bool running = false;

unsigned long previousMillis = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(HOUR_B, INPUT_PULLUP);
  pinMode(MINUTE_B, INPUT_PULLUP);
  pinMode(CLOCKSET, INPUT_PULLUP);
  pinMode(START_P, INPUT_PULLUP);
  pinMode(END_P, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);

  display.setBrightness(0x0f);  
  updateDisplay();
}

void updateDisplay() {
  int displayTime = hours * 100 + minutes;
  display.showNumberDecEx(displayTime, 0b01000000, true);

}

void countDown() { //idk if this works or not
  //makes timer start counting down
  if (minutes > 0) {
    minutes--;
  } else if (hours > 0) {
    hours--;
    minutes = 59;
  } else {
    running = false;
    digitalWrite(ledPin, LOW);
  }
  
}

void loop() {
  if (digitalRead(CLOCKSET) == LOW) {
    delay(200);
    setUP = !setUP;
  }
  
  
  if (!setUP) {  
    if (digitalRead(HOUR_B) == LOW) {
      delay(200);
      hours = (hours + 1) % 24;
      updateDisplay();
    }

    if (digitalRead(MINUTE_B) == LOW) {
      delay(200);
      minutes = (minutes +1) % 60;
      if (minutes == 0) hours = (hours + 1) % 24; 
      updateDisplay();
    }

    if (digitalRead(START_P) == LOW) {
      delay(200);
      running = true;
      setUP = false;
      digitalWrite(ledPin, HIGH);
    }

   
  }
  
  if (running) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= 60000) {
      previousMillis = currentMillis;
      countDown();
      updateDisplay();
    }
  }

  if (digitalRead(END_P) == LOW) {
    delay(200);
    //shut down the countdown
    running = false;
    hours = 0;
    minutes = 0;
    setUP = false;
    digitalWrite(ledPin, LOW);
    updateDisplay();
  }

}
