

#include <TinyGPS++.h>
#include "ExtensionIOXL9555.hpp"

#define BOARD_GPS_RXD 44
#define BOARD_GPS_TXD 43
#define SerialMon Serial
#define SerialGPS Serial2

#define SENSOR_SDA  39
#define SENSOR_SCL  40
#define SENSOR_IRQ  -1


const char *gpsStream =
  "$GPRMC,045103.000,A,3014.1984,N,09749.2872,W,0.67,161.46,030913,,,A*7C\r\n"
  "$GPGGA,045104.000,3014.1985,N,09749.2873,W,1,09,1.2,211.6,M,-22.5,M,,0000*62\r\n"
  "$GPRMC,045200.000,A,3014.3820,N,09748.9514,W,36.88,65.02,030913,,,A*77\r\n"
  "$GPGGA,045201.000,3014.3864,N,09748.9411,W,1,10,1.2,200.8,M,-22.5,M,,0000*6C\r\n"
  "$GPRMC,045251.000,A,3014.4275,N,09749.0626,W,0.51,217.94,030913,,,A*7D\r\n"
  "$GPGGA,045252.000,3014.4273,N,09749.0628,W,1,09,1.3,206.9,M,-22.5,M,,0000*6F\r\n";


// The TinyGPSPlus object
TinyGPSPlus gps;
ExtensionIOXL9555 io;

// The serial connection to the GPS device

void displayInfo(void);

void setup(void)
{
    SerialMon.begin(38400);
    
    // Set PORT0 as input,mask = 0xFF = all pin input
    io.configPort(ExtensionIOXL9555::PORT0, 0x00);
    // Set PORT1 as input,mask = 0xFF = all pin input
    io.configPort(ExtensionIOXL9555::PORT1, 0xFF);

    Serial.println("Power on LoRa and GPS!");
    io.digitalWrite(ExtensionIOXL9555::IO0, HIGH);

    delay(1500);

    SerialGPS.begin(38400, SERIAL_8N1, BOARD_GPS_RXD, BOARD_GPS_TXD);

    // Set the interrupt input to input pull-up
    if (SENSOR_IRQ > 0) {
        pinMode(SENSOR_IRQ, INPUT_PULLUP);
    }

    const uint8_t chip_address = XL9555_SLAVE_ADDRESS0;

    if (!io.init(Wire, SENSOR_SDA, SENSOR_SCL, chip_address)) {
        while (1) {
            Serial.println("Failed to find XL9555 - check your wiring!");
            delay(1000);
        }
    }

    Serial.println(F("DeviceExample.ino"));
    Serial.println(F("A simple demonstration of TinyGPSPlus with an attached GPS module"));
    Serial.print(F("Testing TinyGPSPlus library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
    Serial.println(F("by Mikal Hart"));
    Serial.println();
}

void loop(void)
{
    // while (SerialGPS.available())
    // {
    //     SerialMon.write(SerialGPS.read());
    // }
    // while (SerialMon.available())
    // {
    //     SerialGPS.write(SerialMon.read());
    // }
    // delay(1);

    const char * gpsStr = gpsStream;
    while (*gpsStr)
    if (gps.encode(*gpsStr++))
      displayInfo();

    // if (millis() > 5000 && gps.charsProcessed() < 10)
    // {
    //     Serial.println(F("No GPS detected: check wiring."));
    //     while(true);
    // }
    delay(1000);
}

void displayInfo()
{
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.println();
}
