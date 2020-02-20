/* Arduino temperature logger with SD card, DS18B20 sensor and DS3231 (DS1307) RTC.
 * Time, date and temperature are displayed on 20x4 LCD.
 * This is a free software with NO WARRANTY.
 * https://simple-circuit.com/
 */
 
#include <SD.h>               // include Arduino SD library
#include <LiquidCrystal.h>    // include Arduino LCD library
#include <RTClib.h>           // include Adafruit RTC library
#include <OneWire.h>
#include <DallasTemperature.h>

// LCD module connections (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
 
// initialize RTC library
RTC_DS3231 rtc;
DateTime   now;
 
// buttons definition
#define button1       A1   // button B1 is connected to Arduino pin A1
#define button2       A2   // button B2 is connected to Arduino pin A2
// define DS18B20 data pin
#define ONE_WIRE_BUS A3

// Setup a oneWire instance to communicate with any OneWire device
OneWire oneWire(ONE_WIRE_BUS); 
// Pass oneWire reference to DallasTemperature library
DallasTemperature sensors(&oneWire);
int deviceCount = 0;
float tempC;

File dataLog;
boolean sd_ok = 0;
 
void setup()
{
  sensors.begin();
  // locate devices on the bus
  deviceCount = sensors.getDeviceCount();
  Serial.print(deviceCount, DEC);
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
 
  rtc.begin();          // initialize RTC chip
  lcd.begin(20, 4);     // initialize LCD module
  lcd.print("T1:");
  lcd.setCursor(10, 0); // move cursor to column 10, row 0 (first row)
  lcd.print("T2:");
  lcd.setCursor(0, 3);  // move cursor to column 0, row 3 (last row)
  lcd.print("T3:");
  lcd.setCursor(10, 3);
  lcd.print("T4:");
 
  // open serial communications and wait for port to open:
  Serial.begin(9600);
  Serial.print(F("Initializing SD card..."));
 
  // initialize the SD card
  if ( !SD.begin() )
    Serial.println(F("initialization failed!"));  // initialization error
 
  else {   // initialization OK
    sd_ok = 1;
    Serial.println(F("initialization done."));
    if( SD.exists("Log.txt") == 0 )   // test if file with name 'Log.txt' already exists
    {  // create a text file named 'Log.txt'
      Serial.print(F("\r\nCreate 'Log.txt' file ... "));
      dataLog = SD.open("Log.txt", FILE_WRITE);   // create (&open) file Log.txt
      if(dataLog) {                               // if the file opened okay, write to it:
        Serial.println("OK");
        // write some texts to 'Log.txt' file
        dataLog.println(F("    DATE    |    TIME  | TEMP1 | TEMP2 | TEMP3 | TEMP4 |"));
        dataLog.close();   // close the file
      }
      else
        Serial.println("error creating file.");
    }
  }
 
  Serial.println(F("    DATE    |    TIME  | TEMP1 | TEMP2 | TEMP3 | TEMP4 |"));
  Serial.println(F("(dd-mm-yyyy)|(hh:mm:ss)|"));
}
 
// main loop
void loop()
{ 
  // Send command to all the sensors for temperature conversion
  sensors.requestTemperatures();
  now = rtc.now();  // read current time and date from the RTC chip
  RTC_display();    // diaplay time & calendar
 
  if( !digitalRead(button1) )  // if B1 is pressed
  if( debounce() )             // call debounce function (make sure B1 is pressed)
  {
    while( debounce() );  // call debounce function (wait for B1 to be released)
 
    byte hour   = edit( now.hour() );         // edit hours
    byte minute = edit( now.minute() );       // edit minutes
    byte day    = edit( now.day() );          // edit date
    byte month  = edit( now.month() );        // edit month
    byte year   = edit( now.year() - 2000 );  // edit year
 
    // write time & date data to the RTC chip
    rtc.adjust(DateTime(2000 + year, month, day, hour, minute, 0));
 
    while(debounce());  // call debounce function (wait for button B1 to be released)
  }
 
  static byte p_minute;
  if( (now.minute() % 1 == 0) && (p_minute != now.minute()) )
  {   // read & print temperature value from sensor every 5 min

    char buffer2[26];
    bool sensor_ok = 0;
    p_minute = now.minute();
 
    lcd.setCursor(5, 3);
 
    sprintf( buffer2, " %02u-%02u-%04u | %02u:%02u:%02u | ", now.day(), now.month(), now.year(),
                      now.hour(), now.minute(), now.second() );

    // print data on PC serial monitor
    Serial.print(buffer2);

    // write data to SD card
    if(sd_ok)
    {  // if the SD card was successfully initialized
      // open Log.txt file with write permission
      dataLog = SD.open("Log.txt", FILE_WRITE);
      dataLog.print( buffer2 );
    }

    // Display temperature from each sensor
    for (int i = 0;  i < deviceCount;  i++)
    {
      tempC = sensors.getTempCByIndex(i);
      Serial.print(tempC);
      Serial.print(" | ");
      dataLog.print(tempC);
      dataLog.print(" | ");  
    }

    dataLog.println("");
    Serial.println("");
    dataLog.close();
 
  }
  // update LCD in real time
  for (int i = 0;  i < deviceCount;  i++)
  {
    tempC = sensors.getTempCByIndex(i);
    printTemp(i,tempC);
  }
  delay(1000);   // wait 100ms
}

