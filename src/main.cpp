#include <Adafruit_CircuitPlayground.h>
#include "Adafruit_ZeroFFT.h"

float X, Y, Z;

//this must be a power of 2
#define DATA_SIZE 256

#define SENSOR_RANGE 20

int16_t inputData[DATA_SIZE];

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

// capture 128

// put inside the array inputData
float raw = X;
float scaled = raw / SENSOR_RANGE;
if (scaled > 1.0) scaled = 1.0;
if (scaled < -1.0) scaled = -1.0;
int16_t q15_val = (int16_t)(scaled * 32767);
inputData[0] = q15_val;

  //run the FFT
  ZeroFFT(inputData, DATA_SIZE);


  delay(1000);
}