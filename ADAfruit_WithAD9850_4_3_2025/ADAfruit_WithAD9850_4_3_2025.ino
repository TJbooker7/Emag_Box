 #define W_CLK 13       // Pin 6 - connect to AD9850 module word load clock pin (CLK)
 #define FQ_UD 8       // Pin 5 - connect to freq update pin (FQ)
 #define DATA 11       // Pin 7 - connect to serial data load pin (DATA)
 #define RESET 9      // Pin 4 - connect to reset pin (RST).
 #include <avr/power.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
 #define pulseHigh(pin) {digitalWrite(pin, HIGH); digitalWrite(pin, LOW); }
 #ifndef PSTR 
 #define PSTR 
#endif

#define PIN 6

int c = 255; 


Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(1,   1, 8, 24, PIN,
  NEO_TILE_TOP + NEO_TILE_LEFT + NEO_TILE_ROWS + NEO_TILE_PROGRESSIVE +
  NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
  NEO_GRB + NEO_KHZ800);
 // transfers a byte, a bit at a time, LSB first to the 9850 via serial DATA line
// Adafruit_NeoMatrix matrix2 = Adafruit_NeoMatrix(1,   1, 8, 8, PIN,
//   NEO_TILE_TOP + NEO_TILE_LEFT + NEO_TILE_ROWS + NEO_TILE_PROGRESSIVE +
//   NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
//   NEO_GRB + NEO_KHZ800);

 const uint16_t colors[] = {
  matrix.Color(255, 0, 0), // Red
  matrix.Color(0, 255, 0), // Green
  matrix.Color(0, 0, 255)  // Blue
};

void tfr_byte(byte data)
{
  for (int i=0; i<8; i++, data>>=1) {
    digitalWrite(DATA, data & 0x01);
    pulseHigh(W_CLK);   //after each bit sent, CLK is pulsed high
     delayMicroseconds(1);  // Small delay for stability
  }
}
 
 // frequency calc from datasheet page 8 = <sys clock> * <frequency tuning word>/2^32
void sendFrequency(double frequency) {
  //int32_t freq = frequency * 4294967295 / 100000000;  // Use 100 MHz instead

  int32_t freq = frequency * 4294967295/125000000;  // note 125 MHz clock on 9850
  for (int b=0; b<4; b++, freq>>=8) {
    tfr_byte(freq & 0xFF);
  }
  //tfr_byte(0x000 | 0x01);  // Explicitly enable Sine wave mode (LSB bit set)
tfr_byte(0x000);   // Final control byte, all 0 for 9850 chip
  pulseHigh(FQ_UD);  // Done!  Should see output
}
 
 int sensorPin = A0;
float voltage;

void setup() {
  Serial.begin(115200); // Initialize serial communication
  
  matrix.begin(); // Initialize matrix
  matrix.clear(); // Clear matrix display
  matrix.setBrightness(100); // Set brightness level
  matrix.show(); // Refresh display
 // configure arduino data pins for output
  pinMode(FQ_UD, OUTPUT);
  pinMode(W_CLK, OUTPUT);
  pinMode(DATA, OUTPUT);
  pinMode(RESET, OUTPUT);
 
  pulseHigh(RESET);
delay(10);
pulseHigh(RESET);
delay(10);
pulseHigh(RESET);

  pulseHigh(W_CLK);
  pulseHigh(FQ_UD);  // this pulse enables serial mode - Datasheet page 12 figure 10
}
 
void loop() {
  sendFrequency(400000);  // freq

 c=255;
int adcValue=0;
  while(1){
  adcValue = analogRead(sensorPin);
 voltage = adcValue * (5 / 1023.0);


 Serial.print("Instantaneous Voltage: ");
 Serial.print(voltage, 3);
 Serial.println(" V");
   
     updateLEDs(voltage,c);
matrix.show();
delay(10);
//delay(100);
  }
  
  }

  void updateLEDs(float voltage, uint16_t c) {
    int rowsToLight = (voltage-.37) / 0.19; // Determine how many rows should be lit

    for (int y = 0; y < 24; y++) {
        for (int x = 0; x < 8; x++) {
            if (y < rowsToLight) {
                matrix.drawPixel(x, y, c);  // Turn on the pixel
            } else {
                matrix.drawPixel(x, y, 0);  // Turn off the pixel
            }
        }
    }
}

