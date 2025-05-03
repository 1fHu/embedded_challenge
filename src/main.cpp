#include <Adafruit_CircuitPlayground.h>
#include <Arduino.h>
#include <arduinoFFT.h>


// not larger than 128, power of 2
#define SAMPLES 128

#define SENSOR_RANGE 20
#define SAMPLING_FREQ 43.47 // Hz, determined by last delay in loop 


// variables in FFT
float X, Y, Z;
double real[SAMPLES], imag[SAMPLES];

// FFT itself
ArduinoFFT<double> FFT;

// iteration related
bool endFlag = false;
int i = 0;

// button control
const int buttonPin = PD4; // Right button connected to PD4
bool lastButtonState = HIGH;  // Assume unpressed state initially (pull-up)
bool currentButtonState = HIGH;
bool buttonPressed = false;


// function prototype
double getOneSampleData(); 
void performFFT(double* real, double* imag);
void calculateEnergy(double& tremorEnergy, double& dyskinesiaEnergy);
void determineLED(const double& tremorEnergy, const double& dyskinesiaEnergy);
void restart();
void determineButton();


void setup() {
  CircuitPlayground.begin();
  CircuitPlayground.clearPixels();

  pinMode(13, OUTPUT); // LED control

  pinMode(buttonPin, INPUT_PULLUP); // Left button

}

void loop() {
  // run repeatly
  // here is data collecting

  // enter data process when sample is reached
  if (!endFlag && i == SAMPLES) {
    // turn off LED shows 
    digitalWrite(13, LOW);
    delay(10);

    performFFT(real, imag);

    // calculate energy in tremor and dyskinesia bands
    double tremorEnergy = 0;
    double dyskinesiaEnergy = 0; 

    calculateEnergy(tremorEnergy, dyskinesiaEnergy);

    determineLED(tremorEnergy,  dyskinesiaEnergy);
    endFlag = true;
  }

  while (endFlag){
    determineButton();
  }

  // LED indicate
  digitalWrite(13, HIGH);

  // add one sample data into data array
  double mag = getOneSampleData();
  real[i] = mag;
  imag[i] = 0;


  // record next time
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


void calculateEnergy(double& tremorEnergy, double& dyskinesiaEnergy){
  for (int j = 1; j < SAMPLES / 2; j++){
    double freq = (j * SAMPLING_FREQ) / SAMPLES;

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

void restart(){
  i = 0;
  CircuitPlayground.clearPixels();
  endFlag = false;
}


void determineButton(){
  currentButtonState = digitalRead(buttonPin);

  if (lastButtonState == HIGH && currentButtonState == LOW) {
    // Button just pressed
    buttonPressed = true;
  } 
  else if (lastButtonState == LOW && currentButtonState == HIGH && buttonPressed) {
    // Button released after being pressed
    restart();
    buttonPressed = false; // Reset flag

  }

  lastButtonState = currentButtonState;
}