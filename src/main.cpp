// So far, this code compiles with Pressure code, Actuator, SD + Wifi, reads/writes parameter file. 
//      reads all variables from sensors, calculates dA/dt, and writes into var = dataString 
//      and appends to SD Card

// External TEMP has been added and called within setup and loop.
// TODO: Add Temperature Control for Heating Element

#include <Arduino.h>
#include <math.h>             // math .. Pressure Sensor needs this
#include <String.h>
#include <Wire.h>             //I2C library fort OLED and Pressure Sensor
#include <SPI.h>              //SPI library for SD card
#include <SD.h>               //SD    ********MUST BE VERSION 1.2.2 TO WORK WITH TEENSY********
#include <EEPROM.h>


// Local Files
#include "actuator.h"
#include "Globals.h"
#include "pressure.h"
#include "sd+wifi.h"
#include "oled+switch.h"
#include "param.h"
#include "SoftwareFunctions.h"
#include "temp.h"

String the_sketchname, version;

// displays at startup the Sketch running in the Arduino
void display_Running_Sketch (void){
  String the_path = __FILE__;
  int slash_loc = the_path.lastIndexOf('/');
  String the_cpp_name = the_path.substring(slash_loc+1);
  int dot_loc = the_cpp_name.lastIndexOf('.');
  the_sketchname = the_cpp_name.substring(0, dot_loc);
  version = the_sketchname.substring(0,4);
  
  Serial.println("\nArduino is running Sketch: ");
  Serial.println(the_sketchname);
  Serial.println("Compiled on: ");
  Serial.println(__DATE__);
  Serial.println(" at ");
  Serial.println(__TIME__);
  Serial.println("\n");
}

void setup() {
  Serial.begin(115200);
  display_Running_Sketch();

  // Setup Pins
  pinMode(switch1, INPUT_PULLUP);
  pinMode(switch2, INPUT_PULLUP);
  pinMode(switch3, INPUT_PULLUP);
  
  // initialize Pin for the button
  pinMode(buttonPin, INPUT_PULLUP);
  buttonPressed=FALSE;

    // Attach Actuator
  actuator.attach(actuator_pin);

    // Setup Pressure Sensor
  pressure_setup();
  
  // EXT Temp Sensor Setup
  DallasTemperatureSensorSetup();

  // Setup Display
  oled_setup();

  // Setup SD
  sd_setup();

  // Read Parameters File and/or creates param.txt file (with default values) if not present
  readparam(SD);

  // get the current altitude for deciding how to initialize
  currentAltitude=getMedianPressure(&currentTemp);
  
  
  // if the switch is indicating that we are in flight mode (that is we were in the middle of a 
  // mission when the power cycled) them attempt to pick up from where we left
  
  // if (switchPos(currentAltitude)==FLIGHT) {
  //   char lineBuf[250];          //For log file

  //   // open logfile -> dataFile
  //   if ((File logFile = fs.open(dataFile, FILE_READ))) { 
    
  //     // read the last line of the log file to get programState, the timeOffset, 
  //     // actuator extension, and the start of floating
  //     while (logFile.available()) {
  //         logFile.readBytesUntil('\n',lineBuf,150);    
  //     }

  //     Serial.println("\n***************************** \n");
  //     Serial.print("Last Line in Buffer: ");
  //     Serial.println(lineBuf);
  //     // we should have the last line now
  //     sscanf(lineBuf, "%ld %*d %d %f %ld",&timeOffset, &programState, &currentActuatorExtension, &floatStartTime);
  //     Serial.print("Time Offset: "); Serial.println(timeOffset);
  //     Serial.print("Program State: "); Serial.println(programState);
  //     Serial.print("Current Actuator Extension: "); Serial.println(currentActuatorExtension);
  //     Serial.print("Float Start TIme: "); Serial.println(floatStartTime);
  //     Serial.println("\n***************************** \n");
    
  //     SetStrokePerc(currentActuatorExtension);
  //   }
  // }
  

  String header = "Current Time,Switch State,Program State,Actuator Extension,Float Start Time,Pressure,Altitude,dAdt,Temp,TeensyTemp";
////////////////////////////


/*
  // open logfile
  if (!(logFile = SD.open(datafile, FILE_WRITE))) { 
    Serial.println("Failed to open logfile.");
    display.clearDisplay();
    display.setTextSize(2); 
    display.setTextColor(WHITE);
    display.setCursor(18, 0);
    display.println("LOGFILE");
    display.setCursor(18, 16);
    display.println("FAILED");
    display.display();
    delay(5000);
    //return;
    }
  else
    { unsigned long size = logFile.size();
      Serial.print("Filesize: "); Serial.println(size);
      if (size == 0){
        logDataE("Far Horizons | Adler Planetarium");
        logData("Flight Number: "); logDataE(flightNum);
        logData("Firmware: "); logDataE(the_sketchname);
        logData("Compiled on: ");logData(__DATE__); logData(" at "); logDataE(__TIME__);
        logData("Venting Altitude (m): "); logDataE(VENT_ALT);
        logData("Float Time (min): "); logDataE(FLOAT_TIME);
        logData("Max Float Altitutde (m): "); logDataE(MAXFLOATALT);
        logData("Release Position %: "); logDataE(RELEASE_POS);
        logData("Venting Position %: "); logDataE(OPEN_POS); logData("Closed Position"); logDataE(CLOSED_POS);
        logDataE();
        logDataE(header);
        Serial.println("LOG FILE CREATED with DEFAULT PARAMETERS");
        delay(500);
      }

    }
*/
////////////////////////////
  String parameterText = "Venting Alt: " + String(VENT_ALT) + " m" + "\n" + 
                         "Float Time: "  + String(FLOAT_TIME) + " min" + "\n" + 
                         "MaxFloatAlt: " + String(MAXFLOATALT) + " m" + "\n" + 
                         "Open/Closed_pos: " + String(OPEN_POS) + "/" + String(CLOSED_POS) + " %";
                         
  OLEDScrollText(parameterText);    // prints parameters on OLED with scrolling text
  delay(2000);
  
  lastLogTime=-9999;
  
  Serial.println(header);

  // Prepare_Data();
  writeFile(SD, dataFile, header);    //In an actual code, this should be used to add the header. It's just a regular output for this test though.
}

