// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"
#include <Dusk2Dawn.h>

#define HWSERIAL Serial1

const int swPin = 13;
int switchState = 0;

RTC_PCF8523 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup () {

  pinMode(swPin, INPUT);
  Serial.begin(57600);
  Serial1.begin(9600);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  // This line sets the RTC with an explicit date & time
  // rtc.adjust(DateTime(2020, 11, 20, 0, 28, 0));
  // following line sets the RTC to the date & time this sketch was compiled
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}


void loop () {
  Dusk2Dawn belford(40.408146, -74.091941, -5);
  DateTime now = rtc.now();
  int sunrise  = belford.sunrise(now.year(), now.month(), now.day(), false);
  int sunset   = belford.sunset(now.year(), now.month(), now.day(), false);
  switchState = digitalRead(swPin);

  if ((now.hour() * 60) + now.minute() == sunrise - 5)
  {
    Serial1.write("1");
  }
  else if (((now.hour() * 60) + now.minute()) == sunset + 15)
  {
    Serial1.write("2");
  }
  else if (switchState == HIGH)
  {
    Serial1.write("2");
  }
  //Serial.print(now.year(), DEC);
  //Serial.print('/');
  //Serial.print(now.month(), DEC);
  //Serial.print('/');
  //Serial.print(now.day(), DEC);
  //Serial.print(" (");
  //Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  //Serial.print(") ");
  //Serial.print(now.hour(), DEC);
  //Serial.print(':');
  //Serial.print(now.minute(), DEC);
  //Serial.print(':');
  //Serial.print(now.second(), DEC);
  //Serial.println();
  //Serial.println(sunrise);
  delay(100);
}
