#include <LiquidCrystal.h>

// Display Pins
// LCD      UNO   Nano
// 4  (RS)  12    2 (D2)
// 6  (E)   11    3 (D3)
// 11 (D4)  5     4 (D4)
// 12 (D5)  4     5 (D5)
// 13 (D6)  3     6 (D6)
// 14 (D7)  2     7 (D7)

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

// Alarm Settings
const uint8_t maxAlarmDuration = 5000;

#define PIEZO 10
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

  lcd.begin(16, 2);
  lcd.print("21:03");
  lcd.setCursor(6, 0);
  lcd.print("|");
  lcd.setCursor(6, 1);
  lcd.print("|");
  lcd.setCursor(8, 0);
  lcd.print("Wed");
  lcd.setCursor(8, 1);
  lcd.print("17/05/23");
}

void loop() {
  if (millis() < maxAlarmDuration) {
    soundAlarm();
  }
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
