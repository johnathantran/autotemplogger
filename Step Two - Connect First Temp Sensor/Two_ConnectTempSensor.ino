/* Arduino temperature logger with SD card, DS18B20 sensor and DS3231 (DS1307) RTC.
 * Time, date and temperature are displayed on 20x4 LCD.
 * This is a free software with NO WARRANTY.
 * https://simple-circuit.com/
 */

#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>

// Data output of DS18B20 is connected to Arduino digital pin 2
#define ONE_WIRE_BUS A3

// Setting a one wire instance
OneWire oneWire(ONE_WIRE_BUS);

// Passing onewire instance to Dallas Temperature sensor library
DallasTemperature sensors(&oneWire);

// LCD module connections (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
   
void setup(){ 
    lcd.begin(20,4);                       // Initialize the 20x4 LCD shield
    sensors.begin();                        // Begin the DS18B20 initialization
}

void loop(){ 
int Celsius=0, Fahrenheit=0;     // Variables to store temperature readings from DS18B20 temperature sensor
  sensors.requestTemperatures(); // Call all sensors on one wire to start calculating the temperature readings

  Celsius=sensors.getTempCByIndex(0);   //Get temperature reading from sensor 0 in celsius scale
  Fahrenheit=sensors.getTempFByIndex(0);//Get temperature reading from sensor 0 in fahrenheit scale
  
    lcd.setCursor(0, 0);      // Place the lcd cursor on Row-1 Coulomb-1       
    lcd.print("C: ");         // Print Celsius temperature on first row of lcd          
    lcd.print(Celsius); 
    lcd.setCursor(0, 1);      // Place the lcd cursor on Row-2 Coulomb-1      
    lcd.print("F: ");         // Print Fahrenheit temperature on second row of lcd       
    lcd.print(Fahrenheit);      
    delay(5000);              //Refresh the temperature readings after every 5 seconds   
}
