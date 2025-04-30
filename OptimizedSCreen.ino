#include <ILI9486_SPI.h> 
#include "INA219.h"
#include "ACS712.h"
#define SAMPLE_TIME_MS 250  // Collect data for 1 second
#define SAMPLE_RATE_MS 2     // Sample every 5ms
 INA219 INA(0x40);
//Adafruit_INA260 ina260 = Adafruit_INA260();
ILI9486_SPI tft(SS, 8, 9); // CS, DC, RST
ACS712  ACS(A0, 5.0, 1023, 100);
#define SCREEN_WIDTH  480
#define SCREEN_HEIGHT 320
#define BLACK   0x0000
#define WHITE   0xFFFF
char buffer[10];
    char buffer2[10];  
    char buffer3[10];  
        char buffer4[10];  
     float maxVoltage = 0;  // Track the highest voltage
    double maxAmps=0;
    float Maxpower= 0;
    float bigpower=0;


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
    // if (!ina260.begin()) {
    //     Serial.println("INA260 not found! Check wiring.");
    //    // while (1);
    // }
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
// float applyPolynomial(float x) {
//     return 2.6062*x ;
// }
// float calculateY(float x) {
//   float y = (1E-17 * pow(x, 6)) - (1E-13 * pow(x, 5)) + (7E-10 * pow(x, 4)) - (1E-06 * pow(x, 3)) + (0.001 * pow(x, 2)) + (2.6535 * x);
//   return y;
// }


void loop() {
  //tft.print("Hello");

    
    unsigned long startTime = millis();
   float PrevVolt=maxVoltage;
    double PrevAmp=maxAmps;
    float PrevWatt=Maxpower;
  float maxVoltage2=0;
    while (millis() - startTime < SAMPLE_TIME_MS) {  
        float voltage = INA.getBusVoltage()*1000;
       
   //     Serial.println(amps);
       // Serial.println(voltage);

        if (voltage > maxVoltage2) { 
            maxVoltage2 = voltage;  // Update max voltage
        }
        //   if (abs(amps) > maxAmps) {
        //      maxAmps=abs(amps);// Update max voltage
        //  }

        delay(SAMPLE_RATE_MS);
    }
    maxVoltage=maxVoltage2;
     // Serial.print(INA.getBusVoltage(), 2);
  //Serial.print("hello");
  //float maxVoltage = INA.getBusVoltage()*1000;
  Serial.print(INA.getBusVoltage(), 2);
  Serial.print("hello");
   maxAmps= fabs((ACS.mA_AC_sampling(2,0.5)-130));


//tft.fillRect(0, 0, 287, 128, BLACK);
   // tft.setCursor(0, 10);
  
    Serial.print("Max Voltage: ");
    Serial.print(maxVoltage);
    Serial.println(" mV");
 if(fabs(PrevVolt-maxVoltage)>50){
//tft.fillRect(340, 70, 140, 60,BLACK);
    tft.setCursor(0, 10);
      tft.setTextColor(0x0000);
    tft.print(buffer);
 }
 if(fabs(PrevAmp-maxAmps)>20){
     tft.setCursor(0,70);
      tft.setTextColor(0x0000);
    tft.print(buffer2);
 }
 Maxpower= maxAmps*maxVoltage/1;
 if(fabs(PrevWatt-Maxpower)>=1){
    tft.setCursor(340,70);
      tft.setTextColor(0x0000);
    tft.print(buffer3);
 }
    //add
   // int voltageRead=applyPolynomial(maxVoltage);
    //int voltageRead=calculateY(maxVoltage);
    if(fabs(PrevVolt-maxVoltage)>50){
    Serial.println(maxVoltage);
    dtostrf(maxVoltage, 0, 0, buffer);
    strncat(buffer, "mV", 8);
   // tft.color565(255,0,0);
    tft.setTextColor(0xF800);
    tft.setCursor(0, 10);
    tft.print(buffer);
    Serial.println("Max amps");
  
    Serial.println(maxAmps);
    }

     if(fabs(PrevAmp-maxAmps)>20){
     dtostrf(maxAmps, 0, 0, buffer2);
    strncat(buffer2, "mA", 8);
     tft.setCursor(0,70);
     tft.setTextColor(0x001F);
    tft.print(buffer2);
     }
      if(fabs(PrevWatt-Maxpower)>=1){
 Serial.println(Maxpower);
 Serial.println("^^power");
 Serial.println(Maxpower/1000000);
dtostrf(Maxpower/1000000, 0, 0, buffer3);
    strncat(buffer3, "W", 8);
     tft.setCursor(340,70);
      tft.setTextColor(0xF81F);
    tft.print(buffer3);

      }


  if(Maxpower>PrevWatt&&Maxpower>bigpower){
    bigpower=Maxpower;
    tft.setCursor(340,10);
      tft.setTextColor(0x0000);
    tft.print(buffer4);
    dtostrf(bigpower/1000000, 0, 0, buffer4);
    strncat(buffer4, "W", 8);
     tft.setCursor(340,10);
      tft.setTextColor(0xFFFF);
    tft.print(buffer4);
  }
  // Update Display
    // int numBars = voltageRead / 54.1;  

    // for (int i = 0; i < numBars; i++) {
    //     tft.drawFastVLine(i, 200, 120, WHITE);
    // }

    // for (int i = numBars; i < SCREEN_WIDTH; i++) {
    //     tft.drawFastVLine(i, 200, 120, BLACK);
    // }
    
    int numBars = map(maxVoltage, 0, 20000, 0, 480);
    int numBars2 = map(maxAmps, 0, 5000, 0, 480);
    int numBars3 = (map(Maxpower/1000000, 0, 14, 0, 480));
    Serial.println(numBars3);

for (int i = map(PrevVolt, 0, 20000, 0, 480); i < numBars; i++) {
    tft.drawFastVLine(i, 200, 60, 0xF800);  // Draw bar in white
}
for (int i = map(PrevVolt, 0, 20000, 0, 480); i > numBars; i--) {
    tft.drawFastVLine(i, 200, 60, BLACK);  // Draw bar in white
}
   

for (int i = map(PrevAmp, 0, 5000, 0, 480); i < numBars2; i++) {
    tft.drawFastVLine(i, 260, 60, 0x001F);  // Draw bar in white
}
for (int i = map(PrevAmp, 0, 5000, 0, 480); i > numBars2; i--) {
    tft.drawFastVLine(i, 260, 60, BLACK);  // Draw bar in white
}


for (int i = (map(PrevWatt/1000000, 0, 14, 0, 480)); i < numBars3; i++) {
    tft.drawFastVLine(i, 140, 60, 0xF81F);  // Draw bar in white
}
for (int i = (map(PrevWatt/1000000, 0, 14, 0, 480)); i > numBars3; i--) {
    tft.drawFastVLine(i, 140, 60, BLACK);  // Draw bar in white
}


//delay(500);
    

      // Adjust refresh rate
      Serial.println();
      Serial.println();


Serial.println(buffer);
Serial.println(buffer2);
Serial.println(buffer3);
Serial.println();
Serial.println();
}
