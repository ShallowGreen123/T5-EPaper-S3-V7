

#include <TinyGPS++.h>

#define BOARD_GPS_RXD 44
#define BOARD_GPS_TXD 43
#define SerialMon Serial
#define SerialGPS Serial2

// The TinyGPSPlus object
TinyGPSPlus gps;

void setup(void)
{
    SerialMon.begin(38400);
    SerialGPS.begin(38400, SERIAL_8N1, BOARD_GPS_RXD, BOARD_GPS_TXD);

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