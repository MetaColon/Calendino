// Include libraries and declare the relevant object
#include <RTClib.h>
#include <Timezone.h>
#include "U8glib.h"

RTC_DS3231 rtc;

int oled_cs = 9;
int oled_a0 = 10;
int oled_rst = 8;
U8GLIB_SH1106_128X64 u8g(oled_cs, oled_a0, oled_rst);  // Custom (from https://forum.arduino.cc/t/problem-with-sainsmart-spi-1-3-oled/274912/3)

TimeChangeRule myDST = { "CEST", Last, Sun, Mar, 2, 120 };  //Daylight time = UTC + 2 hours
TimeChangeRule mySTD = { "CET", Last, Sun, Oct, 3, 60 };    //Standard time = UTC + 1 hour
Timezone myTZ(myDST, mySTD);

// Define the pins for the second shift register. The first register has the same pins +3
int latchPin = 3;
int clockPin = 4;
int dataPin = 2;

int beepPort = 2;

int player1Port = 0;
int player2Port = 1;

void setup() {
  // Serial for debugging
  Serial.begin(9600);

  // Set all shift register pins as output
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin + 3, OUTPUT);
  pinMode(clockPin + 3, OUTPUT);
  pinMode(dataPin + 3, OUTPUT);

  pinMode(beepPort, OUTPUT);

  digitalWrite(beepPort, HIGH);

  // Turn off LED
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  // Initialize RTC
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
  } else {
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

  // assign default color value
  if (u8g.getMode() == U8G_MODE_R3G3B2) {
    u8g.setColorIndex(255);  // white
  } else if (u8g.getMode() == U8G_MODE_GRAY2BIT) {
    u8g.setColorIndex(3);  // max intensity
  } else if (u8g.getMode() == U8G_MODE_BW) {
    u8g.setColorIndex(1);  // pixel on
  } else if (u8g.getMode() == U8G_MODE_HICOLOR) {
    u8g.setHiColorByRGB(255, 255, 255);
  }

  alleMeineEntchen();
}

void loop() {
  if (analogRead(player1Port) > 1000 && analogRead(player2Port > 1000))
    pong();

  // Read current datetime
  DateTime nowUTC = rtc.now();  //DateTime(DateTime(F(__DATE__), F(__TIME__)).unixtime() - 2 * 60 * 60);  //
  // ... and convert it to the correct timezone
  time_t nowTime = myTZ.toLocal(nowUTC.unixtime());
  DateTime now = DateTime(nowTime);
  char *buff = malloc(10 * sizeof(*buff));
  strcpy(buff, "hh:mm:ss");
  print(now.toString(buff));
  free(buff);
  // Debug printing
  Serial.print("Time is: ");
  Serial.print(now.hour());
  Serial.print(":");
  Serial.print(now.minute());
  Serial.print(":");
  Serial.println(now.second());
  // Light the appropriate LED (0-index, LEDs are positioned in the "wrong" order)
  lightPin(32 - now.day());

  if (now.hour() == 13 && now.minute() == 12)
    tuff();
  // Wait a bit
  delay(1000);
}

// Prints on screen of OLED
void print(char *text) {
  u8g.firstPage();
  do {
    write(text);
  } while (u8g.nextPage());
}

// Writes the text on the current page of OLED
void write(char *text) {
  // graphic commands to redraw the complete screen should be placed here
  u8g.setFont(u8g_font_fur20n);
  //u8g.setFont(u8g_font_osb21);
  u8g.drawStr(0, 42, text);
}

