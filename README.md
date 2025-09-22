# epilepsy_bracelet

We designed a wristband for epileptic seizure detection, linked to a mobile app capable of automatically alerting emergency services and selected contacts. We trained an AI model to recognize motion patterns associated with seizures, using an MCU with an integrated IMU and a heart rate sensor as a redundant system to reduce false positives.
In the initial tests, after one full day of continuous use, only one false positive was recorded, showing that the model can be affected by erratic movements similar to a seizure. This prototype represents an MVP that demonstrates the system’s viability. It is worth noting that the development of a fully reliable model requires a much larger dataset and telemetry from real seizure events.

## Hardware Components

The system was built and tested using the following components:

- **Seeed XIAO nRF52840 Sense BLE** (main microcontroller with BLE support and built-in IMU)  
- **MAX30102 Pulse Oximeter & Heart Rate Sensor** (for heart rate and SpO2 monitoring)

## Required Libraries

### Available in Arduino Library Manager
- **Seeed LSM6DS3 IMU** (`LSM6DS3.h`)  
  *Repository:* [Seeed_Arduino_LSM6DS3](https://github.com/Seeed-Studio/Seeed_Arduino_LSM6DS3)  
  > Installable directly from Arduino Library Manager as **Seeed Arduino LSM6DS3** (author: Seeed Studio).

- **U8g2** (`U8g2lib.h`, `U8X8lib.h`)  
  *Name in Library Manager:* `U8g2` (author: olikraus)  
  > Installing `U8g2` provides both `U8g2lib.h` and `U8X8lib.h`.

- **ArduinoBLE** (`ArduinoBLE.h`)  
  *Name in Library Manager:* `ArduinoBLE` (author: Arduino)

- **SparkFun MAX3010x Sensor Library** (`MAX30105.h`)  
  *Name in Library Manager:* `SparkFun MAX3010x Pulse and Proximity Sensor Library` (author: SparkFun)

- **SparkFun Heart Rate Algorithm** (`heartRate.h`)  
  *Name in Library Manager:* `SparkFun Heart Rate Algorithm` (author: SparkFun)

- **Wire** (`Wire.h`)  
  *Already included* in the Arduino core (no installation required).

### Custom / Manual Installation
- **Epilepsy_inferencing.h**  
  This is a **custom Edge Impulse Machine Learning library** trained specifically for this project.  
  - The header and its library files are already included in the repository under the `/Libraries` folder.
  - Install it in Arduino IDE via:  
  ```
  Sketch → Include Library → Add .ZIP Library…
  ```
### Gathered Data
All the raw data collected during the testing of the model can be consulted here: 

[Google Sheets – Epilepsy Bracelet Data](https://docs.google.com/spreadsheets/d/1n8vqtZVGNrSCoL_-IGgtQALlc3gfnVIjh_hOWvuD3p8/edit?usp=sharing)  
