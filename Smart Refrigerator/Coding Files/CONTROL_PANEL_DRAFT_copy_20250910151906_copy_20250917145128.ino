#include <dummy.h>

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

void setup() {
  // put your setup code here, to run once:
  pinMode(HOUR_B, INPUT_PULLUP);
  pinMode(MINUTE_B, INPUT_PULLUP);
  pinMode(CLOCKSET, INPUT_PULLUP);
  pinMode(START_P, INPUT_PULLUP);
  pinMode(END_P, INPUT_PULLUP);

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
    delay(60000); //input time delay of a minute
    minutes--;
  } else {
    if (hours > 0) {
      delay(60000); //input time delay of a minute
      hours--;
      minutes = 59;
    } else {
      hours = 0;
      minutes = 0;
      setUP = false;
    }
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
      minutes++;
      if (minutes >= 60) {
        minutes = 0;
        hours = (hours + 1) % 24;
      }
      updateDisplay();
    }
    if (digitalRead(START_P) == LOW) {
      delay(200);
      countDown();
      updateDisplay();
    }

    if (digitalRead(END_P) == LOW) {
      delay(200);
      //shut down the countdown
      hours = 0;
      minutes = 0;
      setUP = false;
    }
  }
 


}
