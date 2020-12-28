//AutoWand Master v1.0
//Evan Schmitt
//2020

#include <SPI.h>
#include <SparkFunDS3234RTC.h>
#include <Dusk2Dawn.h>

#define HWSERIAL Serial1
#define sw1Pin 16
#define sw2Pin 17
#define DS13074_CS_PIN 10 // DeadOn RTC Chip-select pin


int switch1State;
int switch2State;

void setup () {

  pinMode(sw1Pin, INPUT_PULLUP);
  pinMode(sw2Pin, INPUT_PULLUP);

  Serial.begin(57600);
  HWSERIAL.begin(9600);

  // Call rtc.begin([cs]) to initialize the library
  // The chip-select pin should be sent as the only parameter
  rtc.begin(DS13074_CS_PIN);
  //rtc.set12Hour(); // Use rtc.set12Hour to set to 12-hour mode

  // Now set the time...
  // You can use the autoTime() function to set the RTC's clock and
  // date to the compiler's predefined time. (It'll be a few seconds
  // behind, but close!)
  // rtc.autoTime();
  // Or you can use the rtc.setTime(s, m, h, day, date, month, year)
  // function to explicitly set the time:
  // e.g. 7:32:16 | Monday October 31, 2016:
  rtc.setTime(5, 53, 19, 1, 27, 12, 20);  // Uncomment to manually set time

  rtc.update(); // Update time/date values
}


void loop () {
  rtc.update();  // Update time/date values
  Dusk2Dawn belford(40.408146, -74.091941, -5);
  int sunrise  = belford.sunrise(rtc.year(), rtc.month(), rtc.date(), false);
  int sunset   = belford.sunset(rtc.year(), rtc.month(), rtc.date(), false);
  switch1State = digitalRead(sw1Pin);
  switch2State = digitalRead(sw2Pin);

  if ((rtc.hour() * 60) + rtc.minute() == sunrise - 5)
  {
    HWSERIAL.write("1");
    delay(15000);
  }
  else if (((rtc.hour() * 60) + rtc.minute()) == sunset + 25
          )
  {
    HWSERIAL.write("2");
    delay(15000);
  }
  else if (switch1State == LOW)
  {
    HWSERIAL.write("3");
    delay(200);
  }
  else if (switch2State == LOW)
  {
    HWSERIAL.write("4");
    delay(200);
  }
/*
  Serial.print(rtc.year(), DEC);
  Serial.print('/');
  Serial.print(rtc.month(), DEC);
  Serial.print('/');
  Serial.print(rtc.date(), DEC);
  Serial.print(", ");
  Serial.print(rtc.hour(), DEC);
  Serial.print(':');
  Serial.print(rtc.minute(), DEC);
  Serial.print(':');
  Serial.print(rtc.second(), DEC);
  Serial.println();
  Serial.println(sunrise);
  */
  delay(100);
}
