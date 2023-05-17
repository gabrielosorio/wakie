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

void setup() {
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
}
