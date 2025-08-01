/*!
 * @file 7semi_SCD40.h
 * @brief Arduino wrapper for Sensirion SCD40 CO₂ sensor.
 *
 * This lightweight wrapper uses the official Sensirion I2C SCD4x driver and adds:
 * - Serial number verification
 * - Single-shot read mode
 * - Basic configuration options
 *
 *  Based on: Sensirion embedded-scd driver
 */

#ifndef _7SEMI_SCD40_H_
#define _7SEMI_SCD40_H_

#include <Arduino.h>
#include <Wire.h>
#include "SensirionI2cScd4x.h"

#define Serial_number 0x1019EB073B70  ///< Expected serial number for verification

class SCD40 {
  public:
    SCD40();

    /**
     * @brief Initialize sensor and verify serial number.
     */
    bool begin(TwoWire& wirePort = Wire);

    /**
     * @brief Start periodic measurement.
     */
    bool start();

    /**
     * @brief Stop periodic measurement.
     */
    bool stop();

    /**
     * @brief Perform a single-shot CO₂, temperature, and humidity measurement.
     */
    bool readSingleShot(uint16_t& co2, float& temperature, float& humidity);

    /**
     * @brief Read the sensor's 48-bit serial number.
     */
    bool getSerialNumber(uint64_t& serial);

    /**
     * @brief Set temperature compensation offset in °C.
     */
    bool setTemperatureOffset(float offsetC);

    /**
     * @brief Get current temperature compensation offset.
     */
    bool getTemperatureOffset(float& offsetC);

    /**
     * @brief Set ambient pressure for CO₂ compensation in mBar.
     */
    bool setAmbientPressure(uint16_t mBar);

    // (Optional, not used in .cpp currently)
    // bool getAmbientPressure(uint16_t& mBar);

    /**
     * @brief Enable or disable automatic self-calibration.
     */
    bool enableAutomaticSelfCalibration(bool enable);

    /**
     * @brief Reset the sensor to factory default settings.
     */
    bool performFactoryReset();

    /**
     * @brief Reinitialize sensor (loads from EEPROM).
     */
    bool reinit();

    // (Optional raw Pa setter if needed)
    // bool setPressure(uint32_t pressure);

  private:
    TwoWire* _wire;
    SensirionI2cScd4x _scd4x;
};

#endif  // _7SEMI_SCD40_H_
