# 7Semi-SCD4x-Arduino-Library
Arduino library for 7Semi SCD4x (SCD40/SCD41) CO₂, temperature, and humidity sensors using I2C 

![Arduino](https://img.shields.io/badge/platform-arduino-blue.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)
![Status](https://img.shields.io/badge/status-active-brightgreen.svg)

Official Arduino library for the **7Semi SCD4x series** (SCD40 / SCD41) CO₂, Temperature, and Humidity sensors.

Built for seamless integration with Arduino-compatible boards using I2C communication.

---

## 🌟 Features

- 📡 CO₂ (ppm), Temperature (°C), and Humidity (%) readings
- 🔁 Continuous and single-shot measurement modes
- ✅ CRC8 error checking
- 🔄 Data ready status polling
- ⏱ Configurable measurement intervals
- 💡 Ready-to-use Arduino examples
- 🧰 Clean and lightweight C++ implementation

---

## 📦 Supported Devices

- **SCD40** – Compact, low-power CO₂ sensor  
- **SCD41** – Extended measurement range and auto-altitude compensation  

---

## 🧰 Installation

### Option 1: Arduino Library Manager
- Open Arduino IDE → Tools → Manage Libraries...
- Search for **7Semi SCD4x**
- Click **Install**

### Option 2: Manual ZIP Installation
- Download this repository as a `.zip`
- Open Arduino IDE → `Sketch` → `Include Library` → `Add .ZIP Library...`

---

## 🔌 I2C Connection

| SCD4x Pin | Arduino Pin (UNO) |
|-----------|-------------------|
| VCC       | 3.3V / 5V         |
| GND       | GND               |
| SDA       | A4                |
| SCL       | A5                |

> Default I2C address: `0x62`

---

## 📂 Folder Structure

