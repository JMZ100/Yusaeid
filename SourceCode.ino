
#include <M5Core2.h>                 //M5 Library
#include "BluetoothSerial.h"         //Bluetooth library
#include "MAX30100_PulseOximeter.h"  //Sensor library

#define REPORTING_PERIOD_MS     1000 //Period between updates
BluetoothSerial SerialBT;            //Rename Bluetooth 
PulseOximeter pox;                   //Rename sensor
uint32_t tsLastReport = 0;
int distressCount = 0;

void onBeatDetected() //Indicates when the sensor identifies a heartbeat
{
    Serial.println("B:1");           //Indicate heartbeat detection
}


void InitiateSensor(){ //Turns the sensor and its ports on
  if (!pox.begin()) {
        Serial.println("ERROR: Failed to initialize pulse oximeter");  //Send error message to the 
        M5.Lcd.println("ERROR: Failed to initialize pulse oximeter");  //Display error message on the screen
        delay(10000);
  } else {
        Serial.println("SUCCESS");  //Succesful sensor connection
  }

    pox.setOnBeatDetectedCallback(onBeatDetected); 
}

void SendBluetooth(int &count){ //Controls the communication with the phone
  //Set text characteristics
  M5.Lcd.setCursor(70,80);
  M5.Lcd.setTextSize(4);
  //Print text for the heart rate status
  M5.Lcd.print("Status: ");
  if (pox.getHeartRate() > 10 && pox.getHeartRate() < 100) {  //Normal heartbeat configuration
    SerialBT.println("A");    //Send bluetooth signal
    Serial.println("Normal"); //Send debugging signal to the computer
    M5.Lcd.setCursor(70,120); //Set text characteristics
    M5.Lcd.setTextColor(0x07e0, 0xffff);   //Set text characteristics
    M5.Lcd.print("Normal");   //Prints feedback text on the M5 LCD screen
    M5.Lcd.setTextColor(0x061d, 0xffff);   //Set text characteristics
    count = 0; //Reset distress count
    
  } else {                                                    //Abnormal heart rate
    SerialBT.println("B");    //Send bluetooth signal
    Serial.println("Alert!"); //Send debugging signal to the computer  
    Serial.println(count);    //Send counter to the computer
    M5.Lcd.setCursor(50,120); //Set text characteristics
    M5.Lcd.setTextColor(0xfc00, 0xffff);   //Set text characteristics
    M5.Lcd.print("Distress"); //Prints feedback text on the M5 LCD screen
    M5.Lcd.setTextColor(0x061d, 0xffff);   //Set text characteristics
    count++;                  //Increases distress counter
    
    if (count > 9) {          //Activate intervention measure if there are 10s of tachycardia
       SerialBT.println("C");                 //Send bluetooth signal
       Serial.println("Sending alert...");    //Send debugging signal to the computer
       M5.Lcd.setCursor(0,160);               //Set text characteristics
       M5.Lcd.setTextSize(3);
       M5.Lcd.print("Sending alert...");      //Prints feedback text on the M5 LCD screen
    }
  }
}

void OnMessage(){ //Display initial message
  M5.Lcd.fillScreen(0xffff);            //Set text characteristics
  M5.Lcd.setTextColor(0x061d,0xffff);   
  M5.Lcd.setCursor(30,0);               
  M5.Lcd.setTextSize(6);        
  M5.Lcd.print("Welcome!");             //Prints initial messagen on the screen

  M5.Lcd.setTextColor(0xffff, 0x07e0);   //Set text characteristics
  M5.Lcd.setCursor(40,100);     
  M5.Lcd.print("Yusaeid");      
  M5.Lcd.setTextColor(0x061d, 0xffff);   //Set text characteristics
  
  Serial.print("It's on!");              //Send debugging signal to the computer 
}

void ShowHeartBeat(int &count){
   pox.update();                  //Get new value of the sensor measurements

  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {  //If the time interval is larger than the desired frequency 
        
        Serial.print("Heart rate:");          // Send measurement to the computer
        Serial.print(pox.getHeartRate());     
        Serial.print(" bpm \n");               
        
        M5.Lcd.clear();                       // Reset M5 LCD screen content
        M5.Lcd.fillScreen(0xffff);            // Set text characteristics
        M5.Lcd.setCursor(30,0);               
        M5.Lcd.setTextSize(4);                
        M5.Lcd.print("Heart rate:");          // Print heartrate value on the screen
        M5.Lcd.setCursor(50,40);              // Set text characteristics
        M5.Lcd.print(pox.getHeartRate());     // Print heartrate value on the screen
        M5.Lcd.print(" bpm");                 
        
        SendBluetooth(count);                 // Send Bluetooth signal 
        tsLastReport = millis();              // Update time 
  }
}


void setup() {                          //Function executed at the beginning of the code
  SerialBT.begin("M5");                 //Turn Bluetooth on
  M5.begin(true, false, true, true);    //Turn M5, Lcd and ports on
  OnMessage();                          //Display initial message
  delay(5000);                          //for 5 seconds
  InitiateSensor();                     //Try to turn sensor on
}

void loop() {
 ShowHeartBeat(distressCount);          //Show measured heartrate
}
