/*!
 * @file 7semi_SCD40.cpp
 * @brief Arduino wrapper implementation for Sensirion SCD40 CO₂ sensor.
 *
 * This wrapper uses the official Sensirion I2C driver to provide a simplified
 * interface for Arduino-style projects. It supports single-shot reading, serial
 * number verification, and basic configuration like temperature offset and ambient pressure.
 *
 *  Based on Sensirion's official driver: https://github.com/Sensirion/embedded-scd
 */

#include "7semi_SCD40.h"

SCD40::SCD40()
    : _wire(nullptr) {}

/**
 * @brief Initialize the SCD40 sensor and verify serial number.
 */
bool SCD40::begin(TwoWire &wirePort) {
    uint64_t sr_number = 0;
    _wire = &wirePort;
    _wire->begin();
    delay(100);

    _scd4x.begin(*_wire, 0x62);  // Default I2C address
    if (getSerialNumber(sr_number)) {
        if (sr_number == Serial_number) {
            _scd4x.stopPeriodicMeasurement();  // Ensure sensor is in idle mode
            delay(500);
            return true;
        }
    }
    return false;
}

/**
 * @brief Start periodic measurement mode.
 */
bool SCD40::start() {
    return (_scd4x.startPeriodicMeasurement() == 0);
}

/**
 * @brief Stop periodic measurement mode.
 */
bool SCD40::stop() {
    return (_scd4x.stopPeriodicMeasurement() == 0);
}

/**
 * @brief Perform single-shot CO2, temperature, and humidity measurement.
 */
bool SCD40::readSingleShot(uint16_t &co2, float &temperature, float &humidity) {
    int16_t error;
    error = _scd4x.wakeUp();
    if (error != 0) return false;

    error = _scd4x.measureSingleShot();
    if (error != 0) return false;

    error = _scd4x.measureAndReadSingleShot(co2, temperature, humidity);
    return (error == 0);
}

/**
 * @brief Get sensor serial number.
 */
bool SCD40::getSerialNumber(uint64_t &serial) {
    return (_scd4x.getSerialNumber(serial) == 0);
}

/**
 * @brief Set temperature compensation offset in °C.
 */
bool SCD40::setTemperatureOffset(float offsetC) {
    return (_scd4x.setTemperatureOffset(offsetC) == 0);
}

/**
 * @brief Get temperature compensation offset in °C.
 */
bool SCD40::getTemperatureOffset(float &offsetC) {
    return (_scd4x.getTemperatureOffset(offsetC) == 0);
}

/**
 * @brief Set ambient pressure for CO2 compensation (in mBar).
 */
bool SCD40::setAmbientPressure(uint16_t mBar) {
    return (_scd4x.setAmbientPressure(mBar) == 0);
}

// Optional: Read back the ambient pressure if needed.
// bool SCD40::getAmbientPressure(uint16_t &mBar) {
//     uint32_t pressurePa;
//     if (_scd4x.getAmbientPressure(pressurePa) != 0)
//         return false;
//     mBar = pressurePa / 100;
//     return true;
// }

/**
 * @brief Enable or disable automatic self-calibration (ASC).
 */
bool SCD40::enableAutomaticSelfCalibration(bool enable) {
    return (_scd4x.setAutomaticSelfCalibrationTarget(enable) == 0);
}

/**
 * @brief Reset the sensor to factory settings.
 */
bool SCD40::performFactoryReset() {
    return (_scd4x.performFactoryReset() == 0);
}

/**
 * @brief Reinitialize the sensor (reloads settings from EEPROM).
 */
bool SCD40::reinit() {
    return (_scd4x.reinit() == 0);
}

// Optionally expose raw pressure setter if needed
// bool SCD40::setPressure(uint32_t pressure) {
//     return (_scd4x.setAmbientPressure(pressure) == 0);
// }
