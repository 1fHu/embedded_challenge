# Shake, Rattle, and Roll: Detection of Parkinsonian Symptoms

This project was developed as part of the Embedded Challenge in **Embedded System Design (ECE-UY 4144)** at NYU Tandon during Spring 2025.  
All rights reserved by the authors.

## Overview

This embedded system detects **Parkinsonian tremors** and **dyskinesia** using a 3-axis accelerometer onboard the **Adafruit Circuit Playground Classic**. By applying a **Fast Fourier Transform (FFT)** to 3 seconds of motion data, the system distinguishes between two key frequency bands:

- **3–5 Hz**: Parkinsonian tremors (displayed with **blue LEDs**)
- **5–7 Hz**: Dyskinesia (displayed with **red LEDs**)

The number of LEDs lit corresponds to the **intensity** of the detected symptoms.

## Features

- **Real-time frequency analysis** using FFT  
- **Blue LEDs** (positions 0–4) indicate tremor intensity  
- **Red LEDs** (positions 5–9) indicate dyskinesia intensity  
- **Both red and blue LEDs** light up if intensities are equal  
- A **small red LED** (pin 13) indicates data collection  
- **Left button** (PD4) restarts the detection cycle  

## How It Works

1. Collects **128 samples over 3 seconds** 
2. Calculates the **magnitude** of acceleration using all three axes
3. Applies **FFT** to analyze the frequency spectrum
4. Measures energy in 3–5 Hz and 5–7 Hz bands
5. Displays detection results with onboard **LEDs**
6. Pressing the **left button** resets the test


## License

This project is for academic use only.  
**Copyright © 2025, Simon Hu and contributors**
