#include <LiquidCrystal.h>

// define digital pins on Arduino
// LCD RS -> D12
// LCD E -> D11
// LCD D4 -> D5
// LCD D5 -> D4
// LCD D6 -> D3
// LCD D7 -> D2
// LCD module connections (RS, E, D4, D5, D6, D7)
//LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
void setup() {
  lcd.begin(20, 4);
  lcd.print("hello, world!");
  Serial.print("hello, world!");
}

void loop() {
  
}
