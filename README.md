# Low-Energy-Bluetooth-Service-Broadcasting

# Bluetooth Service Broadcasting with ESP32

## Overview
This project demonstrates how to broadcast temperature and humidity services over Bluetooth using an ESP32 development board. The project also includes functionality to transfer WiFi credentials over Bluetooth and connect to WiFi.

## Prerequisites
- ESP32 Development Board
- Arduino Uno
- DHT22 Sensor
- nRF Connect app

## Setup
1. Clone the repository.
2. Open the project in Arduino IDE.
3. Upload the code to your ESP32 board.

## How It Works
- The ESP32 broadcasts two BLE services: Temperature and Humidity.
- The services can be read and notified using the nRF Connect app.
- Extra feature: Transfer WiFi credentials over Bluetooth and connect to WiFi.

## Testing with nRF Connect
Open the nRF Connect app.
Scan for devices and connect to your ESP32.
Verify that you can read and get notifications for the temperature and humidity characteristics.


