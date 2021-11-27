#include <M5Core2.h>
#include "BluetoothSerial.h"
BluetoothSerial SerialBT;
void setup() {
   SerialBT.begin("M5"); //Bluetooth device name
   M5.begin();
//   M5.Power.begin();   
}
void loop() {
   if (SerialBT.available()) {
     char ch=SerialBT.read();
     if(ch=='a' || ch=='A')
        M5.Lcd.fillScreen(WHITE);
     if(ch=='b' || ch=='B')
        M5.Lcd.fillScreen(RED);
     if(ch=='c' || ch=='C')
        M5.Lcd.fillScreen(GREEN);
     if(ch=='d' || ch=='D')
        M5.Lcd.fillScreen(BLUE);        
   }
   M5.update();   // update button state
   if(M5.BtnA.isPressed()) {
      SerialBT.println("Key A");
   //   M5.Speaker.beep(); //beep
   }
   if(M5.BtnB.isPressed()) {
      SerialBT.println("Key B");
   //   M5.Speaker.tone(1500, 200); //beep
   }
   if(M5.BtnC.isPressed()) {
      SerialBT.println("Key C");
   //   M5.Speaker.tone(2000, 200); //beep
   }
   delay(100);
}
