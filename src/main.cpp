#include <Adafruit_CircuitPlayground.h>
#include <Arduino.h>
#include <arduinoFFT.h>


float X, Y, Z;

// not larger than 128, power of 2
#define SAMPLES 128

#define SENSOR_RANGE 20
#define SAMPLING_FREQ 43.47 // Hz, determined by last delay in loop 

// variables in FFT
double real[SAMPLES], imag[SAMPLES];

// FFT itself
ArduinoFFT<double> FFT;

bool endFlag = false;
int i = 0;

// function prototype
double getOneSampleData(); 
void performFFT(double* real, double* imag);
void calculateEnergy(double& tremorEnergy, double& dyskinesiaEnergy,
  double* testPtr);
void determineLED(const double& tremorEnergy, const double& dyskinesiaEnergy);

void setup() {
  // Serial.begin(9600);
  // while (!Serial);  
  CircuitPlayground.begin();

  CircuitPlayground.clearPixels();
}

void loop() {


  if (!endFlag && i == SAMPLES) {

    performFFT(real, imag);

    // calculate energy in tremor and dyskinesia bands
    double tremorEnergy = 0;
    double dyskinesiaEnergy = 0; 
    double testEnergy = 0;
    double* testPtr = &testEnergy;
    // calculateEnergy(tremorEnergy, dyskinesiaEnergy);
    calculateEnergy(tremorEnergy, dyskinesiaEnergy, testPtr);

    // Serial.print("Current Energy: ");
    // Serial.println(testEnergy); 
    // Serial.print("tremorEnergy Energy: ");
    // Serial.println(tremorEnergy); 
    // Serial.print("dyskinesiaEnergy Energy: ");
    // Serial.println(dyskinesiaEnergy); 

    determineLED( tremorEnergy,  dyskinesiaEnergy);
    endFlag = true;
  }

  if (endFlag) return;


  // normal code
  double mag = getOneSampleData();

  // add into data array
  real[i] = mag;
  imag[i] = 0;

  i++; 
  delay(23); // determines Sampling frequency

}

double getOneSampleData(){
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
  
  return mag;
}


void performFFT(double* real, double* imag){
  FFT.windowing(real, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.compute(real, imag, SAMPLES, FFT_FORWARD);
  FFT.complexToMagnitude(real, imag, SAMPLES);
}

void calculateEnergy(double& tremorEnergy, double& dyskinesiaEnergy,
                     double* testPtr){
  for (int j = 1; j < SAMPLES / 2; j++){
    double freq = (j * SAMPLING_FREQ) / SAMPLES;
    // if (testPtr != nullptr){
    //   if (freq >= 3 && freq <= 7){
    //     *testPtr += real[j];
    //   }
    // }

    if (freq >= 3 && freq <= 5) {
      tremorEnergy += real[j];
    } else if (freq > 5 && freq <= 7){
      dyskinesiaEnergy += real[j];
    }
  }
}

// not consider when same number
void determineLED(const double& tremorEnergy, const double& dyskinesiaEnergy){
  if (tremorEnergy < 2 && dyskinesiaEnergy < 2){
    return;
  }
  else if (tremorEnergy > dyskinesiaEnergy)
  { 
    CircuitPlayground.setPixelColor(0, 0, 0, 255); // set it blue
  }
  else if (tremorEnergy < dyskinesiaEnergy)
  {
    CircuitPlayground.setPixelColor(5, 255, 0, 0); // set it red
  }
  
}