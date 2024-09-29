# M5Stack-OSC
Sending OSC data from M5StickCPlus2 over local network

### Structure

Projects have Arduino IDE sketches structure

### Prerequisities
To make a project work you will need to create `wifi_config.h` file in this projects folder.  
The file must have the following structure:

```cpp
#define SSID "your-wifi-network-name"
#define PASS "your-wifi-network-password"
#define OUT_IP { 127, 0, 0, 1 }
```
Where `SSID` and `PASS` reflec your WiFi's name and password and `OUT_IP` matches IPv4 adress of the device that will receive the OSC data.

## Projects

### SendGyro
Script that will send M5StickCPlus2 onboard IMU (MPU6886) data via OSC messages.  

Created based on: [Edinburgh-College-of-Art/m5stickc-plus-introduction/.../SendFloat_IMU.ino](https://github.com/Edinburgh-College-of-Art/m5stickc-plus-introduction/blob/main/examples/Wireless-Communication/OSC/SendFloat_IMU/SendFloat_IMU.ino)

The following data is transmitted:  
 - `/gyroX` `/gyroY` `/gyroZ` - gyroscope data  
 - `/accX` `/accY` `/accZ` - accelerometer data  
 - `/pitch` `/roll` `/yaw` - attitude and heading reference system (AHRS) data - keep in mind that the MPU6886 IMU does not have geomagnetic sensor so those value are calculated using [Mahony's AHRS algorithm](https://x-io.co.uk/open-source-imu-and-ahrs-algorithms/)


## Dependencies

 - [M5StickCPlus2 library (M5Stack)](https://github.com/m5stack/M5StickCPlus2)
 - [OSC library (Adrian Freed)](https://github.com/CNMAT/OSC)
