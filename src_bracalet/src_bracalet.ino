/* Edge Impulse Arduino examples
 * Copyright (c) 2021 EdgeImpulse Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/* Includes ---------------------------------------------------------------- */
#include <Epilepsy_inferencing.h>
#include <LSM6DS3.h>
#include <U8g2lib.h>
#include <U8X8lib.h>
#include <Wire.h>
#include <ArduinoBLE.h>
#include "MAX30105.h"
#include "heartRate.h"
/* Constant defines -------------------------------------------------------- */
#define CONVERT_G_TO_MS2    9.80665f
#define MAX_ACCEPTED_RANGE  2.0f        // starting 03/2022, models are generated setting range to +-2, but this example use Arudino library which set range to +-4g. If you are using an older model, ignore this value and use 4.0f instead

/*
 ** NOTE: If you run into TFLite arena allocation issue.
 **
 ** This may be due to may dynamic memory fragmentation.
 ** Try defining "-DEI_CLASSIFIER_ALLOCATION_STATIC" in boards.local.txt (create
 ** if it doesn't exist) and copy this file to
 ** `<ARDUINO_CORE_INSTALL_PATH>/arduino/hardware/<mbed_core>/<core_version>/`.
 **
 ** See
 ** (https://support.arduino.cc/hc/en-us/articles/360012076960-Where-are-the-installed-cores-located-)
 ** to find where Arduino installs cores on your machine.
 **
 ** If the problem persists then there's not enough memory for this model and application.
 */

U8X8_SSD1306_64X48_ER_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE); 

/* Private variables ------------------------------------------------------- */
static bool debug_nn = false; // Set this to true to see e.g. features generated from the raw signal
LSM6DS3 myIMU(I2C_MODE, 0x6A);
/**
* @brief      Arduino setup function
*/

/*const int RED_ledPin =  11;
const int BLUE_ledPin =  12;
const int GREEN_ledPin =  13;*/ 

//Conection
//---------------------------------------------------------------------------------------------------------------
BLEService AlertService("19b10000-e8f2-537e-4f6c-d104768a1214"); // Service UUID
BLEFloatCharacteristic CombulsionCharacteristic("19b10001-e8f2-537e-4f6c-d104768a1214", BLERead | BLEWrite | BLENotify);
BLEFloatCharacteristic WalkingCharacteristic("19b10002-e8f2-537e-4f6c-d104768a1214", BLERead | BLEWrite | BLENotify);
BLEFloatCharacteristic NaturalCharacteristic("19b10003-e8f2-537e-4f6c-d104768a1214", BLERead | BLEWrite | BLENotify);
BLEFloatCharacteristic StaticCharacteristic("19b10004-e8f2-537e-4f6c-d104768a1214", BLERead | BLEWrite | BLENotify);
BLEFloatCharacteristic BpmCharacteristic("19b10005-e8f2-537e-4f6c-d104768a1214", BLERead | BLEWrite | BLENotify);
BLEFloatCharacteristic EmergencyCharacteristic("19b10006-e8f2-537e-4f6c-d104768a1214", BLERead | BLEWrite | BLENotify);
BLEFloatCharacteristic RunningCharacteristic("19b10007-e8f2-537e-4f6c-d104768a1214", BLERead | BLEWrite | BLENotify);
BLEFloatCharacteristic ClappingCharacteristic("19b10008-e8f2-537e-4f6c-d104768a1214", BLERead | BLEWrite | BLENotify);
//---------------------------------------------------------------------------------------------------------------

//BPM
//---------------------------------------------------------------------------------------------------------------
MAX30105 particleSensor;

const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred

float beatsPerMinute;
float beatAvg;
//---------------------------------------------------------------------------------------------------------------

int count = 0;
float getingCombulsion = 0;

float Avgbpm[19];
float mitjanaBpm;
int sum = 0;

long timer;

bool Utimer = false;

