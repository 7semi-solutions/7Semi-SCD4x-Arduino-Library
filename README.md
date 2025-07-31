# 7Semi-SCD4x-CO₂-Temperature-Humidity-Sensor-Module-I2C-Arduino-Library

This Arduino library provides support for the 7Semi-SCD4x (SCD40/SCD41) sensor module over I2C. It enables real-time measurement of **CO₂ (ppm)**, **temperature (°C)**, and **humidity (%)**, and supports both continuous and single-shot measurement modes. The library also includes CRC8 validation and optional data-ready status polling.

![platform](https://img.shields.io/badge/platform-arduino-blue.svg)
![license](https://img.shields.io/badge/license-MIT-green.svg)
![status](https://img.shields.io/badge/status-active-brightgreen.svg)



## Hardware Required

- SCD40 or SCD41 sensor module  
- Arduino-compatible board  
- I2C connection (SDA, SCL)

## Getting Started

  ### 1. Library Installation
  
  - Download or clone the repository.
  
  - Copy the files `7semi_scd4x.cpp` and `7semi_scd4x.h` into a folder named `7semi_SCD4x` under your Arduino `libraries` directory.
  
  - Restart the Arduino IDE.

---

### 2. Wiring

| SCD4x Pin | Arduino Pin |
|-----------|--------------|
| SDA       | A4 (Uno)     |
| SCL       | A5 (Uno)     |
| VCC       | 3.3V / 5V    |
| GND       | GND          |

> Default I2C address: `0x62`  




---

## 📂 Folder Structure

