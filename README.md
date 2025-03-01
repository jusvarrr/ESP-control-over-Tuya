# ESP Control over Tuya IoT Cloud
This project involves controlling a microcontroller using Tuya Cloud by interacting with a proccess wthat communicates to ESP device via serial port. The "Ubus" system is used to allow inter-process comunication between process interacting with ESP and process communicating with Tuya Cloud, allowing the switching on and off of contacts, retrieving sensor data (e.g., from a "DHT" sensor), and listing connected devices.

!WARNING: the project is missing one more package responsible for creating a process for comminication with Tuya

## Functional Requirements

- **Actions:**
  - Switch on/off the microcontroller's contacts.
  - Retrieve information about the controller's contacts.
  - Get a list of connected devices (the list includes the device name, vendor, and product ID).
  - Returning responses in JSON format regarding the success or failure of the commands.

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

## Building & Installation (OpenWRT SDK)  

### 1. Set Up the OpenWRT Build Environment  
Ensure your OpenWRT SDK is correctly set up on your host machine.  

### 2. Add the Repository to OpenWRT's Package Directory  
Clone this repository into OpenWRT’s package/ directory:  
```bash
cd openwrt/package
git clone https://github.com/jusvarrr/ESP-control-over-Tuya.git
```

### 3. Compile packages argp_standalone, libtuyasdk, libserialport and tuyaespcontrol in OpenWRT host machine environment with:
   
```bash
  make  package/argp_standalone/compile
  make  package/libtuyasdk/compile
  make  package/libserialport/compile
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
