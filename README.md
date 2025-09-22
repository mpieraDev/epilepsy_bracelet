# epilepsy_bracelet

## 🛠 Hardware Components

The system was built and tested using the following components:

- **Seeed XIAO nRF52840 Sense BLE** (main microcontroller with BLE support and built-in IMU)  
- **MAX30102 Pulse Oximeter & Heart Rate Sensor** (for heart rate and SpO2 monitoring)

---

## 📦 Required Libraries

### ✅ Available in Arduino Library Manager
- **SparkFun LSM6DS3 IMU** (`LSM6DS3.h`)  
  *Name in Library Manager:* `SparkFun LSM6DS3 Breakout - 6 DoF IMU` (author: SparkFun)

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

### 🔁 Requires manual installation (ZIP)
- **Epilepsy_inferencing.h**  
  This header is generated from an **Edge Impulse project**.  
  - Download the Arduino library ZIP from Edge Impulse (*Deployment → Arduino library*).  
  - In Arduino IDE: `Sketch → Include Library → Add .ZIP Library…` and select the downloaded ZIP.

---

> 💡 Tip: After installing, check `Sketch → Include Library` in Arduino IDE to confirm all libraries are available.