void lightPin(int pin) {
  // Initialize by setting latchPin of the second pair to 0
  digitalWrite(latchPin, 0);
  // Write data to the two shift registers
  shiftOut(dataPin, clockPin, pin >= 16 && pin < 24 ? 1 << (pin - 16) : 0);
  shiftOut(dataPin, clockPin, pin >= 24 && pin < 32 ? 1 << (pin - 24) : 0);
  // Done by setting latchPin of the second pair to 1
  digitalWrite(latchPin, 1);
  // ... and the same story for the first pair
  digitalWrite(latchPin + 3, 0);
  shiftOut(dataPin + 3, clockPin + 3, pin < 8 ? 1 << pin : 0);
  shiftOut(dataPin + 3, clockPin + 3, pin >= 8 && pin < 16 ? 1 << (pin - 8) : 0);
  digitalWrite(latchPin + 3, 1);
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

// ----- Music ------
void tuff() {
  int tones[] = {
    5, 19, 17, 16, 14, 5, 14, 5, 14,
    2, 16, 14, 12, 9, 0, 9, 0, 9,
    4, 18, 16, 14, 13, 4, 13, 4, 13,
    12, 5, 12, 5, 12, 14, 5, 14, 5, 14
  };
  for (int i = 0; i < 37; i++)
    tones[i] += 5;
  double lengths[] = {
    1.0/4, 1.0/4, 1.0/8, 1.0/4, 3.0/8, 1.0/8, 1.0/4, 1.0/8, 1.0/4,
    1.0/4, 1.0/4, 1.0/8, 1.0/4, 3.0/8, 1.0/8, 1.0/4, 1.0/8, 1.0/4,
    1.0/4, 1.0/4, 1.0/8, 1.0/4, 3.0/8, 1.0/8, 1.0/4, 1.0/8, 1.0/4,
    1.0/4, 1.0/8, 1.0/4, 1.0/8, 1.0/4, 1.0/4, 1.0/8, 1.0/4, 1.0/8, 1.0/4, 
  };
  playSong(880, 1000, tones, lengths, 37);
}

void alleMeineEntchen() {
  int tones[] = {
    3, 5, 7, 8, 10, 10,
    12, 12, 12, 12, 10, 0,
    12, 12, 12, 12, 10, 0,
    8, 8, 8, 8, 7, 7,
    10, 10, 10, 10, 3, 0
  };
  double lengths[] = {
    1.0 / 8, 1.0 / 8, 1.0 / 8, 1.0 / 8, 1.0 / 4, 1.0 / 4,
    1.0 / 8, 1.0 / 8, 1.0 / 8, 1.0 / 8, 1.0 / 4, -1.0 / 4,
    1.0 / 8, 1.0 / 8, 1.0 / 8, 1.0 / 8, 1.0 / 4, -1.0 / 4,
    1.0 / 8, 1.0 / 8, 1.0 / 8, 1.0 / 8, 1.0 / 4, 1.0 / 4,
    1.0 / 8, 1.0 / 8, 1.0 / 8, 1.0 / 8, 1.0 / 4, -1.0 / 4
  };
  playSong(1760, 2000, tones, lengths, 30);
}

void playSong(long baseFreq, long baseDuration, int *notes, double *lengths, int length) {
  for (int i = 0; i < length; i++) {
    playNote(baseFreq, notes[i], lengths[i] * baseDuration);
  }
}

void playNote(long base, int note, long duration) {
  playTone(base * pow(2, 1.0 / 12 * note), duration);
}

// Negative duration means pause
void playTone(long freq, long duration) {
  if (duration < 0) {
    delay(-duration);
    return;
  }
  Serial.print(freq);
  Serial.println("Hz");
  for (long i = 0; i < duration * 1000; i += 1000000 / freq) {
    digitalWrite(beepPort, HIGH);
    delayMicroseconds(500000 / freq);
    digitalWrite(beepPort, LOW);
    delayMicroseconds(500000 / freq);
  }
}

double ballX = 63.5;
double ballY = 31.5;
double velX = 1.0;
double velY = 0.0;
long lastUpdate = 0;
long updateDiff = 10;
double barHeight = 16;
int y1 = 26;
int y2 = 26;
int score1 = 0;
int score2 = 0;

void pong() {
  score1 = 0;
  score2 = 0;
  lost();
  while (score1 < 11 && score2 < 11) {
    y1 = (long)(1024 - analogRead(player1Port)) * (64 - barHeight) / 1024;
    y2 = (long)(1024 - analogRead(player2Port)) * (64 - barHeight) / 1024;
    u8g.firstPage();
    do {
      u8g.drawVLine(0, y1, barHeight);
      u8g.drawVLine(127, y2, barHeight);
      u8g.drawPixel(ballX, ballY);
      u8g.setFont(u8g_font_fur14n);
      char cstr[4];
      itoa(score1, cstr, 10);
      u8g.drawStr(10, 20, cstr);
      itoa(score2, cstr, 10);
      u8g.drawStr(108, 20, cstr);
    } while (u8g.nextPage());

    if (millis() - lastUpdate > updateDiff) {
      lastUpdate = millis();
      ballX += velX;
      ballY += velY;
      edgeCases();
    }
  }
  u8g.firstPage();
  do {
    u8g.setFont(u8g_font_courB24);
    if (score1 == 11)
      u8g.drawStr(10, 20, "1 has won!");
    else
      u8g.drawStr(10, 20, "2 has won!");
  } while (u8g.nextPage());
  delay(1000);
}

void edgeCases() {
  if ((int)ballX >= 127) {
    if (y2 <= ballY && y2 + barHeight >= ballY) {
      ballX = 127;
      velX = -velX;
      velY = (ballY - y2 - barHeight / 2) / barHeight * abs(velX);
    } else {
      score1++;
      lost();
      velX = -velX;
    }
  } else if ((int)ballX <= 0) {
    if (y1 <= ballY && y1 + barHeight >= ballY) {
      ballX = 0;
      velX = -velX;
      velY = (ballY - y1 - barHeight / 2) / barHeight * abs(velX);
    } else {
      score2++;
      lost();
    }
  }

  if ((int)ballY >= 63) {
    ballY = 62;
    velY = -velY;
  } else if ((int)ballY <= 0) {
    ballY = 1;
    velY = -velY;
  }
}

void lost() {
  ballX = 63.5;
  ballY = 31.5;
  velX = 1 + (double)(max(score1, score2)) / 4;
  velY = 0.0;
  delay(500);
}
