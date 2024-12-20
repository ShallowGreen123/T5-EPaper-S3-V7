
#include "bq27220.h"
#include "Free_Fonts.h" 
#include "Wire.h"

#define BOARD_I2C_SDA  39
#define BOARD_I2C_SCL  40

#define I2C_DEV_ADDR 0x55

BQ27220 bq;

char buf[32];

void i2c_scan(void)
{
    byte error, address;
    int nDevices = 0;
    Serial.println("Scanning for I2C devices ...");
    Wire.begin(BOARD_I2C_SDA, BOARD_I2C_SCL);
    for(address = 0x01; address < 0x7F; address++){
        Wire.beginTransmission(address);
        // 0: success.
        // 1: data too long to fit in transmit buffer.
        // 2: received NACK on transmit of address.
        // 3: received NACK on transmit of data.
        // 4: other error.
        // 5: timeout
        error = Wire.endTransmission();
        if(error == 0){ // 0: success.
            nDevices++;
            Serial.printf("I2C devices found [0x%x]\n", address);
        }
    }
    if (nDevices == 0){
        Serial.println("No I2C devices found");
    }
}

void setup()
{
    Serial.begin(115200);
    while (!Serial);

    i2c_scan();

    // uint8_t ctrl_cmd[] = {0x00, 0x01};
    // uint8_t read_dat[2] = {0};

    // bq.i2cWriteBytes(CommandControl, ctrl_cmd, 2);
    // delay(15);
    // // delayMicroseconds()
    // bq.i2cReadBytes(CommandMACData, read_dat, 2);
    
    // // Wire.write((uint8_t *)ctrl_cmd, 2);
    
    Serial.printf("device number:0x%x\n", bq.getDeviceNumber());

    BQ27220OperationStatus opr_st;
    bq.getOperationStatus(&opr_st);

    Serial.printf("OperationStatus.SEC: %d\n", opr_st.reg.SEC);

    bq.reset();
    // Serial.printf("FW version:0x%x\n", bq.readCtrlWord(Control_FW_VERSION));

}

BQ27220BatteryStatus batt;

void loop() 
{
    batt.full = bq.getBatterySt();
    Serial.printf("-------------------------------------------\n");
    Serial.printf("Status = %x\n", batt.full);
    Serial.printf("Status = %s\n", bq.getIsCharging() ? "Charging" : "Discharging");
    Serial.printf("Temp = %.2f K\n", (float)(bq.getTemp() / 10));
    Serial.printf("battery = %d mAh\n", bq.getRemainCap());
    Serial.printf("battery full= %d mAh\n", bq.getFullChargeCap());
    Serial.printf("Volt = %d mV\n", bq.getVolt(VOLT));
    Serial.printf("Volt Charg= %d mV\n", bq.getVolt(VOLT_CHARGING));
    Serial.printf("Curr Average=%d mA\n", bq.getCurr(CURR_AVERAGE));
    Serial.printf("Curr Instant=%d mA\n", bq.getCurr(CURR_INSTANT));
    Serial.printf("Curr Standby=%d mA\n", bq.getCurr(CURR_STANDBY));
    Serial.printf("Curr Charging=%d mA\n", bq.getCurr(CURR_CHARGING));

    delay(1000);
}


