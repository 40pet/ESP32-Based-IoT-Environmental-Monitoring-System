# ESP32-Based IoT Environmental Monitoring System 🌍📡

This project is a real-time environmental monitoring system that uses an ESP32 microcontroller to collect data on air quality, temperature, humidity, and GPS location. The data is stored in Firebase, making it accessible remotely via a web interface, which is ideal for monitoring environmental conditions from any location.

## Table of Contents
- [Project Overview](#project-overview)
- [How It Works](#how-it-works)
- [Features](#features)
- [Hardware Requirements](#hardware-requirements)
- [Software Requirements](#software-requirements)
- [Pin Connections](#pin-connections)
- [System Flowchart](#system-flowchart)
- [Circuit Diagram](#circuit-diagram)
- [Installation and Setup](#installation-and-setup)
- [Usage](#usage)
- [Project Structure](#project-structure)
- [Contributing](#contributing)
- [License](#license)

## Project Overview
The ESP32-based environmental monitoring system uses sensors and modules to gather environmental data. This data is sent to Firebase for real-time tracking and can be accessed via a web interface. The key sensors and modules include:
- **MQ-135 Gas Sensor** for air quality measurements.
- **DHT22 Sensor** for temperature and humidity.
- **GPS Module** for obtaining latitude and longitude.
- An **LED Indicator** to show GPS connection status.

The collected data can be used for applications like air quality monitoring, weather station projects, or environmental tracking in a specified location.

## How It Works

1. **Wi-Fi Connection**: When the system starts, the ESP32 attempts to connect to a Wi-Fi network using the specified SSID and password. This connection enables data transmission to Firebase.

2. **Component Initialization**: 
   - The **DHT22 sensor** initializes to start reading temperature and humidity.
   - The **GPS module** initializes to start acquiring location data. The system uses a hardware serial connection (UART1) for communication with the GPS module.

3. **GPS Location Tracking**:
   - The system attempts to fetch a valid GPS location upon startup.
   - The **LED blinks** while waiting for a GPS fix. Once valid GPS coordinates are obtained (latitude and longitude), the **LED stays on** briefly to indicate a successful connection.
   - GPS data is sent to Firebase for remote access, marking the location of the monitoring station.

4. **Sensor Data Acquisition**:
   - The **MQ-135 Gas Sensor** measures air quality and estimates the Air Quality Index (AQI). The sensor’s analog value is read and mapped to an AQI scale (0-1000).
   - The **DHT22 Sensor** provides temperature and humidity data. This data, along with a randomly generated atmospheric pressure value, gives a comprehensive environmental snapshot.

5. **Data Upload to Firebase**:
   - All sensor readings (AQI, temperature, humidity, and GPS coordinates) are uploaded to Firebase, making it accessible through a web interface.
   - The Firebase database serves as a backend, storing data in a JSON format, which is convenient for real-time applications.

6. **Data Visualization on Web Interface**:
   - The web interface fetches data from Firebase, allowing users to monitor environmental conditions remotely.

The system runs continuously, updating sensor data every 3 seconds to Firebase.

## Features
- **Air Quality Monitoring**: Uses the MQ-135 sensor to measure air quality, especially useful for tracking pollution levels.
- **Temperature & Humidity Monitoring**: Captures ambient temperature and humidity via the DHT22 sensor.
- **GPS Location Tracking**: Tracks the device's location, which is useful for mobile or fixed environmental stations.
- **Firebase Integration**: Stores data in Firebase, making it easily accessible and storable.
- **Web Interface**: Displays live sensor data, offering a remote monitoring solution.
- **Status LED**: Provides a visual indication for GPS data acquisition status.

## Hardware Requirements
| Component             | Description                             |
|-----------------------|-----------------------------------------|
| **ESP32**             | Wi-Fi and Bluetooth-enabled microcontroller. |
| **MQ-135 Gas Sensor** | Detects air quality by measuring gases like CO2, NH3, and others. |
| **DHT22 Sensor**      | Measures temperature and humidity.     |
| **GPS Module**        | Provides real-time latitude and longitude. |
| **LED**               | Indicates GPS data acquisition status. |
| **Resistors**         | Required for LED connections.          |
| **Jumper Wires**      | For circuit connections.               |
| **Breadboard**        | For circuit assembly.                  |

## Software Requirements
- **Arduino IDE**: Programming environment.
- **Firebase ESP32 Client**: For Firebase connectivity.
- **ArduinoJson Library**: For JSON parsing.
- **TinyGPS++ Library**: For GPS data parsing.
- **DHT Sensor Library**: For reading data from DHT22.

## Pin Connections
| Component          | ESP32 Pin      | Description                    |
|--------------------|----------------|--------------------------------|
| **DHT22 Sensor**   | GPIO 23        | Data pin for temperature and humidity readings. |
| **MQ-135 Sensor**  | GPIO 35 (A)    | Analog output pin for air quality readings. |
| **GPS Module**     | GPIO 16 (RX), GPIO 17 (TX) | Serial communication for GPS data. |
| **LED Indicator**  | GPIO 12        | Indicates GPS data acquisition status. |

## System Flowchart
Here’s the flowchart of the program’s operation:

## Circuit Diagram
To create the circuit, connect the components as follows:

### DHT22 Sensor:
- VCC pin to 3.3V on the ESP32
- Data pin to GPIO 23 on the ESP32 (with a 10K pull-up resistor between the data pin and VCC)
- GND pin to GND on the ESP32

### MQ-135 Sensor:
- VCC pin to 5V on the ESP32
- Analog output (A0) pin to GPIO 35 on the ESP32
- GND pin to GND on the ESP32

### GPS Module:
- VCC pin to 3.3V on the ESP32
- TX pin to GPIO 16 on the ESP32 (RX)
- RX pin to GPIO 17 on the ESP32 (TX)
- GND pin to GND on the ESP32

### LED Indicator:
- Anode (long pin) to GPIO 12 on the ESP32
- Cathode (short pin) to GND (with a suitable resistor in series)

![Circuit Diagram](link_to_your_circuit_diagram_image) <!-- Replace with your actual circuit diagram image link -->

## Installation and Setup
1. Clone the repository to your local machine:
   ```bash
   git clone https://github.com/yourusername/your-repo-name.git
   cd your-repo-name
