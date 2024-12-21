
![Build Status](https://github.com/Xinyuan-LilyGO/T5S3-4.7-e-paper-PRO/actions/workflows/platformio.yml/badge.svg?event=push)

<p> 
<img src="https://img.shields.io/badge/ESP—IDF-5.1.1-ff3034" height="20px"></a>
<img src="https://img.shields.io/badge/PlatformIO-6.5.0-ff7f00" height="20px"></a>
<img src="https://img.shields.io/badge/Arduino-2.0.14-008284" height="20px"></a>
</p>


Examples




ESP-IDF     -v5.1.1
Arduino     -v2.0.14
PlatformIO  -espressif32@6.5.0



| Arduino IDE Setting                  | Value                              |
| ------------------------------------ | ---------------------------------- |
| Board                                | ***ESP32S3 Dev Module***           |
| Port                                 | Your port                          |
| USB CDC On Boot                      | Enable                             |
| CPU Frequency                        | 240MHZ(WiFi)                       |
| Core Debug Level                     | None                               |
| USB DFU On Boot                      | Disable                            |
| Erase All Flash Before Sketch Upload | Disable                            |
| Events Run On                        | Core1                              |
| Flash Mode                           | QIO 80MHZ                          |
| Flash Size                           | **16MB(128Mb)**                    |
| Arduino Runs On                      | Core1                              |
| USB Firmware MSC On Boot             | Disable                            |
| Partition Scheme                     | **16M Flash(3M APP/9.9MB FATFS)**  |
| PSRAM                                | **OPI PSRAM**                      |
| Upload Mode                          | **UART0/Hardware CDC**             |
| Upload Speed                         | 921600                             |
| USB Mode                             | **CDC and JTAG**                   |



# ESP-IDF

idf.py add-dependency "lvgl/lvgl=8.4.0"
idf.py add-dependency "cfscn/sensorlib=0.2.2"