void printTemp(int i,float tempC)
{
  if (i == 0) {
    lcd.setCursor(4,0);
  }
  if (i == 1) {
    lcd.setCursor(14,0);
  }
  if (i == 2) {
    lcd.setCursor(4,3);
  }
  if (i == 3) {
    lcd.setCursor(14,3);
  }
  lcd.print(tempC);
}

//////////////////////////////////////// RTC functions ////////////////////////////////////////
void RTC_display()
{
  char _buffer[17];
  char dow_matrix[7][4] = {"SUN", "MON", "TUE", "WED",
                             "THU", " FRI", "SAT"};
  lcd.setCursor(16,2);
  lcd.print( dow_matrix[now.dayOfTheWeek()] );
 
  // print time
  sprintf( _buffer, "TIME: %02u:%02u:%02u", now.hour(), now.minute(), now.second() );
  lcd.setCursor(0, 1);
  lcd.print(_buffer);
  // print date
  sprintf( _buffer, "DATE: %02u-%02u-%04u", now.day(), now.month(), now.year() );
  lcd.setCursor(0, 2);
  lcd.print(_buffer);
}
 
byte edit(byte parameter)
{
  static byte i = 0, y_pos,
              x_pos[5] = {6, 9, 6, 9, 14};
  char text[3];
  sprintf(text,"%02u", parameter);
 
  if(i < 2)
    y_pos = 1;
  else
    y_pos = 2;
 
  while( debounce() );   // call debounce function (wait for B1 to be released)
 
  while(true) {
    while( !digitalRead(button2) ) {  // while B2 is pressed
      parameter++;
      if(i == 0 && parameter > 23)    // if hours > 23 ==> hours = 0
        parameter = 0;
      if(i == 1 && parameter > 59)    // if minutes > 59 ==> minutes = 0
        parameter = 0;
      if(i == 2 && parameter > 31)    // if day > 31 ==> day = 1
        parameter = 1;
      if(i == 3 && parameter > 12)    // If month > 12 ==> month = 1
        parameter = 1;
      if(i == 4 && parameter > 99)    // If year > 99 ==> year = 0
        parameter = 0;
 
      sprintf(text,"%02u", parameter);
      lcd.setCursor(x_pos[i], y_pos);
      lcd.print(text);
      delay(200);       // wait 200ms
    }
 
    lcd.setCursor(x_pos[i], y_pos);
    lcd.print("  ");
    unsigned long previous_m = millis();
    while( (millis() - previous_m < 250) && digitalRead(button1) && digitalRead(button2) ) ;
    lcd.setCursor(x_pos[i], y_pos);
    lcd.print(text);
    previous_m = millis();
    while( (millis() - previous_m < 250) && digitalRead(button1) && digitalRead(button2) ) ;
 
    if(!digitalRead(button1))
    {                     // if button B1 is pressed
      i = (i + 1) % 5;    // increment 'i' for the next parameter
      return parameter;   // return parameter value and exit
    }
  }
}
 
// a small function for button1 (B1) debounce
bool debounce ()
{
  byte count = 0;
  for(byte i = 0; i < 5; i++)
  {
    if ( !digitalRead(button1) )
      count++;
    delay(10);
  }
 
  if(count > 2)  return 1;
  else           return 0;
}
////////////////////////////////////// end RTC functions //////////////////////////////////////
 

 
// end of code.
