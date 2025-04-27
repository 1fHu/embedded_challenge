#include <Adafruit_CircuitPlayground.h>
#include "Adafruit_ZeroFFT.h"

float X, Y, Z;

//this must be a power of 2
#define DATA_SIZE 256

#define SENSOR_RANGE 20

int16_t inputData[DATA_SIZE];
int i = 0;

void setup() {
  Serial.begin(9600);
  CircuitPlayground.begin();
}

void loop() {

  
  X = CircuitPlayground.motionX();
  Y = CircuitPlayground.motionY();
  Z = CircuitPlayground.motionZ();

  // Serial.print("X: ");
  // Serial.print(X);
  // Serial.print("  Y: ");
  // Serial.print(Y);
  // Serial.print("  Z: ");
  // Serial.println(Z);



  // put inside the array inputData

  float raw = X;
  float scaled = raw / SENSOR_RANGE; // scale to -1.0 to 1.0

  // prevent overflow
  if (scaled > 1.0) scaled = 1.0;
  if (scaled < -1.0) scaled = -1.0;

  // int16_t range from -32768 to 32767, convert from float to int16_t
  int16_t q15_val = (int16_t)(scaled * 32767);

  inputData[i] = q15_val;
  i++;

  if (i == 128){
    ZeroFFT(inputData, DATA_SIZE);  //run the FFT
    i = 0;
  }
    delay(1000);

}