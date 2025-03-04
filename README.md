# ESP Control over Tuya IoT Cloud
This project involves creating a process on an OpenWRT-based router to control a microcontroller via Tuya Cloud. The process interacts with another process that controls an ESP device through a serial port. The "Ubus" system facilitates inter-process communication. This allows for switching the microcontroller's contacts on and off, retrieving sensor data (e.g., from a "DHT" sensor), and listing connected devices.

## Functional Requirements

- **Actions:**
  - Use Tuya to switch on/off the microcontroller's contacts.
  - Use Tuya to retrieve information about the controller's contacts.
  - Use Tuya to get a list of connected devices (the list includes the device name, vendor, and product ID).
  - Returning responses to Tuya in JSON format regarding the success or failure of the commands.

- **Automatic Start:**
  - The service starts automatically when the router is powered on.

- **USB Communication:**
  - Secure and correct communication with the USB device is ensured.

- **Tuya Cloud Control:**
  - Controlling microcontrollers is done using the Tuya IoT Cloud system.

## Non-Functional Requirements

- **Message Format:**
  - All messages must be formatted in JSON.

- **Secure Communication:**
  - A library is used to ensure secure and correct serial communication with the USB device.

- **Ubus System:**
  - The "Ubus" system and its functions are used for communication between processes.

## Components
### argp_standalone  
A package for building the argument parsing library installation file on the router. This enables parsing of configuration files for Tuya Cloud authentication on OpenWRT.  

### libtuyasdk  
A package for building the Tuya IoT Core SDK installation file. This SDK is required for establishing communication between the router and the Tuya Cloud platform.

### libserialport
A package responsible for building installation file implementing libary to be used for serial communication to interact with ESP8266.

### tuyaespcontrol
The main package of this project responsible for building the installation files required to establish communication between Tuya and the router. It makes the program use Tuya Cloud to control ESP devices via the Ubus system on the router.

### espcontrol
Package that builds service for the router allowing communication with ESP8266 over serial port (USB cable). It works as a UBUS server and takes request from other processes requesting actions to be performed on ESP8266.

## Building & Installation (OpenWRT SDK)  

### 1. Set Up the OpenWRT Build Environment  
Ensure your OpenWRT SDK is correctly set up on your host machine.  

### 2. Add the Repository to OpenWRT's Package Directory  
Clone this repository into OpenWRT’s package/ directory:  
```bash
cd openwrt/package
git clone https://github.com/jusvarrr/ESP-control-over-Tuya.git
```

### 3. Compile packages argp_standalone, libtuyasdk, libserialport, espcontrol and tuyaespcontrol in OpenWRT host machine environment with:
   
```bash
  make  package/argp_standalone/compile
  make  package/libtuyasdk/compile
  make  package/libserialport/compile
  make  package/espcontrol/compile
  make  package/tuyaespcontrol/compile
```

### 4. Deploy the installation file to the router using ssh:
   
```bash
   scp bin/packages/*/*.ipk device@<device_ip>:/tmp/
```

### 5. On the device run:
   
```bash
   opkg install /tmp/*.ipk
```
