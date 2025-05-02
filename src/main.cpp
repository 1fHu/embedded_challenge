#include <Adafruit_CircuitPlayground.h>
#include <Arduino.h>
#include <arduinoFFT.h>


float X, Y, Z;

// not larger than 128
#define SAMPLES 128

#define SENSOR_RANGE 20
#define SAMPLING_FREQ 100 // Hz, determined by last delay in loop 

// variables in FFT
double real[SAMPLES], imag[SAMPLES];

// FFT itself
ArduinoFFT<double> FFT;


// int16_t inputData[sample];
int i = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial);  
  CircuitPlayground.begin();
}

void loop() {

  
  X = CircuitPlayground.motionX();
  Y = CircuitPlayground.motionY();
  Z = CircuitPlayground.motionZ();


  // normalize data in each axis 
  float scaled_x = constrain(X / SENSOR_RANGE, -1.0, 1.0);
  float scaled_y = constrain(Y / SENSOR_RANGE, -1.0, 1.0);
  float scaled_z = constrain(Z / SENSOR_RANGE, -1.0, 1.0);

  // first combine, then one FFT
  double mag = sqrt(scaled_x * scaled_x + scaled_y * scaled_y +
                    scaled_z * scaled_z );

  real[i] = mag;
  imag[i] = 0;
  i++; 

  if (i == SAMPLES) {
    // perform FFT
    FFT.windowing(real, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.compute(real, imag, SAMPLES, FFT_FORWARD);
    FFT.complexToMagnitude(real, imag, SAMPLES);

    // calculate energy in tremor and dyskinesia bands
    double tremorEnergy = 0;
    double dyskinesiaEnergy = 0; 
    double testEnergy = 0;
    for (int j = 1; j < SAMPLES / 2; j++){
      double freq = (j * SAMPLING_FREQ) / SAMPLES;
      if (freq >= 3 && freq <= 7){
        testEnergy += real[j];
        Serial.print("Current Energy: ");
        Serial.println(testEnergy); 
      }

      // if (freq >= 3 && freq <= 5) {
      //   tremorEnergy += real[j];
      // } else if (freq > 5 && freq <= 7){
      //   dyskinesiaEnergy += real[j];
      // }
    }

    i = 0;
  }

  delay(10); // determines Sampling frequency of 100Hz

}
