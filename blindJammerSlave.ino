// blindJammer slave v0.4
// eschmitt
// 2020

#include <AccelStepper.h>
#include <EEPROM.h>

#define HWSERIAL Serial1
#define ledPin 16
#define calSwPin 21
#define potPin A6   // pin 20 on Teensy LC

long potVal = 0;
long potOldVal = 0;
long o;
long c;
int incomingByte = 0;
int calSwState = 0;
int ledPwm = 0;
unsigned int posAddress = 10;   //EEPROM memory location to store stepper motor last known location

// Define a stepper and the pins it will use
AccelStepper stepper(1, 15, 14); // type, step, dir


void setup()
{
  //Serial.begin(9600);
  HWSERIAL.begin(9600);
  pinMode(potPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(calSwPin, INPUT_PULLUP);
  stepper.setCurrentPosition(EEPROM.get( posAddress, o ));   //set current position to last know position

  //populate potOldVal
  potVal = map(analogRead(potPin), 0, 1023, 1150, -1150);
  potOldVal = potVal;

}

void sunset()     //sunset action, close blinds slowly
{
  stepper.moveTo(1200);
  stepper.setMaxSpeed(2);
  stepper.setAcceleration(50);
}

void sunrise()    //sunrise action, open blinds slowly
{
  stepper.moveTo(0);
  stepper.setMaxSpeed(2);
  stepper.setAcceleration(50);
}

void openBlind()
{
  stepper.moveTo(0);
  stepper.setMaxSpeed(500);
  stepper.setAcceleration(250);
}

void closeBlind()
{
  stepper.moveTo(1150);
  stepper.setMaxSpeed(500);
  stepper.setAcceleration(250);
}

void checkPot()  //check the potentiometer position and set motor position
{
  potVal = map(analogRead(potPin), 0, 1023, 1150, -1150);
  if (potVal > (potOldVal + 40) || potVal < (potOldVal - 40)) {  // pot smoothing
    stepper.moveTo(potVal);
    stepper.setMaxSpeed(5000);
    stepper.setAcceleration(7000);
    potOldVal = potVal;
  }
}

void checkMaster()   //check for commands from master via XBee
{
  if (HWSERIAL.available() > 0) {
    incomingByte = HWSERIAL.read();
  }
  if (incomingByte == 49) {
    sunrise();
    incomingByte = 0;
  }
  else if (incomingByte == 50) {
    sunset();
    incomingByte = 0;
  }
  else if (incomingByte == 51) {
    openBlind();
    incomingByte = 0;
  }
  else if (incomingByte == 52) {
    closeBlind();
    incomingByte = 0;
  }
}

void writePosition()
{
  long c = stepper.currentPosition();  //Variable to store in EEPROM
  if (stepper.distanceToGo() == 0 && c != EEPROM.get( posAddress, o )) {
    EEPROM.put( posAddress, c );         //Store to EEPROM
  }
}


void checkCalSw()      //check calibration button
{
  calSwState = digitalRead(calSwPin);
  if (calSwState == LOW) {
    potVal = map(analogRead(potPin), 0, 1023, 1150, -1150);
    stepper.setCurrentPosition(0);
    stepper.moveTo(potVal);
    stepper.setMaxSpeed(5000);
    stepper.setAcceleration(7000);
  }

}

void ledBrightness() {
  ledPwm = map(stepper.currentPosition(), 0, 1150, 0, 255);
  analogWrite(ledPin, ledPwm);
}

void loop()
{
  /*
    Serial.print("current motor position:");
    Serial.println(stepper.currentPosition());
    Serial.print("current commanded position:");
    Serial.println(potVal);
    Serial.print("stored position value:");
    EEPROM.get( posAddress, o );
    Serial.println(o);
    Serial.println();
  */
  checkPot();
  stepper.run();
  checkMaster();
  stepper.run();
  checkCalSw();
  stepper.run();
  writePosition();
  stepper.run();
  ledBrightness();
  stepper.run();

  /* XBee debugging
    if (HWSERIAL.available() > 0) {
    incomingByte = HWSERIAL.read();
    Serial.print("UART received: ");
    Serial.println(incomingByte, DEC);
    HWSERIAL.print("UART received:");
    HWSERIAL.println(incomingByte, DEC);
    }
    Serial.println("pot Val:");
    HWSERIAL.println(potVal);
    Serial.println("pot Old Val:");
    Serial.println(potOldVal);
    Serial.println("Distance To Go:");
    Serial.println(stepper.distanceToGo());
    HWSERIAL.println("Current Pos:");
    HWSERIAL.println(stepper.currentPosition());
  */
}
