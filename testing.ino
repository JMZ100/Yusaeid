
#include <M5Core2.h>
#include "MAX30100_PulseOximeter.h"

#define REPORTING_PERIOD_MS     1000
PulseOximeter pox;
uint32_t tsLastReport = 0;

void onBeatDetected()
{
    Serial.println("B:1");
}

void setup() {
  M5.begin(true, false, true, true);  //Init M5Core2.  初始化 M5Core2
  M5.Lcd.print("Welcome!");  // Print text on the screen (string) 在屏幕上打印文本(字符串)
  Serial.print("It's on!");

  if (!pox.begin()) {
        Serial.println("ERROR: Failed to initialize pulse oximeter");
        M5.Lcd.println("ERROR: Failed to initialize pulse oximeter");
  } else {
        Serial.println("SUCCESS");
  }

    pox.setOnBeatDetectedCallback(onBeatDetected);
}



void loop() {
  pox.update();

  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
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
