

#include <TinyGPS++.h>
#include "ExtensionIOXL9555.hpp"

#define BOARD_GPS_RXD 44
#define BOARD_GPS_TXD 43
#define SerialMon Serial
#define SerialGPS Serial2

#define SENSOR_SDA  39
#define SENSOR_SCL  40
#define SENSOR_IRQ  -1


// The TinyGPSPlus object
TinyGPSPlus gps;
ExtensionIOXL9555 io;

void setup(void)
{
    SerialMon.begin(38400);
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

    // Set PORT0 as input,mask = 0xFF = all pin input
    io.configPort(ExtensionIOXL9555::PORT0, 0x00);
    // Set PORT1 as input,mask = 0xFF = all pin input
    io.configPort(ExtensionIOXL9555::PORT1, 0xFF);

    Serial.println("Power on LoRa and GPS!");
    io.digitalWrite(ExtensionIOXL9555::IO0, HIGH);

    delay(1500);
}

void loop(void)
{
    while (SerialGPS.available())
    {
        SerialMon.write(SerialGPS.read());
    }
    while (SerialMon.available())
    {
        SerialGPS.write(SerialMon.read());
    }
    delay(1);
}