// RTC
#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

// Tiny RTC Pins
// RTC    Nano
// SDA    A4
// SCL    A5

// LCD
#include <LiquidCrystal.h>

// Display Pins
// LCD      UNO   Nano
// 4  (RS)  12    4 (D4)
// 6  (E)   11    5 (D5)
// 11 (D4)  5     6 (D6)
// 12 (D5)  4     7 (D7)
// 13 (D6)  3     8 (D8)
// 14 (D7)  2     9 (D9)

LiquidCrystal lcd(4, 5, 6, 7, 8, 9);

// Hour/minutes in 24-hour time format
uint8_t currentHour = 6;
uint8_t currentMinute = 45;
uint8_t lastMinute = -1;
const uint8_t alarmHour = 6;
const uint8_t alarmMinute = 45;

#define DEACTIVATE_ALARM_BUTTON 12
bool alarmDeactivated = false;

#define PIEZO 11
const uint8_t noteBitmapRows = 2;
const uint8_t noteBitmapColumns = 8;
uint8_t noteDuration = 50;

// Bits represent notes on or off
unsigned char tuneBitmap[noteBitmapRows] = {
  B01010101,
  B00000000
}; // Bits are traversed back-to-front

void setup() {
  Serial.begin(9600);

  pinMode(DEACTIVATE_ALARM_BUTTON, INPUT);
}

void loop() {
  tmElements_t tm;

  if (RTC.read(tm)) {
    currentHour = tm.Hour;
    currentMinute = tm.Minute;
  } else {
    Serial.print("[DS1307] Read error!");
  }

  if (currentMinute != lastMinute) {
    lastMinute = currentMinute;
    renderDisplay();
  }

  if (digitalRead(DEACTIVATE_ALARM_BUTTON) == HIGH) {
    Serial.println("Button pressed");
    alarmDeactivated = true;
  }

  // Current duration is within the specified alarmMinute
  if (alarmTimeIsReached(currentHour, currentMinute)) {
    if (!alarmDeactivated) {
      soundAlarm();
    }
  } else {
    // Outside of the alarm window reset the
    // alarmDeactivated check if it's enabled
    if (alarmDeactivated) {
      Serial.println("Resetting button");
      alarmDeactivated = false;
    }
  }
}

void renderDisplay() {
  lcd.begin(16, 2);
  lcd.print(currentHour);
  lcd.print(":");
  lcd.print(currentMinute);
  lcd.setCursor(6, 0);
  lcd.print("|");
  lcd.setCursor(6, 1);
  lcd.print("|");
  lcd.setCursor(8, 0);
  lcd.print("Wed");
  lcd.setCursor(8, 1);
  lcd.print("17/05/23");
}

bool alarmTimeIsReached(uint8_t hour, uint8_t minute) {
  if (hour == alarmHour && minute == alarmMinute) {
    return true;
  }
  return false;
}

void soundAlarm() {
  // Go through the note bitmap rows
  for (uint8_t row = 0; row < noteBitmapRows; row++) {
    // Go through the columns in the note bitmap row
    for (uint8_t column = 0; column < noteBitmapColumns; column++) {
      byte currentTuneBit = bitRead(tuneBitmap[row], column);  // needs to pass *bitmap if not already a pointer

      if (currentTuneBit == 1) {
        // A note should play
        handleCurrentNoteOn();
      } else {
        // It is a rest
        handleCurrentNoteOff();
      }
    }
  }
}

// The melody is playing a note
void handleCurrentNoteOn() {
  Serial.println("Note On");
  tone(PIEZO, 880, noteDuration);
  delay(noteDuration);
}

// The melody is playing a rest
void handleCurrentNoteOff() {
  Serial.println("Note Off");
  delay(noteDuration);
}
