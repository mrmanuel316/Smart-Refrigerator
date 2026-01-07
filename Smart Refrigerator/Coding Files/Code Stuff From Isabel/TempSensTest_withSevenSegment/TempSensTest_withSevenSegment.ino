#include <OneWire.h>
#include <DallasTemperature.h>
#include <TM1637Display.h>

// Data wire is conntec to the Arduino digital pin 4
#define ONE_WIRE_BUS 21
#define CLK 19 
#define DIO 18


TM1637Display display(CLK,DIO);
float temp = 0.0;
// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

void setup(void)
{
  // Start serial communication for debugging purposes
  Serial.begin(9600);
  // Start up the library
  sensors.begin();
  display.setBrightness(0x0f);  
  updateDisplay();
}
void updateDisplay() {
  int displayTemp = temp*100;
  display.showNumberDecEx(displayTemp, 0b01000000, true);

}
void loop(void){ 
  // Call sensors.requestTemperatures() to issue a global temperature and Requests to all devices on the bus
  sensors.requestTemperatures(); 
  temp = sensors.getTempFByIndex(0);
  updateDisplay();
  Serial.print("Celsius temperature: ");
  // Why "byIndex"? You can have more than one IC on the same bus. 0 refers to the first IC on the wire
  Serial.print(sensors.getTempCByIndex(0)); 
  Serial.print(" - Fahrenheit temperature: ");
  Serial.println(sensors.getTempFByIndex(0));
  delay(1000);
}