float Combulsion; 
float Walking; 
float Clapping;
float Natural;
float Static;
float Running; 

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(115200);
    //u8g2.begin();
    u8x8.begin();
    Serial.println("Edge Impulse Inferencing Demo");

    //if (!IMU.begin()) {
      if (!myIMU.begin()) {
        ei_printf("Failed to initialize IMU!\r\n");
    }
    else {
        ei_printf("IMU initialized\r\n");
    }

    if (EI_CLASSIFIER_RAW_SAMPLES_PER_FRAME != 3) {
        ei_printf("ERR: EI_CLASSIFIER_RAW_SAMPLES_PER_FRAME should be equal to 3 (the 3 sensor axes)\n");
        return;
    }

    //Setup conection
    //---------------------------------------------------------------------------------------------------------------
      if (!BLE.begin()) {                                           
        while (1);                                                  
      }
 
      BLE.setLocalName("LifeLink");                                  
      BLE.setAdvertisedService(AlertService);                         
      AlertService.addCharacteristic(CombulsionCharacteristic);             
      AlertService.addCharacteristic(WalkingCharacteristic);           
      AlertService.addCharacteristic(NaturalCharacteristic); 
      AlertService.addCharacteristic(StaticCharacteristic); 
      AlertService.addCharacteristic(BpmCharacteristic); 
      AlertService.addCharacteristic(EmergencyCharacteristic); 
      AlertService.addCharacteristic(ClappingCharacteristic); 
      AlertService.addCharacteristic(RunningCharacteristic);    
      BLE.addService(AlertService);                                  
      BLE.advertise();                                              
    //---------------------------------------------------------------------------------------------------------------

    //BPM
    //---------------------------------------------------------------------------------------------------------------
    // Initialize sensor
    if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
    {
      Serial.println("MAX30105 was not found. Please check wiring/power. ");
      while (1);
    }
    Serial.println("Place your index finger on the sensor with steady pressure.");

    particleSensor.setup(); //Configure sensor with default settings
    particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
    particleSensor.setPulseAmplitudeGreen(0); //Turn off Green LED
    //---------------------------------------------------------------------------------------------------------------

    pinMode(LED_BUILTIN, OUTPUT);
}

/**
 * @brief Return the sign of the number
 * 
 * @param number 
 * @return int 1 if positive (or 0) -1 if negative
 */
float ei_get_sign(float number) {
    return (number >= 0.0) ? 1.0 : -1.0;
}

/**
* @brief      Get data and run inferencing
*
* @param[in]  debug  Get debug info if true
*/
void loop()
{
  if(Utimer == false){
    timer = millis();
    Utimer = true;
  }

if(millis() - timer > 500) {
    uint8_t buf1[64]="Comvulsion";
    uint8_t buf2[64]="walking";
    uint8_t buf3[64]="natural";
    uint8_t buf4[64]="static";
    uint8_t buf5[64]="dancingTek";
    uint8_t buf6[64]="running";
    uint8_t buf7[64]="clapping";
    
    u8x8.clear();
    u8x8.setFont(u8g2_font_ncenB08_tr);
  
    ei_printf("\nStarting inferencing...\n");

    ei_printf("Sampling...\n");

    // Allocate a buffer here for the values we'll read from the IMU
    float buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE] = { 0 };

    for (size_t ix = 0; ix < EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE; ix += 3) {
        // Determine the next tick (and then sleep later)
        //uint64_t next_tick = micros() + (EI_CLASSIFIER_INTERVAL_MS * 256);

buffer[ix] = myIMU.readFloatAccelX();
buffer[ix+1] = myIMU.readFloatAccelY();
buffer[ix+2] = myIMU.readFloatAccelZ();

        for (int i = 0; i < 3; i++) {
            if (fabs(buffer[ix + i]) > MAX_ACCEPTED_RANGE) {
                buffer[ix + i] = ei_get_sign(buffer[ix + i]) * MAX_ACCEPTED_RANGE;
            }
        }

        buffer[ix + 0] *= CONVERT_G_TO_MS2;
        buffer[ix + 1] *= CONVERT_G_TO_MS2;
        buffer[ix + 2] *= CONVERT_G_TO_MS2;

        //delayMicroseconds(next_tick - micros());
    }

    // Turn the raw buffer in a signal which we can the classify
    signal_t signal;
    int err = numpy::signal_from_buffer(buffer, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, &signal);
    if (err != 0) {
        ei_printf("Failed to create signal from buffer (%d)\n", err);
        return;
    }

    // Run the classifier
    ei_impulse_result_t result = { 0 };

    err = run_classifier(&signal, &result, debug_nn);
    if (err != EI_IMPULSE_OK) {
        ei_printf("ERR: Failed to run classifier (%d)\n", err);
        return;
    }

    // print the predictions
    ei_printf("Predictions ");
    ei_printf("(DSP: %d ms., Classification: %d ms., Anomaly: %d ms.)",
        result.timing.dsp, result.timing.classification, result.timing.anomaly);
    ei_printf(": \n");
    for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
        ei_printf("    %s: %.5f\n", result.classification[ix].label, result.classification[ix].value);
    }
