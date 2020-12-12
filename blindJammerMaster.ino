//AutoWand Master v0.3
//Evan Schmitt
//2020

#include <Wire.h>
#include <RTClib.h>
#include <Dusk2Dawn.h>

#define HWSERIAL Serial1
#define sw1Pin 16;
#define sw2Pin 17;

int switch1State;
int switch2State;

RTC_PCF8523 rtc;

void setup () {

  pinMode(sw1Pin, INPUT_PULLUP);
  pinMode(sw2Pin, INPUT_PULLUP);
  
  Serial.begin(57600);
  HWSERIAL.begin(9600);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  Dusk2Dawn belford(40.408146, -74.091941, -5);

  // This line sets the RTC with an explicit date & time
  // rtc.adjust(DateTime(2020, 11, 20, 0, 28, 0));
  // following line sets the RTC to the date & time this sketch was compiled
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}


void loop () {
  DateTime now = rtc.now();
  int sunrise  = belford.sunrise(now.year(), now.month(), now.day(), false);
  int sunset   = belford.sunset(now.year(), now.month(), now.day(), false);
  switchState = digitalRead(swPin);

  if ((now.hour() * 60) + now.minute() == sunrise - 5)
  {
    HWSERIAL.write("1");
    delay(15000);
  }
  else if (((now.hour() * 60) + now.minute()) == sunset + 15)
  {
    HWSERIAL.write("2");
    delay(15000);
  }
  else if (switch1State == LOW)
  {
    HWSERIAL.write("3");
    delay(200);
  }
  else if (switch2state == LOW)
  {
    HWSERIAL.write("4");
    delay(200);
  }
  
  //Serial.print(now.year(), DEC);
  //Serial.print('/');
  //Serial.print(now.month(), DEC);
  //Serial.print('/');
  //Serial.print(now.day(), DEC);
  //Serial.print(", ");
  //Serial.print(now.hour(), DEC);
  //Serial.print(':');
  //Serial.print(now.minute(), DEC);
  //Serial.print(':');
  //Serial.print(now.second(), DEC);
  //Serial.println();
  //Serial.println(sunrise);
  delay(100);
}
