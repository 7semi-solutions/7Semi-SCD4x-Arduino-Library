/***************************************************************
 * @file    Basic.ino
 * @brief   Example for using the 7Semi SCD4x CO₂, temperature,
 *          and humidity sensor with I²C on ESP32/Arduino.
 *
 * Features demonstrated:
 * - Start/stop of standard periodic measurement
 * - Reading CO₂ (ppm), temperature (°C), and humidity (%RH)
 * - Notes on single-shot mode, offsets, and airflow considerations
 *
 * Sensor configuration used:
 * - Periodic Mode : Standard (2 s polling)
 * - I²C Frequency : 100 kHz (recommended for bring-up)
 * - Supply Voltage: 3.3V–5.5V (module dependent)
 * - Data Output   : CO₂ (ppm), Temperature (°C), RH (%)
 *
 * Connections:
 * - SDA -> A4 Default board SDA (or define custom pin)
 * - SCL -> A5 Default board SCL (or define custom pin)
 * - VIN -> 3.3V / 5V (depending on module)
 * - GND -> GND
 ***************************************************************/

#include <7Semi_SCD4x.h>

// I2C speed for bring-up (100 kHz recommended; raise later if stable)
#define I2C_ADDR 0x62
#define I2C_FREQ_HZ 100000UL

// Set to -1 to use board defaults
#define I2C_SDA_PIN -1
#define I2C_SCL_PIN -1

// Polling period while in periodic modes
#define POLL_MS 2000UL

SCD4x_7Semi scd(&Wire);

uint32_t lastPoll = 0;
bool running = false;
bool lowPower = false;

void setup() {
  Serial.begin(115200);
  while (!Serial) {}

  Serial.println(F("\n== 7Semi SCD4x =="));

  while (!scd.begin(I2C_ADDR,I2C_SDA_PIN, I2C_SCL_PIN, I2C_FREQ_HZ)) {
    Serial.println(F("SCD4x not detected. Check wiring/power."));
    delay(1000);
  }

  // Start in standard periodic mode by default
  if (scd.startPeriodicMeasurement()) {
    running = true;
    lowPower = false;
    Serial.println(F("Started standard periodic mode. First valid reading ~5s."));
  } else {
    Serial.println(F("failed to start periodic mode."));
  }
}
void loop() {
  static uint32_t lastCheck = 0;
  if (millis() - lastCheck < 2000) return;  // every 2 s
  lastCheck = millis();

  uint16_t ready = 0;
  // Bit 10..0 used as "new data available" in many Sensirion examples
  if (scd.getDataReadyStatus(ready) && (ready & 0x07FF)) {
    uint16_t co2;
    float temp, rh;

    if (scd.readMeasurement(co2, temp, rh)) {
      Serial.print("CO₂: ");
      Serial.print(co2);
      Serial.print(" ppm\t");

      Serial.print("Temp: ");
      Serial.print(temp, 2);
      Serial.print(" °C\t");

      Serial.print("RH: ");
      Serial.print(rh, 1);
      Serial.println(" %");
    } else {
      Serial.println("Read failed (CRC/I2C error)");
    }
  }
}

/* ===================== Notes =====================
 - If you never see data ready:
   * Re-check wiring, pull-ups, and I²C pins for your board.
   * Try 100 kHz I²C during bring-up.
   * Ensure stable power; SCD4x can run from 3.3–5.5 V depending on module.

 - If values look offset:
   * Consider setTemperatureOffset() to compensate board self-heating.

 - If you need single-shot mode:
   * Use measureSingleShot()/measureSingleShotRhtOnly() and then readMeasurement().
=================================================== */
