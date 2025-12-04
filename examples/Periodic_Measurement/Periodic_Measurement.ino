/***************************************************************
 * @file    Periodic_Measurement.ino
 * @brief   Example for running the 7Semi SCD4x sensor in 
 *          standard periodic measurement mode.
 *
 * Features demonstrated:
 * - First valid sample: ~5 s after start
 * - Poll rate here: 2 s (matches typical update cadence)
 * - ASC enabled (optional; requires regular exposure to fresh air)
 *
 * Sensor configuration used:
 * - Mode            : Standard Periodic
 * - Poll Interval   : 2 s (matches sensor update cadence)
 * - ASC             : Enabled by default (requires fresh air exposure)
 * - I²C Frequency   : 100 kHz (recommended for bring-up)
 * - Data Output     : CO₂ (ppm), Temperature (°C), RH (%)
 *
 * Connections:
 * - SDA -> A4 Default board SDA (or custom pin if supported)
 * - SCL -> A5 Default board SCL (or custom pin if supported)
 * - VIN -> 3.3V / 5V (depending on module)
 * - GND -> GND
 *
 * @author   7Semi
 * @license  MIT
 * @version  1.0
 * @date     24 September 2025
 ***************************************************************/

#include <7Semi_SCD4x.h>

SCD4x_7Semi scd;  // default I2C bus (&Wire), device address 0x62

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;

  Serial.println(F("7Semi SCD4x\nPeriodic Standard"));

  // begin(sda, scl, freq). For fixed-pin MCUs, sda/scl are ignored.
  while (!scd.begin(-1, -1, 100000)) {
    Serial.println(F("Sensor not detected..."));
    delay(1000);
  }
  scd.setAutomaticSelfCalibrationEnabled(true);

  if (!scd.startPeriodicMeasurement()) {
    Serial.println(F("startPeriodicMeasurement failed"));
    while (1) delay(1000);
  }
  // if (!scd.startLowPowerPeriodicMeasurement()) {
  //   Serial.println(F("startLowPowerPeriodicMeasurement failed"));
  //   while (1) delay(1000);
  // }
}

/**
 * - Poll data-ready every ~2 s
 * - If ready, read CO₂ (ppm), Temperature (°C), RH (%)
 * - Print in a compact, stable format
 */
void loop() {
  static uint32_t t = 0;
  if (millis() - t < 2000) return;  // poll every ~2 s
  t = millis();

  uint16_t st = 0;
  // Many Sensirion refs use (status & 0x07FF) as "new data available"
  if (scd.getDataReadyStatus(st) && (st & 0x07FF)) {
    uint16_t co2;
    float tc, rh;
    if (scd.readMeasurement(co2, tc, rh)) {
      Serial.print(F("CO2 "));
      Serial.print(co2);
      Serial.print(F(" ppm  "));
      Serial.print(F("T "));
      Serial.print(tc, 2);
      Serial.print(F(" C  "));
      Serial.print(F("RH "));
      Serial.print(rh, 1);
      Serial.println(F(" %"));
    }
  }
}

/* ===================== Notes =====================
 - If you never get data-ready:
   * Recheck wiring, pull-ups, and I²C pins for your board.
   * Keep I²C at 100 kHz during bring-up.
   * Ensure stable power and short wires.

 - If accuracy drifts:
   * Disable ASC if the sensor never sees fresh air.
   * Consider forced recalibration in a known reference environment.
=================================================== */
