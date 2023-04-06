// Include the RTC and timezone libraries and declare the relevant object
#include <RTClib.h>
#include <Timezone.h>
RTC_DS3231 rtc;

TimeChangeRule myDST = {"CEST", Last, Sun, Mar, 2, 120};    //Daylight time = UTC + 2 hours
TimeChangeRule mySTD = {"CET", Last, Sun, Oct, 3, 60};     //Standard time = UTC + 1 hour
Timezone myTZ(myDST, mySTD);

// Define the pins for the second shift register. The first register has the same pins +3
int latchPin = 3;
int clockPin = 4;
int dataPin = 2;

void setup() {
  // Serial for debugging
  Serial.begin(9600);

  // Set all shift register pins as output
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin+3, OUTPUT);
  pinMode(clockPin+3, OUTPUT);
  pinMode(dataPin+3, OUTPUT);

  // Turn off LED
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  // Initialize RTC
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1);
  }
  else {
    Serial.println("Initialized RTC");
    Serial.flush();
  }
  // ... and set it to the current UTC time. Only run this once!
  //rtc.adjust(DateTime(DateTime(F(__DATE__), F(__TIME__)).unixtime()-2*60*60));
  // Animation
  for (int i = 0; i < 32; i++) {
    lightPin(i);
    delay(100);
  }
}

void loop() {
  // Read current datetime
  DateTime nowUTC = rtc.now();
  // ... and convert it to the correct timezone
  time_t nowTime = myTZ.toLocal(nowUTC.unixtime());
  DateTime now = DateTime(nowTime);
  // Debug printing
  Serial.print("Time is: ");
  Serial.print(now.hour());
  Serial.print(":");
  Serial.print(now.minute());
  Serial.print(":");
  Serial.println(now.second());
  // Light the appropriate LED (0-index, LEDs are positioned in the "wrong" order)
  lightPin(32-now.day());
  // Wait a bit
  delay(10000);
}

void lightPin(int pin) {
  // Initialize by setting latchPin of the second pair to 0
  digitalWrite(latchPin, 0);
  // Write data to the two shift registers
  shiftOut(dataPin, clockPin, pin >= 16 && pin < 24 ? 1<<(pin-16) : 0);
  shiftOut(dataPin, clockPin, pin >= 24 && pin < 32 ? 1<<(pin-24) : 0);
  // Done by setting latchPin of the second pair to 1
  digitalWrite(latchPin, 1);
  // ... and the same story for the first pair
  digitalWrite(latchPin+3, 0);
  shiftOut(dataPin+3, clockPin+3, pin < 8 ? 1<<pin : 0);
  shiftOut(dataPin+3, clockPin+3, pin >= 8 && pin < 16 ? 1<<(pin-8) : 0);
  digitalWrite(latchPin+3, 1);
}

// Copied from the examples (https://docs.arduino.cc/tutorials/communication/guide-to-shift-out#shftout22)
void shiftOut(int myDataPin, int myClockPin, byte myDataOut) {
  int i = 0;
  int pinState;
  digitalWrite(myDataPin, 0);
  digitalWrite(myClockPin, 0);
  for (i = 7; i >= 0; i--) {
    digitalWrite(myClockPin, 0);
    if (myDataOut & (1 << i)) {
      pinState = 1;
    } else {
      pinState = 0;
    }
    digitalWrite(myDataPin, pinState);
    digitalWrite(myClockPin, 1);
    digitalWrite(myDataPin, 0);
  }
  digitalWrite(myClockPin, 0);
}