#if EI_CLASSIFIER_HAS_ANOMALY == 1
    ei_printf("    anomaly score: %.3f\n", result.anomaly);
#endif

  if (result.classification[1].value > 0.03) {      
      Serial.println("OK");
      ++count;
   }

  if (result.classification[1].value < 0.03 && count > 0) {
      --count;
   }

  if (count > 4) {//&& beatAvg > (150 * mitjanaBpm) / 100) {
      digitalWrite(LED_BUILTIN, LOW);
      getingCombulsion = 1.0;
      Serial.println("LED ON");
   }

  if (count == 0) {
      digitalWrite(LED_BUILTIN, HIGH);
      getingCombulsion = 0.0;
   }

   Combulsion = result.classification[1].value;
   Walking = result.classification[5].value;
   Clapping = result.classification[0].value;
   Natural = result.classification[2].value;
   Static = result.classification[4].value;
   Running =  result.classification[3].value;
   Serial.println(count);
   
Utimer = false;
}
  //BPM
  //---------------------------------------------------------------------------------------------------------------
  else {
  long irValue = particleSensor.getIR();

  if (checkForBeat(irValue) == true)
  {
    //We sensed a beat!
    long delta = millis() - lastBeat;
    lastBeat = millis();

    beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute < 255 && beatsPerMinute > 20)
    {
      rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
      rateSpot %= RATE_SIZE; //Wrap variable

      //Take average of readings
      beatAvg = 0;
      for (byte x = 0 ; x < RATE_SIZE ; x++)
        beatAvg += rates[x];
      beatAvg /= RATE_SIZE;
    }
    BPM();
  }
  
  Serial.print("IR=");
  Serial.print(irValue);
  Serial.print(", BPM=");
  Serial.print(beatsPerMinute);
  Serial.print(", Avg BPM=");
  Serial.print(beatAvg);
  
  if (irValue < 50000)
    Serial.print(" No finger?");

  Serial.println();
  }
  //---------------------------------------------------------------------------------------------------------------

    //conection loop
    //---------------------------------------------------------------------------------------------------------------
      BLEDevice central = BLE.central();                          

      if (!central) {
        Serial.println("TryingConection"); 
      }
 
      if (central) {                                                

        Serial.println("Conected");                                                         
                                                                
        if (central.connected()) {   
        Serial.println("Conected...");                                                                                                         
        CombulsionCharacteristic.writeValue(Combulsion);
        WalkingCharacteristic.writeValue(Walking);
        NaturalCharacteristic.writeValue(Natural);
        StaticCharacteristic.writeValue(Static);
        ClappingCharacteristic.writeValue(Clapping);
        RunningCharacteristic.writeValue(Running);
        BpmCharacteristic.writeValue(beatAvg);
        EmergencyCharacteristic.writeValue(getingCombulsion);      
      }
    }                                                
    //--------------------------------------------------------------------------------------------------------------- 
}

void BPM() {
  Avgbpm[sum] = beatAvg;
  ++sum;

  if(sum = 19) {
    Arrange();
    mitjanaBpm = (Avgbpm[9] + Avgbpm[10]) / 2;
    sum = 0;
  }
}

void Arrange() {     
  int menor;

  for(int i = 0; i < 19; i++){
    menor = Avgbpm[0];

    if (Avgbpm[i] < menor){
      menor = Avgbpm[i];
    }

    else{
      if (Avgbpm[i] > menor){
        menor = menor;
      }      
    }
  }
}
