#include <ILI9486_SPI.h> 
#include "ACS712.h"
ILI9486_SPI tft(SS, 8, 9); // CS, DC, RST
ACS712  ACS(A0, 5.0, 1023, 100);
#define SCREEN_WIDTH  480
#define SCREEN_HEIGHT 320
#define BLACK   0x0000
#define WHITE   0xFFFF
char buffer2[10];  
double maxAmps=0;

void setup() {
    Serial.begin(115200);
    tft.setSpiKludge(false);  
    tft.init();
    tft.fillScreen(BLACK);
  Serial.print("ACS712_LIB_VERSION: ");
  Serial.println(ACS712_LIB_VERSION);

 
  while (!Serial);
  Serial.println(__FILE__);
  Serial.print("ACS712_LIB_VERSION: ");
  Serial.println(ACS712_LIB_VERSION);

  ACS.autoMidPoint();
  Serial.print("MidPoint: ");
  Serial.print(ACS.getMidPoint());
  Serial.print(". Noise mV: ");
  Serial.println(ACS.getNoisemV());

     Serial.println(__FILE__);
  Serial.print("INA219_LIB_VERSION: ");
  Serial.println(INA219_LIB_VERSION);

  Wire.begin();
  if (!INA.begin() )
  {
    Serial.println("could not connect. Fix and Reboot");
  }

    tft.setRotation(3);
    tft.setTextSize(7);
    Serial.println(INA.getBusVoltageRange());


    
}


void loop() {
 

    
    unsigned long startTime = millis();
  
    double PrevAmp=maxAmps;
   
   maxAmps= fabs((ACS.mA_AC_sampling(2,0.5)-130));

//
//This part will check if the voltage change is big enough to clear pixels and write new value.
//

 if(fabs(PrevAmp-maxAmps)>20){
     tft.setCursor(0,10);
      tft.setTextColor(0x0000);
    tft.print(buffer2);
 }


     if(fabs(PrevAmp-maxAmps)>20){ //if there is a difference in amps of 20mA  change previous number to black and write over
     dtostrf(maxAmps, 0, 0, buffer2);
    strncat(buffer2, "mA", 8);
     tft.setCursor(0,10);
     tft.setTextColor(0x001F);
    tft.print(buffer2);
     }

    int numBars2 = map(maxAmps, 0, 5000, 0, 480);
 
   
//
//This part display the horizontal bars on the screen for current
//If Previous value is greater then it will draw the pixels it needs to in black
//If the current value is greater then it will draw the pixels needed in color 
//If you reach the max of amps make sure to change the max value. 
// 
for (int i = map(PrevAmp, 0, 5000, 0, 480); i < numBars2; i++) {// (min,max)-> (0-5,000) mili amps mapped to 0-480 pixel
    tft.drawFastVLine(i, 260, 60, 0x001F);  // Draw bar in blue (16 bit color)
}
for (int i = map(PrevAmp, 0, 5000, 0, 480); i > numBars2; i--) {// (min,max)-> (0-5,000) mili amps mapped to 0-480 pixel
    tft.drawFastVLine(i, 260, 60, BLACK);  // Draw bar in black to where amps value now is
}




}
