# epilepsy_bracelet

## 🛠 Hardware Components

The system was built and tested using the following components:

- **Seeed XIAO nRF52840 Sense BLE** (main microcontroller with BLE support and built-in IMU)  
- **MAX30102 Pulse Oximeter & Heart Rate Sensor** (for heart rate and SpO2 monitoring)

## 📦 Required Libraries

### ✅ Available in Arduino Library Manager
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

### 🔁 Custom / Manual Installation
- **Epilepsy_inferencing.h**  
  This is a **custom Edge Impulse Machine Learning library** trained specifically for this project.  
  - The header and its library files are already included in the repository under the `/Libraries` folder.  
  - If you want to regenerate it:  
    1. Go to [Edge Impulse](https://edgeimpulse.com).  
    2. Train your model and export it as an **Arduino Library**.  
    3. Install it in Arduino IDE via:  
       ```
       Sketch → Include Library → Add .ZIP Library…
       ```
