# Setup and Installation Guide

This guide provides step-by-step instructions for setting up the hardware communication examples in this repository.

## Prerequisites

### Hardware Requirements

#### For I2C Communication:
- 2× Arduino Uno boards
- 1× 10kΩ Potentiometer  
- 1× I2C LCD Display (16x2)
- 2× 4.7kΩ Pull-up resistors
- Breadboard and jumper wires
- USB cables for programming

#### For UART Communication:
- 1× Arduino Mega 2560
- 1× Arduino Uno
- Jumper wires for serial connections
- USB cables for programming
- LED for testing (optional)

#### For TCP/IP Communication:
- 1× Arduino Mega 2560
- 1× Ethernet Shield (W5100 or W5500)
- Ethernet cable
- Router/Switch with available ports
- Computer for client testing

### Software Requirements

- **Arduino IDE** (version 1.8.19 or later) OR **PlatformIO**
- **Required Libraries:**
  - `Wire.h` (built-in)
  - `SoftwareSerial.h` (built-in)
  - `Adafruit_LiquidCrystal.h`
  - `Ethernet.h` (for Arduino Ethernet library)
  - `SPI.h` (built-in)

## Installation Steps

### 1. Arduino IDE Setup

1. Download and install Arduino IDE from [arduino.cc](https://www.arduino.cc/en/software)
2. Install required libraries:
   - Go to **Tools > Manage Libraries**
   - Search for "Adafruit LiquidCrystal" and install
   - Search for "Ethernet" and install Arduino Ethernet library

### 2. PlatformIO Setup (Alternative)

1. Install PlatformIO IDE or VS Code extension
2. Open the project directories containing `platformio.ini`
3. Libraries will be automatically downloaded during first build

### 3. Hardware Connections

#### I2C Setup (Bytewise Example):
```
Master Arduino (Uno):          Slave Arduino (Uno):
A0 → Potentiometer middle       A4 → LCD SDA
A4 → Slave A4 (SDA)            A5 → LCD SCL  
A5 → Slave A5 (SCL)            GND → LCD GND
5V → Pull-up resistors         5V → LCD VCC
GND → Common ground            I2C Address: 4
```

#### I2C Setup (Byteshift Example):
```
Same as above, but slave address = 9
```

#### UART Setup:
```
Arduino Mega:                  Arduino Uno:
Serial1 TX (Pin 18) →          Pin 2 (SoftwareSerial RX)
Serial1 RX (Pin 19) ←          Pin 3 (SoftwareSerial TX)  
GND →                          GND
                               Pin 13 → LED (optional)
```

#### TCP/IP Setup:
```
Arduino Mega + Ethernet Shield:
- Mount Ethernet shield on Mega
- Connect Ethernet cable to shield
- Connect to same network as computer
- Default IP: 192.168.31.177
```

## Programming the Boards

### Method 1: Arduino IDE

1. **I2C Communication:**
   - Open `baseI2C/bytewise/master_arduino.cpp` or `byteshift/master_arduino.cpp`
   - Select **Tools > Board > Arduino Uno**
   - Select correct COM port
   - Upload to first Arduino (Master)
   - Open corresponding `slave_arduino.cpp`
   - Upload to second Arduino (Slave)

2. **UART Communication:**
   - Open `baseUART/arduino-communication-mega/src/main.cpp`
   - Select **Tools > Board > Arduino Mega**
   - Upload to Arduino Mega
   - Open `baseUART/arduino-communication/src/main.cpp`  
   - Select **Tools > Board > Arduino Uno**
   - Upload to Arduino Uno

3. **TCP/IP Communication:**
   - Open `baseTCP_IP/tcp_ip-arduino_mega_uart/src/main.cpp`
   - Select **Tools > Board > Arduino Mega**
   - Upload to Arduino Mega with Ethernet Shield

### Method 2: PlatformIO

1. Navigate to project directory containing `platformio.ini`
2. Run: `pio run` to build
3. Run: `pio run --target upload` to upload
4. Specify upload port if needed: `pio run --target upload --upload-port COM3`

## Testing and Verification

### I2C Testing:

1. **Hardware Check:**
   - Verify all connections match wiring diagrams
   - Check pull-up resistors are connected
   - Confirm power connections (5V, GND)

2. **Software Testing:**
   - Open Serial Monitor (9600 baud) for both boards
   - Rotate potentiometer on master
   - Verify values appear on slave LCD and serial output
   - Expected range: 0-255 (bytewise) or 0-1023 (byteshift)

### UART Testing:

1. **Hardware Check:**
   - Verify TX/RX connections (crossed)
   - Confirm common ground connection
   - Check LED connection to pin 13

2. **Software Testing:**
   - Open Serial Monitor for both boards (9600 baud)
   - Send data through Mega's serial monitor
   - Verify command reception and LED control on Uno
   - Check for "checksum valid" messages

### TCP/IP Testing:

1. **Network Check:**
   - Verify Ethernet cable connection
   - Confirm network settings match local subnet
   - Check Arduino gets IP address (serial monitor output)

2. **Software Testing:**
   - Use telnet client: `telnet 192.168.31.177 23`
   - Compile and run C++ client applications
   - Verify data transmission over network
   - Check for connection status messages

## Common Issues and Solutions

### I2C Issues:
- **No communication:** Check pull-up resistors and wiring
- **Garbled LCD:** Verify LCD address and power connections
- **Intermittent data:** Check for loose connections

### UART Issues:
- **No data received:** Verify baud rates match (9600)
- **Corrupted data:** Check TX/RX wiring (must be crossed)
- **Commands not working:** Verify checksum values in code

### TCP/IP Issues:
- **No network connection:** Check Ethernet cable and network settings
- **IP conflicts:** Use unique IP address for local network
- **Connection refused:** Verify port 23 is not blocked

## Advanced Configuration

### Customizing I2C Addresses:
```cpp
// In slave code, change address:
Wire.begin(4);  // Change number for different address
```

### Modifying UART Baud Rates:
```cpp
// Change in both master and slave:
Serial1.begin(9600);      // Master
CommandSerialCom.begin(9600);  // Slave
```

### Changing TCP/IP Network Settings:
```cpp
// Modify in TCP/IP code:
IPAddress ip(192, 168, 1, 100);     // New IP address
IPAddress gateway(192, 168, 1, 1);  // Gateway
IPAddress subnet(255, 255, 255, 0); // Subnet mask
```

## Monitoring and Debugging

### Serial Monitor Settings:
- **Baud Rate:** 9600
- **Line Ending:** Both NL & CR
- **Enable timestamps** for debugging

### Debug Output Examples:

**I2C Master:**
```
Potentiometer: 512
Transmitted: 128
```

**I2C Slave:**
```
Received: 128
Display value: 512
```

**UART Communication:**
```
Command received: ID=1, Data=1, Checksum=1
LED State: ON
```

**TCP/IP Communication:**
```
Network initialized: 192.168.31.177
New client connected
Data transmitted: 22 bytes
```

## Extending the Projects

### Adding More I2C Devices:
- Use different I2C addresses (0-127)
- Add more sensors or actuators
- Implement I2C multiplexing for many devices

### UART Protocol Extensions:
- Add more command types
- Implement bidirectional communication
- Add error correction codes

### TCP/IP Enhancements:
- Implement UDP for real-time data
- Add web server functionality  
- Integrate with IoT platforms

This setup guide ensures successful implementation of all communication protocols in the repository.