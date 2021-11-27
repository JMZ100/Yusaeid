
#include <M5Core2.h>
#include <Preferences.h>

#include "MAX30100_PulseOximeter.h"

#define REPORTING_PERIOD_MS     1000

PulseOximeter pox;
uint32_t tsLastReport = 0;

void IndividualSetting(bool& Choice, String Text){
  M5.Lcd.clear();
  M5.Lcd.setCursor(100,0);
  M5.Lcd.setTextSize(3);
  M5.Lcd.print("Settings");

  M5.Lcd.setCursor(30,100);
  M5.Lcd.setTextSize(2);
  M5.Lcd.print("Do you want to ");
  M5.Lcd.print(Text);

  M5.Lcd.setCursor(30,200);
  M5.Lcd.print("Yes");

  M5.Lcd.setCursor(150,200);
  M5.Lcd.print("No");
  bool fin=false;
 
  while (fin!=true){
  M5.update();
  if (M5.BtnA.pressedFor(1000, 200)) {
    M5.Lcd.print("porra 1");
    //Choice = false;
    fin=true;
    
  } else if (M5.BtnB.pressedFor(1000, 200)){
    M5.Lcd.print("porra 2");
    //Choice = true;
    fin=true;
  }
  }
  return;
}

void onBeatDetected()
{
    Serial.println("B:1");
}

void Settings(bool& Contact, bool& Message){
  IndividualSetting(Contact, "contact a friend?");
  IndividualSetting(Message, "read a peaceful message?");
}


void InitiateSensor(){
  if (!pox.begin()) {
        Serial.println("ERROR: Failed to initialize pulse oximeter");
        M5.Lcd.println("ERROR: Failed to initialize pulse oximeter");
  } else {
        Serial.println("SUCCESS");
  }

    pox.setOnBeatDetectedCallback(onBeatDetected);
}

void ShowHeartBeat(){
   pox.update();

  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        Serial.print("Heart rate:");
        Serial.print(pox.getHeartRate());
        Serial.print("bpm \n");
        M5.Lcd.clear();
        M5.Lcd.setCursor(0,0);
        M5.Lcd.setTextSize(5);
        M5.Lcd.print("Heart rate:");
        M5.Lcd.print(pox.getHeartRate());
        M5.Lcd.print("bpm");

        tsLastReport = millis();
  }
}

void setup() {
  M5.begin(true, false, true, true);
  M5.Lcd.print("Welcome!");
  Serial.print("It's on!");

  bool Contact = true, Message = true;
  Settings(Contact, Message);
  
  InitiateSensor();
}

void loop() {
  // put your main code here, to run repeatedly:
 ShowHeartBeat();
}