void loop(){
  int switchState;
  long int currentTime;
  float currentPressure,currentTemp,currentAltitude,currentDAdt;

  // get time
  currentTime=millis()+timeOffset;
  
  // get EXT Temp
  getEXT_TempData();  

  // get pressure sensor values
  currentPressure= getMedianPressure(&currentTemp);

  // derive altitude and derivative  
  currentAltitude=altitudeFromPressure(currentPressure);
  currentDAdt=dAdt_fiT_2(currentAltitude,currentTime);

  // process switch and button
  switchState = switchPos(currentAltitude);
  if (digitalRead(buttonPin) == HIGH) buttonPressed=TRUE; 
  
  // Prepares all variable data for write to SD.
  dataString = 
    String(currentTime) + "," + 
    String(switchState) + "," + 
    String(programState) + "," + 
    String(currentActuatorExtension) + "," + 
    String(floatStartTime) + "," + 
    String(currentPressure) + "," + 
    String(currentAltitude) + "," + 
    String(currentDAdt) + "," + 
    String(currentTemp) + "," + 
    String(currentEXT_Temp) + "\n"; // Replace with external Temp sensor Dallas...
    lastLogTime=currentTime;
  
  // Writes to SD.
  appendFile(SD, dataFile, dataString);     //Append the contents of the dataString to the end of the file on a new line.

  display.clearDisplay();
  display.setCursor(0,20);
  logFileStatus();        // CHECK STATUS OF SD CARD

  switch (switchState) {
    case(RELEASE):
        display.setCursor(0,0);
        display.println("Press to RELEASE");
        display.setCursor(0,16);
        display.println("Actuator will retreat");
        if (buttonPressed) {
          display.clearDisplay();
          display.setCursor(16,16);
            display.println("Button pressed");
            delay(100);
            buttonPressed=FALSE;
            SetStrokePerc(RELEASE_POS);
        }
        programState=GROUND;
        logFileStatus();        // CHECK STATUS OF SD CARD
        display.display();
        break;
    case(CLOSED):
        display.setCursor(0,0);
        display.println("CLOSED POSITION");
        display.print("Pres: "); display.println(currentPressure);
        display.setCursor(0,24);
        display.println("Closing Nozzle...");
        logFileStatus();        // CHECK STATUS OF SD CARD
        display.display();
        SetStrokePerc(CLOSED_POS);
        programState=GROUND;
        break;
    case(FLIGHT):
        switch (programState) {
          case (GROUND):
              programStateString = "GROUND";
              SetStrokePerc(CLOSED_POS);            // this may take a lot of energy if the actuator can't quite get there
              if (currentAltitude>2000) programState=ASCENT;
              if (!buttonPressed) {
                display.setCursor(0,24); display.setTextColor(BLACK, WHITE); display.print("Press to Start"); display.setTextColor(WHITE, BLACK);
              }
              if (buttonPressed) {
                 display.setCursor(0,24); display.println("... pressed");
                 delay(100);
                 buttonPressed=FALSE;
                 programState=ASCENT;
              }
              break;
          case (ASCENT):
              programStateString = "ASCENT";
              if (currentAltitude>VENT_ALT) {     // this is in meters
                SetStrokePerc(OPEN_POS);
                programState=VENTING;
                N_DAdt_lt_0=0;                
              }
              break;
          case (VENTING):
              programStateString = "VENTING";
              if (currentDAdt<0) {
                N_DAdt_lt_0++;
                if (N_DAdt_lt_0>N_ASCENTRATEBELOWZERO) {
                  SetStrokePerc(CLOSED_POS);
                  programState=FLOATING; 
                  floatStartTime=currentTime;  
                }                            
              }
              break;
          case (FLOATING):
              programStateString = "FLOATING";
              if ((currentTime-floatStartTime>FLOAT_TIME*60*1000) || (currentAltitude>MAXFLOATALT)) {
                SetStrokePerc(RELEASE_POS);
                programState=DESCENT;                               
              }
              break;
          case (DESCENT):
              programStateString = "DESCENT";
              break;
          default:
            programStateString = "Error";
            break;                  
        }
        display.setCursor(0,0); display.print("FLIGHT-"); display.print(programState); display.print(":"); display.print(programStateString); 
        display.setCursor(90,24); display.println(SDStatus);
        if (SDStatus == "SD-ERR"){
          display.setCursor(90,24); 
          display.setTextColor(BLACK, WHITE); display.println(SDStatus); display.setTextColor(WHITE, BLACK);
        }
        display.setCursor(0,8); display.print("Press"); display.setCursor(52,8); display.println("Alt"); display.setCursor(90,8); display.println("Vel");
        display.print(currentPressure,2); display.setCursor(52,16); display.print(currentAltitude,0); display.setCursor(90,16); display.println(currentDAdt);
        display.display();
        break;
  }
  delay(200);
  buttonPressed=FALSE;
}

