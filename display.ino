#include <LiquidCrystal_I2C.h>;
LiquidCrystal_I2C lcd(0x27, 20, 4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
char error_array[9][17] = { "Humidity low    ", "Humidity high   ", "Flash detected  ", "Temp too high   ", "Temp too low    ", "Motion detected ", "Laser triggered ", "Security OFF    ", "Security ON     " };

void setup() {
  Serial.begin(9600);
  lcd.init();  // initialize the lcd
  lcd.backlight();
}

//Display function, works off of error ID's. Call the function with the correct id and it is displayed
void display_error(int errorID) {
  lcd.setCursor(0, 0);
  lcd.print("ERROR CODE: ");
  lcd.print(errorID);
  lcd.setCursor(0, 1);
  lcd.print(error_array[errorID]);
}

void loop() {
  for (int i = 0; i < 9; i++) {
    display_error(i);
    delay(2000);
  }
}
