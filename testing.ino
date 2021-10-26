
#include <M5Core2.h> // M5CORE2 library
#include "MAX30100_PulseOximeter.h" // Sensor header file

#define REPORTING_PERIOD_MS     1000 //Time between screen updates
PulseOximeter pox; //Sensor functions
uint32_t tsLastReport = 0; 

void onBeatDetected() //Function prints if a heartbeat 
{
    Serial.println("B:1");
}

void setup() {  //One time executed functions
  M5.begin(true, false, true, true);  //Initiate M5Core2.  
  M5.Lcd.print("Welcome!");  // Print text on the M5 screen
  Serial.print("It's on!"); //Print on ide if M5 is on

  if (!pox.begin()) { // Check if the sensor is recognized
        Serial.println("ERROR: Failed to initialize pulse oximeter");
        M5.Lcd.println("ERROR: Failed to initialize pulse oximeter");
  } else {
        Serial.println("SUCCESS");
  }

    pox.setOnBeatDetectedCallback(onBeatDetected); //Print heartbeat detected if it is working
}



void loop() { //Repeatedly executed program 
  pox.update(); //Retrieve next heartbeat value

  if (millis() - tsLastReport > REPORTING_PERIOD_MS) { //Print heartbeat frequency in the screen and in the console
        M5.Lcd.clear();
        M5.Lcd.setCursor(0,0);
        M5.Lcd.setTextSize(5);
        Serial.print("Heart rate:");
        Serial.print(pox.getHeartRate());
        Serial.print("bpm \n");
        M5.Lcd.print("Heart rate:");
        M5.Lcd.print(pox.getHeartRate());
        M5.Lcd.print("bpm");

        tsLastReport = millis();
  }
}
