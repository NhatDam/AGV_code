# AGV_code - Automated Guided Vehicle Control System

This repository contains firmware for the low-level control of an Automated Guided Vehicle (AGV). The system provides comprehensive motor control, line following, safety monitoring, battery management, and sensor integration capabilities. Built on the Arduino framework with PlatformIO, it manages complex robotics operations on an ATmega2560 microcontroller.

## Table of Contents

- [Overview](#overview)
- [Hardware Setup](#hardware-setup)
- [Project Structure](#project-structure)
- [Component Description](#component-description)
- [Building and Uploading](#building-and-uploading)
- [Usage](#usage)

## Overview

The AGV system is designed for autonomous navigation with the following key features:

- **Line Following**: Autonomous path tracking using sensor arrays
- **Motor Control**: Precise speed regulation using PID control
- **Safety Systems**: Collision detection and emergency stop mechanisms
- **Battery Management**: Real-time State of Charge (SoC) monitoring via INA219
- **Communication**: Serial interfaces for debugging and external control (ROS compatibility)
- **Sensor Integration**: Encoder feedback, proximity sensors, and UV marking detection

## Hardware Setup

**Target Platform**: Arduino Mega 2560 (ATmega2560)  
**Framework**: Arduino  
**Monitor Speed**: 57600 baud

**Key Dependencies**:
- **DFRobot_INA219**: Power monitoring and SoC calculation
- **SimpleKalmanFilter**: Sensor data filtering
- **NewPing**: Distance/proximity sensing

## Project Structure

```
AGV_code/
├── include/                  # Header files for main application
│   ├── AGV_controller.h      # PID control structure and parameters
│   ├── commands.h            # Command definitions for system communication
│   └── README
├── lib/                      # Component libraries
│   ├── control/              # Motor control system
│   ├── Esp_control/          # ESP microcontroller interface
│   ├── Follow_line/          # Line following algorithm
│   ├── GPIO/                 # GPIO and sensor management
│   ├── pid/                  # PID controller implementation
│   ├── ROS/                  # ROS communication interface
│   ├── Safe/                 # Safety system
│   ├── SoC/                  # State of Charge monitoring
│   └── Speed_read/           # Encoder and speed reading
├── src/
│   └── main.cpp              # Main application entry point
├── platformio.ini            # PlatformIO configuration
└── README.md                 # This file
```

## Component Description

### 1. **Main Application** (`src/main.cpp`)

The main entry point that orchestrates all system components with timing management:

- **PID Rate**: Configured at 30 Hz with 33ms intervals
- **Safety Check Interval**: 100ms for collision detection
- **Setup**: Initializes Serial communication (57600 baud) and Serial2 (115200 baud for ESP)
- **Includes**: Integrates all library components for coordinated operation

### 2. **Motor Control** (`lib/control/`)

Handles motor drive commands and PWM output management:

- **Functions**:
  - `straight()`: Forward movement with speed control for both motors
  - `back()`: Backward movement
  - `left()` / `right()`: Directional turns
  - `stopp()`: Emergency stop
  - `setMotorSpeeds()`: Set combined speeds for both motors
  - `setEachMotorSpeed()`: Individual motor PWM control

- **Pin Configuration**:
  - FR1, FR2: Forward/Reverse control pins (1=right, 2=left)
  - SV1, SV2: Speed control pins (PWM signals 0-255)
  - Max PWM: 255

### 3. **PID Controller** (`lib/pid/` and `include/AGV_controller.h`)

Implements precision speed control through feedback loops:

- **PID Parameters**: Kp, Ki, Kd (tunable for each motor)
- **Data Structure** (`SetPointInfo`):
  - `TargetCountPerLoop`: Target speed in encoder ticks per frame
  - `Encoder`: Current encoder count
  - `PrevInput`/`PrevErr`: Previous feedback for derivative calculation
  - `ITerm`: Integrated error for smooth acceleration
  - `output`: Motor PWM command

- **Features**:
  - Anti-derivative kick using previous input method
  - Integrated term tuning without accumulation issues
  - Separate tuning for left and right motors

### 4. **Line Following System** (`lib/Follow_line/`)

Autonomous path navigation using sensor arrays:

- **Sensor Array**: 16-element sensor array for line detection
- **Key Functions**:
  - `follow_line()`: Main line tracking algorithm
  - `sensor_position()`: Calculate line position relative to robot
  - `UVC()`: UV light control for plant treatment
  - `count_on()`: Count sensors detecting the line

- **Features**:
  - Plant marking detection with magnetic sensors
  - Automatic plant counting and stopped at marked locations
  - Weighted sensor algorithm for accurate centering

### 5. **GPIO Management** (`lib/GPIO/`)

Low-level sensor and digital I/O operations:

- **Defines**:
  - Motor direction pins (FR1, FR2) for left/right motors
  - Speed control pins (SV1, SV2) for PWM
  - State variable for system status

- **Sensor Array**: `input_pin[]` - all connected sensor pins
- **Position Tracking**: Weighted sensor values for error calculation
- **Speed Parameters**: `maxSpeed`, `rotateSpeed` for motion limits

### 6. **Speed/Encoder Reading** (`lib/Speed_read/`)

Real-time encoder feedback for motor velocity measurement:

- **Functions**:
  - Reads encoder pulses to determine motor speed
  - Calculates RPM or ticks-per-second
  - Provides feedback for PID loop closure

- **Purpose**: Enables precise speed regulation and odometry

### 7. **State of Charge (SoC) Monitoring** (`lib/SoC/`)

Battery management using INA219 power monitor:

- **Hardware**: DFRobot_INA219 I2C sensor at address INA219_I2C_ADDRESS4
- **Measurements**:
  - `voltage`: Battery voltage (V)
  - `current`: Current draw (mA)
  - `pw_consump`: Power consumption (W)
  - `SoC_percentage`: Battery state of charge (%)

- **Functions**:
  - `Calculate_V_and_A()`: Read voltage and current from INA219
  - `Calculate_SoC()`: Update SoC based on current draw and time
  - `saveSoCToEEPROM()` / `readSoCFromEEPROM()`: Persistent SoC storage

- **Calibration**: Parameters for linear calibration against multimeter readings

### 8. **Safety System** (`lib/Safe/`)

Collision detection and emergency response:

- **Main Function**: `SAFE()` - Checks safety conditions at 100ms intervals
- **Capabilities**:
  - Obstacle detection via IR sensors
  - Automatic emergency stop if collision imminent
  - Integration with control system for safe shutdown

### 9. **ESP Communication** (`lib/Esp_control/`)

Interface with ESP microcontroller for wireless/remote operations:

- **Serial Interface**: Serial2 at 115200 baud
- **Purpose**: Enables remote command reception and telemetry transmission

### 10. **Command Definitions** (`include/commands.h`)

Protocol definition for system communication via serial interface (UART):

| Command | Symbol | Purpose |
|---------|--------|---------|
| UV_ON | 'u' | Enable UV light for marking detection |
| UV_OFF | 'o' | Disable UV light |
| READ_ENCODERS | 'e' | Request encoder values |
| MOTOR_SPEEDS | 'l' | Set motor speeds |
| MARK_PLANT | 'm' | Mark plant location for mission|
| AUTOMATED | 'a' | Enter autonomous mode |
| RESET_PLANT | 'r' | Reset plant counter |

## Building and Uploading

### Requirements
- PlatformIO (VSCode extension or CLI)
- Arduino IDE compatible board drivers
- USB cable for Arduino Mega 2560

### Build
```bash
platformio run --environment megaatmega2560
```

### Upload
```bash
platformio run --environment megaatmega2560 --target upload
```

### Monitor Serial Output
```bash
platformio device monitor --monitor-speed 57600
```

## Usage

### System Startup
1. Power the AGV
2. Arduino initializes all modules (motors, sensors, communication)
3. PID controller activates at 30 Hz
4. Safety system monitors continuously at 100ms intervals

### Line Following Mode
- AGV reads sensor array to detect line position
- PID controller adjusts motor speeds to center on line
- Detect marked plants' locations then give UV treatment

### Manual Control
- Send commands via serial interface (57600 baud or 115200 via ESP)
- Use motor speed commands for direct control
- Monitor encoder feedback and SoC status

### Autonomous Features
- Automatic speed regulation via PID
- Line tracking without external intervention
- Battery monitoring with low-power warnings
- Collision avoidance via safety system

### Special Features
- Controllable via ESP for local wireless operation (using ESP's wifi)
- Remote control via web interface through communication with Jetson Nano running ROS2
- Speed readings update to serial interface -> ROS2 on Jetson Nano -> Digital Twin on PC
---

**Version**: 1.0  
**Last Updated**: 2026-01-09  
**Platform**: Arduino Mega 2560 (ATmega2560)
