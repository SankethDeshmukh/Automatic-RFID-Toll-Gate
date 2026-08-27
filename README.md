# Automatic RFID Toll Gate System

An Arduino-based automatic toll gate system using RFID, IR sensor, servo motor, and LEDs.

## Project Overview

This project is designed to automate vehicle access at a toll gate using RFID technology.

When an authorized RFID card is scanned:
- The RFID reader verifies the card UID.
- The green LED turns ON.
- The servo motor opens the gate.
- The IR sensor detects when the vehicle passes.
- The gate automatically closes.

When an unauthorized RFID card is scanned:
- The red LED turns ON.
- Access is denied.
- The gate remains closed.

## Components Used

- Arduino
- MFRC522 RFID Reader
- RFID Card/Tag
- Servo Motor
- IR Sensor
- Green LED
- Red LED
- Resistors
- Jumper Wires
- Breadboard

## Pin Configuration

| Component | Arduino Pin |
|---|---|
| RFID SDA/SS | D10 |
| RFID RST | D9 |
| IR Sensor | D7 |
| Servo Motor | D6 |
| Green LED | D3 |
| Red LED | D4 |

## Software

- Arduino IDE
- Arduino C/C++
- MFRC522 RFID Library
- Servo Library

## Working

**Authorized Card:**

RFID Scan → UID Verification → Green LED → Gate Opens → Vehicle Passes → Gate Closes

**Unauthorized Card:**

RFID Scan → UID Verification → Red LED → Access Denied → Gate Remains Closed

## Future Improvements

- Automatic toll payment
- LCD/OLED display
- Multiple authorized RFID cards
- Vehicle number recognition
- Database integration
- IoT monitoring

## Author

Sanketh Deshmukh
