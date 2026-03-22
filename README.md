# MiniSat RTOS Project

This project is part of a training program for the Potiguar Rocket Design team. It serves as the foundational codebase for a real-time mini-satellite (CubeSat), built to evolve and scale over time. The system is developed on the ESP32 platform, leveraging the FreeRTOS real-time operating system to handle concurrent tasks efficiently.

## Core Features

- **Real-Time Operating System (RTOS):** Utilizes FreeRTOS to manage multiple tasks, ensuring predictable and timely execution, which is critical for satellite operations.
- **Sensor Integration:** Interfaces with an MPU6050 (Inertial Measurement Unit) and a BMP180 (Barometric Pressure Sensor) to collect essential flight data.
- **Multitasking Architecture:**
  - **Acquisition Task:** A high-frequency task dedicated to reading data from the MPU6050 (accelerometer, gyroscope) and BMP180 (temperature, pressure, altitude) at a rate of 100Hz.
  - **Telemetry Task:** A lower-frequency task responsible for processing the collected data and sending it to the serial monitor for ground monitoring at a rate of 10Hz.
- **Inter-Task Communication:** Employs a FreeRTOS queue (`sensor_queue`) as a thread-safe buffer to reliably pass data from the high-frequency acquisition task to the telemetry task.

## Hardware Components

- ESP32 Development Board
- MPU6050 Accelerometer and Gyroscope Module
- BMP180 Temperature and Barometric Pressure Module

## Software & Libraries

- [PlatformIO](https://platformio.org/) IDE
- [Arduino Framework](https://www.arduino.cc/)
- `FreeRTOS.h`: Core library for the FreeRTOS kernel.
- `Adafruit_MPU6050.h`: Driver for the MPU6050 sensor.
- `Adafruit_BMP085.h`: Driver for the BMP180 sensor (compatible with BMP085).

## Project Structure

The codebase is organized into two primary tasks:

1.  **`task_acquisition`**: This task runs every 10 milliseconds (100Hz). Its sole responsibility is to poll the MPU6050 and BMP180 sensors, package the readings into a `SatelliteData_t` struct, and send this data packet to the `sensor_queue`.
2.  **`task_telemetry`**: This task waits for data to become available in the `sensor_queue`. Upon receiving a data packet, it formats the information and prints it to the serial port every 100 milliseconds (10Hz), simulating a telemetry downlink.

## Future Development

This project is the starting point for a more complex CubeSat system. Future expansions will build upon this RTOS foundation, adding more advanced capabilities such as:

- Data logging to an SD card
- Radio communication (LoRa/RF)
- Attitude determination and control algorithms
- Power management systems

This incremental development approach allows team members to learn and contribute to a growing, mission-oriented project.
