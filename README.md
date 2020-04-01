# autotemplogger
Arduino program to log and timestamp multiple temperatures from DS18B20 sensors to a text file on an SD card. Application for solar collector heat transfer monitoring, although you can use it for any application where you want to automatically record multiple temperatures of different environments.

Reference for Multi-Temperature logging: https://arduinoinfo.mywikis.net/wiki/Brick-Temperature-DS18B20#Multiple_DS18B20_Example

Reference for SD card/temp data integration: https://simple-circuit.com/sd-card-arduino-temperature-data-logger/

The full Arduino circuit diagram is the "Full Circuit Diagram.png" file. Use this to build the circuit.

The final working script is: "autoTempLogger.ino". Upload this script to your Arduino.

When building the final circuit, it is possible to just build the entire circuit right away by following the Full Circuit Diagram provided.  However, it is best practice to incrementally build the circuit and test if different parts of the circuit work along the way. Therefore I included a few other circuit diagrams and test scripts uploaded in this repo. These are meant for testing purposes, to be used as a reference as the user is building and debugging their circuit from scratch.

In this case, you would build the circuit in order of these steps:
1. Connect LCD Screen
2. Connect First Temperature Sensor 
3. Connect SD card and RTC to Circuit
4. Connect Multiple Temperature Sensors to Circuit

A separate folder is provided for testing and debugging steps 1 and 2. Steps 3 and 4 use the final working script and circuit diagram.
