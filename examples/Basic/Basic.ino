/*!
 * @file SCD40_Sample.ino
 * @brief Example for reading CO₂, temperature, and humidity from SCD40 sensor using 7semi_SCD40 wrapper library.
 *
 * This example uses the Sensirion SCD40 sensor via the 7semi_SCD40 Arduino library.
 * The library internally uses the official Sensirion I2C driver for SCD4x (SCD40/SCD41),
 * with wrapper functions for easy initialization and single-shot readout.
 *
 *  NOTE:
 * - SCD40 does NOT measure pressure; ambient pressure must be set manually for compensation.
 * - The sensor supports single-shot and periodic modes. This example uses single-shot mode.
 *
 *  Library credits:
 * - Sensirion official driver (https://github.com/Sensirion/embedded-scd)
 * - Arduino wrapper by 7semi 
 *
 *  Sensor specs:
 * - CO₂ accuracy: ±(40 ppm + 5%)
 * - Temp/Humidity accuracy: ±0.8 °C / ±3% RH
 * - Update rate: minimum 5 seconds (single-shot or periodic)
 */

#include <7semi_SCD40.h>
#include <Wire.h>

SCD40 scdSensor;

void setup() {
  Serial.begin(115200);
  // Initialize the sensor and verify serial number
  if (scdSensor.begin()) {
    Serial.println(" SCD40 sensor initialized.");
  } else {
    Serial.println(" SCD40 sensor failed to initialize.");
    while (1);  // Stop here on failure
  }
}

void loop() {
  uint16_t co2;
  float temp, rh;

  // Read one single-shot measurement from SCD40
  if (scdSensor.readSingleShot(co2, temp, rh)) {
    Serial.print("CO2: ");
    Serial.print(co2);
    Serial.print(" ppm | Temp: ");
    Serial.print(temp, 2);
    Serial.print(" °C | RH: ");
    Serial.print(rh, 2);
    Serial.println(" %");
  } else {
    Serial.println(" Failed to read measurement from SCD40.");
  }

  delay(5000);  // Must wait at least 5 seconds between reads
}

