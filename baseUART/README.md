# UART Communication Implementation

This directory contains UART (Universal Asynchronous Receiver-Transmitter) communication implementation between Arduino Mega (master) and Arduino Uno (slave) using structured data packets and reliable transmission protocols.

## Overview

UART communication enables serial data exchange between microcontrollers without requiring clock synchronization. This implementation demonstrates a robust communication protocol with data validation, command processing, and bidirectional communication capabilities.

## Hardware Configuration

- **Master**: Arduino Mega 2560
  - Uses Hardware Serial1 (pins 18-TX, 19-RX)
  - Higher processing power and multiple serial ports
  - Initiates communication and sends commands

- **Slave**: Arduino Uno
  - Uses SoftwareSerial library (pins 2-RX, 3-TX)
  - Receives commands and controls connected devices
  - Digital pin 13 configured for LED/device control

## Communication Protocol

### Data Structure

Both master and slave use a structured data packet for reliable communication:

```cpp
typedef struct {
    uint16_t start;      // Start frame identifier (0xABCD)
    int16_t id;          // Command identifier
    int16_t data;        // Primary data field
    int16_t data1;       // Additional data field 1
    int16_t data2;       // Additional data field 2
    int16_t data3;       // Additional data field 3
    int16_t data4;       // Additional data field 4
    int16_t data5;       // Additional data field 5
    int16_t data6;       // Additional data field 6
    int16_t data7;       // Additional data field 7
    uint16_t checksum;   // Validation checksum
} SerialCommand;
```

### Protocol Features

1. **Start Frame Detection**: `0xABCD` ensures proper packet alignment
2. **Multi-field Data**: Support for complex command structures
3. **Checksum Validation**: Data integrity verification
4. **Command Classification**: Predefined sensor/control commands
5. **Error Handling**: Invalid data detection and recovery

## Implementation Details

### Master Implementation (`arduino-communication-mega/`)

**Key Functions:**
- Reads data from Serial0 (USB connection)
- Constructs command packets with predefined structure
- Transmits via Serial1 to slave Arduino
- Configurable data fields for different applications

**Sample Packet Construction:**
```cpp
Command.start = (uint16_t)START_FRAME;     // 0xABCD
Command.id = (int16_t)1;                   // Command ID
Command.data = (int16_t)inputValue;        // Variable data
Command.data1 = (int16_t)11;               // Fixed data 1
// ... additional fields
Command.checksum = (uint16_t)Sensor1;      // Validation
```

### Slave Implementation (`arduino-communication/`)

**Key Functions:**
- **`Receive()`**: Handles incoming data packets
  - Frame synchronization using start frame detection
  - Byte-by-byte packet reconstruction
  - Data validation through checksum verification
  - Command execution based on received data

- **Command Processing**: 
```cpp
switch(Command.checksum) {
    case Sensor1:
        digitalWrite(13, Command.data == 1 ? HIGH : LOW);
        break;
    case Sensor2:
        digitalWrite(10, Command.data == 1 ? HIGH : LOW);
        break;
}
```

### Data Reception Algorithm

The slave implements a sophisticated reception algorithm:

1. **Byte Reading**: Continuous monitoring of serial buffer
2. **Frame Detection**: Identifies start frame (0xABCD) in byte stream
3. **Packet Assembly**: Constructs complete data structure
4. **Validation**: Verifies packet integrity using checksum
5. **Command Execution**: Processes valid commands
6. **Error Recovery**: Handles invalid or corrupted data

## Technical Specifications

### Communication Parameters
- **Baud Rate**: 9600 bps
- **Data Bits**: 8
- **Stop Bits**: 1
- **Parity**: None
- **Flow Control**: None

### Packet Structure
- **Total Size**: 22 bytes (11 fields × 2 bytes each)
- **Header**: 2 bytes (start frame)
- **Payload**: 18 bytes (9 data fields)
- **Validation**: 2 bytes (checksum)

### Performance Characteristics
- **Transmission Rate**: ~435 packets/second theoretical maximum
- **Actual Rate**: Limited by processing delays and error handling
- **Reliability**: High, with built-in error detection and recovery

## Applications

### Industrial Control
- Remote device activation/deactivation
- Sensor data collection and transmission
- Multi-node communication networks

### IoT Systems
- Distributed sensor networks
- Home automation control
- Remote monitoring systems

### Educational Projects
- Learning serial communication protocols
- Understanding data validation techniques
- Embedded systems communication

## Debugging and Monitoring

### Serial Output Features
- Real-time packet transmission status
- Received data validation results
- Command execution confirmation
- Error detection and reporting

### Debug Output Example
```
Received: start=0xABCD, id=1, data=1, checksum=1
Command executed: LED ON
Invalid packet: checksum mismatch
```

## Error Handling

The implementation includes robust error handling:
- **Invalid Start Frame**: Packet discarded, continues monitoring
- **Checksum Mismatch**: Packet rejected, error logged
- **Buffer Overflow**: Automatic index reset and recovery
- **Communication Loss**: Timeout handling and reconnection

## Wiring Diagram

```
Arduino Mega          Arduino Uno
    |                     |
    Serial1 TX (18) ----> Pin 2 (SoftwareSerial RX)
    Serial1 RX (19) <---- Pin 3 (SoftwareSerial TX)
    GND             ----> GND
```

![UART Communication](./baseUART.jpeg)

## Usage Instructions

1. **Hardware Setup**: Connect Arduino boards as per wiring diagram
2. **Code Upload**: Flash master code to Mega, slave code to Uno
3. **Serial Monitor**: Open serial monitors for both boards (9600 baud)
4. **Testing**: Send data through Mega's serial monitor to test communication
5. **Observation**: Monitor LED responses and serial output for verification

This UART implementation provides a foundation for reliable serial communication in embedded systems, with built-in error handling and expandable command structures.