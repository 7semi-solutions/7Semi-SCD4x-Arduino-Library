 /***************************************************************
 * @file    Periodic_Compensated.ino
 * @brief   Example for running the 7Semi SCD4x sensor in 
 *          periodic measurement mode with compensation settings.
 *
 * Features demonstrated:
 *  - Temperature offset (board self-heating)
 *  - Installation altitude (m)
 *  - Ambient pressure raw 
 *
 * Sensor configuration used:
 * - Mode            : Periodic (2 s polling interval)
 * - Temperature Offs: +1.50 °C (default constant in example)
 * - Altitude        : 30 m (default constant in example)
 * - Ambient Pressure: Optional (disabled by default, PRESS_RAW=-1)
 * - I²C Frequency   : 100 kHz (recommended for bring-up)
 * - Data Output     : CO₂ (ppm), Temperature (°C), RH (%)
 *
 * Connections:
 * - SDA -> A4 Default board SDA (or define custom pin)
 * - SCL -> A5  Default board SCL (or define custom pin)
 * - VIN -> 3.3V / 5V (depending on module)
 * - GND -> GND
 *
 * @author   7Semi
 * @license  MIT
 * @version  1.0
 * @date     24 September 2025
 ***************************************************************/

#include <7Semi_SCD4x.h>

// I2C speed for bring-up (100 kHz recommended; raise later if stable)
#define I2C_ADDR 0x62
#define I2C_FREQ_HZ 100000UL

// Set to -1 to use board defaults
#define I2C_SDA_PIN -1
#define I2C_SCL_PIN -1

SCD4x_7Semi scd;

const float TEMP_OFFSET_C = 1.50f;
const uint16_t ALTITUDE_M = 30;
/**
 * Optional ambient pressure (raw units per Sensirion spec).
 * Set to >= 0 to apply; keep -1 to skip.
 */
const int PRESS_RAW = -1;

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;

  Serial.println(F("7Semi SCD4x\n Periodic with compensation"));

  // begin(sda, scl, freq). For default MCUs I2C, keep sda and scl -1.
  while (!scd.begin()) {
  // while (!scd.begin(I2C_ADDR,I2C_SDA_PIN, I2C_SCL_PIN, I2C_FREQ_HZ)) {
    Serial.println(F("Sensor not detected."));
    delay(1000);
  }

  // ---- Compensation settings ----
  if (!scd.setTemperatureOffset(TEMP_OFFSET_C))
    Serial.println(F("setTemperatureOffset failed"));

  if (!scd.setSensorAltitude(ALTITUDE_M))
    Serial.println(F("setSensorAltitude failed"));

  if (PRESS_RAW >= 0 && !scd.setAmbientPressureRaw((uint16_t)PRESS_RAW))
    Serial.println(F("setAmbientPressureRaw failed"));

  scd.setAutomaticSelfCalibrationEnabled(true);

  if (!scd.startPeriodicMeasurement()) {
    Serial.println(F("startPeriodicMeasurement failed"));
    while (1) delay(1000);
  }
  Serial.println(F("Started. First valid reading in ~5s."));
}
/**
 * - Poll data-ready every ~2 s
 * - When ready, read CO₂ (ppm), Temperature (°C), RH (%)
 * - Print compact line
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


