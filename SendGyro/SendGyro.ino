//=============================================================
// File:    SendGyro.ino
//
// Dependencies:
//     - M5StickCPlus2 library (M5Stack)
//     - OSC library (Adrian Freed)
//
// Description:
//     - Send M5StickCPlus2 IMU data via OSC messages
//=============================================================

#include <M5StickCPlus2.h>

#include <WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>

#include "wifi_config.h"

// WiFi setup
const char *ssid = SSID;            // <----- your WiFi network's name
const char *password = PASS;        // <----- your WiFi password
const IPAddress outIp = OUT_IP;     // <----- IP address of receiving device
const unsigned int outPort = 8000;  // <----- receiving device in port

// Object for OSC/UDP
WiFiUDP udp;

// Network port for incoming messages
const unsigned int localPort = 8000; // This is not strictly needed for sending messages

// Variables for storing the different bits of IMU data
float accX = 0.f;
float accY = 0.f;
float accZ = 0.f;

float gyroX = 0.f;
float gyroY = 0.f;
float gyroZ = 0.f;

float pitch = 0.f;
float roll = 0.f;
float yaw = 0.f;

//=============================================================
// FUNCTIONS

/// Connect to WiFi
/// - Function for connecting to WiFi network
/// @return status (bool) true = successful, false = failed
bool connectToWiFi()
{
  StickCP2.Display.print("Connecting");

  // initialise - WIFI_STA = Station Mode
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // start timer
  unsigned long startAttemptTime = millis();

  // while not connected to WiFi AND before timeout
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 30000)
  {
    StickCP2.Display.print(".");
    delay(400);
  }

  // Print status to LCD
  if (WiFi.status() != WL_CONNECTED)
  {
    StickCP2.Display.println("\nErr: Failed to connect");
    delay(2000);
    return false;
  }
  else
  {
    StickCP2.Display.println("\nConnected to:");
    StickCP2.Display.println(ssid);
    StickCP2.Display.println(WiFi.localIP());

    delay(2000);
    return true;
  }
}

/// Send float OSC Message
/// - function for sending float messages via OSC
/// @param address (const char*) message tag
/// @param message (float) message to send
void sendFloatOscMessage(const char *address, float message)
{
  // init message
  OSCMessage oscMsg(address);
  oscMsg.add(message);

  // send message
  udp.beginPacket(outIp, outPort);
  oscMsg.send(udp);

  // clear message
  udp.endPacket();
  oscMsg.empty();
}

//=============================================================
// SETUP
void setup()
{
  auto cfg = M5.config();
  StickCP2.begin(cfg);

  // Connect to WiFi network
  while (!connectToWiFi())
  {
  }
  udp.begin(localPort);

  delay(1000);
  StickCP2.Display.fillScreen(BLACK);

  // Display setup
  StickCP2.Display.setRotation(3);
  StickCP2.Display.fillScreen(BLACK);
  StickCP2.Display.setTextSize(1);

  StickCP2.Display.setCursor(80, 15);
  StickCP2.Display.println("IMU TEST");

  StickCP2.Display.setCursor(30, 30);
  StickCP2.Display.println("  X       Y       Z");

  StickCP2.Display.setCursor(30, 70);
  StickCP2.Display.println("  Pitch   Roll    Yaw");
}

//=============================================================
// LOOP
void loop()
{

  // 1. GET IMU DATA
  auto imu_update = StickCP2.Imu.update();
  if (imu_update)
  {
    auto data = StickCP2.Imu.getImuData();

    // The data obtained by getImuData can be used as follows.
    accX = data.accel.x; // accel x-axis value.
    accY = data.accel.y; // accel y-axis value.
    accZ = data.accel.z; // accel z-axis value.
    // data.accel.value; // accel 3values array [0]=x / [1]=y / [2]=z.

    gyroX = data.gyro.x; // gyro x-axis value.
    gyroY = data.gyro.y; // gyro y-axis value.
    gyroZ = data.gyro.z; // gyro z-axis value.
    // data.gyro.value;  // gyro 3values array [0]=x / [1]=y / [2]=z.

    pitch = data.mag.x;  // mag x-axis value.
    roll = data.mag.y;   // mag y-axis value.
    yaw = data.mag.z;    // mag z-axis value.
    // data.mag.value;   // mag 3values array [0]=x / [1]=y / [2]=z.

    // data.value;       // all sensor 9values array [0~2]=accel / [3~5]=gyro / [6~8]=mag
  }

  // 2. PRINT DATA TO M5 LCD (optional)
  // Gyroscope data
  StickCP2.Display.setCursor(30, 40);
  StickCP2.Display.printf("%6.2f  %6.2f  %6.2f      ", gyroX, gyroY, gyroZ);
  StickCP2.Display.setCursor(170, 40);
  StickCP2.Display.print("o/s");

  // Accelerometer data
  StickCP2.Display.setCursor(30, 50);
  StickCP2.Display.printf(" %5.2f   %5.2f   %5.2f   ", accX, accY, accZ);
  StickCP2.Display.setCursor(170, 50);
  StickCP2.Display.print("G");

  // AHRS data
  StickCP2.Display.setCursor(30, 80);
  StickCP2.Display.printf(" %5.2f   %5.2f   %5.2f   ", pitch, roll, yaw);

  // 3. SEND DATA VIA OSC
  // Gyroscope data
  sendFloatOscMessage("/gyroX", gyroX);
  sendFloatOscMessage("/gyroY", gyroY);
  sendFloatOscMessage("/gyroZ", gyroZ);

  // Accelerometer data
  sendFloatOscMessage("/accX", accX);
  sendFloatOscMessage("/accY", accY);
  sendFloatOscMessage("/accZ", accZ);

  // AHRS data
  sendFloatOscMessage("/pitch", pitch);
  sendFloatOscMessage("/roll", roll);
  sendFloatOscMessage("/yaw", yaw);

  // Add a short pause (e.g. 100 milliseconds) between cycles
  // This helps the WiFi router keep up
  delay(100);
}