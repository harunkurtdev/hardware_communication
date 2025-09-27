# I2C Communication Implementation

This directory contains two different approaches to I2C communication between Arduino boards, demonstrating master-slave architecture with different data transmission methods.

## Overview

I2C (Inter-Integrated Circuit) is a synchronous, multi-master, multi-slave, packet-switched, single-ended, serial communication bus. This implementation uses two Arduino Uno boards: one as master and one as slave.

## Hardware Configuration

- **Master Arduino**: Arduino Uno with potentiometer connected to analog pin A0
- **Slave Arduino**: Arduino Uno with I2C LCD display
- **Connections**: 
  - SDA: Pin A4 on both Arduinos
  - SCL: Pin A5 on both Arduinos
  - Common Ground connection
  - Power: 5V for both boards

## Implementation Methods

### 1. Bytewise Method (`bytewise/`)

**Purpose**: Simple 8-bit data transmission with potentiometer readings

**Master (`master_arduino.cpp`):**
- Reads analog value from potentiometer (0-1023)
- Converts to 8-bit by right-shifting: `pot >> 2` (divides by 4)
- Transmits to slave at I2C address 4
- Sends descriptive string followed by data byte

**Slave (`slave_arduino.cpp`):**
- Configured with I2C address 4
- Receives data in `receiveEvent()` function
- Reconstructs original scale: `x = (Wire.read() << 2)`
- Displays received value on LCD and serial monitor

**Key Features:**
- Simple byte-level communication
- Real-time potentiometer monitoring
- LCD visual feedback
- Serial debugging output

### 2. Byteshift Method (`byteshift/`)

**Purpose**: Advanced 16-bit data transmission using union structures

**Master (`master_arduino.cpp`):**
```cpp
union byteint {
    byte b[sizeof(int)];  // Byte array representation
    int i;                // Integer representation
};
```
- Uses union to split 16-bit integer into two bytes
- Transmits high and low bytes separately
- Supports full potentiometer resolution (0-1023)

**Slave (`slave_arduino.cpp`):**
- Receives two separate bytes
- Reconstructs 16-bit value: `a = x + y*256`
- Displays full-resolution value on LCD

**Key Features:**
- Full data precision preservation
- Union-based byte manipulation
- Multi-byte transmission protocol
- Enhanced data integrity

## Code Analysis

### Master Communication Protocol

Both methods follow this pattern:
1. Read sensor data (potentiometer)
2. Process/convert data for transmission
3. Begin I2C transmission to slave address
4. Send data bytes
5. End transmission
6. Serial output for debugging

### Slave Communication Protocol

Both methods implement:
1. I2C address assignment in `setup()`
2. Event-driven data reception
3. Data processing and reconstruction
4. LCD display update
5. Serial output for monitoring

### Data Flow

```
Potentiometer → ADC → Master Arduino → I2C Bus → Slave Arduino → LCD Display
    (0-5V)     (0-1023)    (Processing)   (Digital)   (Reception)    (Visual)
```

## Practical Applications

1. **Sensor Networks**: Multi-node sensor data collection
2. **Display Systems**: Remote display control and data visualization
3. **Control Systems**: Distributed control with master-slave architecture
4. **Educational Projects**: Learning I2C protocol implementation

## Testing and Simulation

### Tinkercad Simulations

- **Byteshift Method**: [Tinkercad Simulation](https://www.tinkercad.com/things/eE9b4X5jmpW-copy-of-i2c-com-with-wireh/editel?sharecode=XLDyg7p9E6Mbm2572xAJjLvie2I9KFtmwcOHa5AqQSc)
- **Bytewise Method**: [Tinkercad Simulation](https://www.tinkercad.com/things/cnZYMkmAhdb-copy-of-i2c-com-with-wireh/editel?sharecode=hEAVQgVywABOqQ5WvQSRtm-AC5PjpibXAh19wcuV0Ys)

### Debugging Features

- Serial monitor output on both master and slave
- LCD real-time display updates
- Transmission status indicators
- Data validation through visual feedback

## Technical Specifications

- **Communication Speed**: Standard I2C (100 kHz)
- **Data Rate**: ~20 samples per second (50ms delay)
- **Resolution**: 8-bit (bytewise) or 16-bit (byteshift)
- **Protocol**: Standard I2C with 7-bit addressing
- **Power Requirements**: 5V for Arduino boards and peripherals

![I2C Communication Diagram](./baseI2C.png)